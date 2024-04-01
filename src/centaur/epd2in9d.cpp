// Copyright (C) 2024 Eric Sessoms
// See license at end of file

// Access the Waveshare 2.9 inch e-Paper display on a DGT Centaur
//
// The display is documented at:
// - https://www.waveshare.com/wiki/2.9inch_e-Paper_HAT_(D)
// - https://files.waveshare.com/upload/b/b5/2.9inch_e-Paper_%28D%29_Specification.pdf
//
// This implementation borrows from MIT-licensed example code provided by Waveshare:
// - https://github.com/waveshareteam/e-Paper/blob/master/RaspberryPi_JetsonNano/c/lib/e-Paper/EPD_2in9d.c

#include "epd2in9d.h"

#include <alloca.h>
#include <cstdio>
#include <cstring>
#include <stdexcept>

#include <pigpio.h>

using namespace std;

//
// General Purpose Input Output
//

enum Pin {
    PIN_RESET         = 12,
    PIN_COMMAND_DATA  = 16,  // 0: Command, 1: Data
    PIN_CLEAR_TO_SEND = 18,
    PIN_BUSY          = 13,
};

// Use destructor to ensure gpioTerminate() is always called, otherwise crashes
// result in DMA memory leaks which can only be recovered via reboot.
Gpio::~Gpio() {
    gpioWrite(PIN_RESET, 0);
    gpioWrite(PIN_COMMAND_DATA, 0);
	gpioWrite(PIN_CLEAR_TO_SEND, 0);
    gpioTerminate();
}

Gpio::Gpio() {
    if (gpioInitialise() < 0) {
        throw runtime_error("Failed to initialize GPIO");
    }
    gpioSetMode(PIN_RESET, PI_OUTPUT);
    gpioSetMode(PIN_COMMAND_DATA, PI_OUTPUT);
    gpioSetMode(PIN_CLEAR_TO_SEND, PI_OUTPUT);
    gpioSetMode(PIN_BUSY, PI_INPUT);
    gpioSetPullUpDown(PIN_BUSY, PI_PUD_UP);
    gpioWrite(PIN_CLEAR_TO_SEND, 1);
}

static void delay_ms(uint32_t ms) {
    gpioDelay(1000 * ms /* micros */);
}

// Trigger software reset of e-Paper display
static void gpio_reset() {
    for (int i = 0; i != 3; ++i) {
        gpioWrite(PIN_RESET, 1);
        delay_ms(20);
        gpioWrite(PIN_RESET, 0);
        delay_ms(2);
    }
    gpioWrite(PIN_RESET, 1);
    delay_ms(20);
}

//
// Serial Peripheral Interface
//

Spi::~Spi() {
    spiClose(handle);
}

Spi::Spi() {
    // e-Paper display is on the auxilliary SPI peripheral
    const auto SPI_AUX = 0x100;
    handle = spiOpen(0, 4'000'000, SPI_AUX);
    if (handle < 0) {
        throw runtime_error("Failed to initialize SPI");
    }
}

void Spi::send_byte(int value, int command_data) {
    uint8_t byte = value;
    gpioWrite(PIN_COMMAND_DATA, command_data);
    gpioWrite(PIN_CLEAR_TO_SEND, 0);
    spiWrite(handle, (char*)&byte, 1);
    gpioWrite(PIN_CLEAR_TO_SEND, 1);
}

void Spi::send_command(int command) {
    send_byte(command, 0);
}

void Spi::send_data(int data) {
    send_byte(data, 1);
}

int Spi::send_array(const uint8_t* buf, size_t len) {
    gpioWrite(PIN_COMMAND_DATA, 1);
    gpioWrite(PIN_CLEAR_TO_SEND, 0);

    auto num_written = spiWrite(handle, (char*)buf, len);
    gpioWrite(PIN_CLEAR_TO_SEND, 1);

    return num_written;
}

//
// e-Paper Display
//

enum Command {
    COMMAND_PSR   = 0x00,  // Panel Setting
    COMMAND_PWR   = 0x01,  // Power Setting
    COMMAND_POF   = 0x02,  // Power OFF
    COMMAND_PON   = 0x04,  // Power ON
    COMMAND_BTST  = 0x06,  // Booster Soft-Start
    COMMAND_DSLP  = 0x07,  // Deep Sleep
    COMMAND_DTM1  = 0x10,  // Data Start Transmission 1
    COMMAND_DSP   = 0x11,  // Data Stop
    COMMAND_DRF   = 0x12,  // Display Refresh
    COMMAND_DTM2  = 0x13,  // Data Start Transmission 2 (hypothetical)
    COMMAND_LUTC  = 0x20,  // VCOM LUT
    COMMAND_LUTWW = 0x21,  // W2W LUT
    COMMAND_LUTBW = 0x22,  // B2W LUT
    COMMAND_LUTWB = 0x23,  // W2B LUT
    COMMAND_LUTBB = 0x24,  // B2B LUT
    COMMAND_PLL   = 0x30,  // PLL Control
    COMMAND_CDI   = 0x50,  // VCOM and Data Interval Setting
    COMMAND_TRES  = 0x61,  // Resolution Setting
    COMMAND_FLG   = 0x71,  // Get Status
    COMMAND_VDCS  = 0x82,  // VCM_DC Setting
    COMMAND_PTL   = 0x90,  // Partial Window
    COMMAND_PTIN  = 0x91,  // Partial In
};

Epd2in9d::~Epd2in9d() {
    sleep();
}

Epd2in9d::Epd2in9d() {
}

void Epd2in9d::read_busy() {
    do {
        spi.send_command(COMMAND_FLG);
    } while ((gpioRead(PIN_BUSY) & 1) == 0);
    delay_ms(20);
}

void Epd2in9d::sleep() {
    spi.send_command(COMMAND_CDI);
    spi.send_data(0xf7);
    spi.send_command(COMMAND_POF);
    read_busy();
    spi.send_command(COMMAND_DSLP);
    spi.send_data(0xA5);

    // Reinit after sleep/wake
    lut_ready = false;

    // Wait at least 2s before doing anything else
    delay_ms(2000);
}

void Epd2in9d::wake() {
    gpio_reset();

	spi.send_command(COMMAND_PON);
	read_busy();

    // LUT from OTP  KW-BF  KWR-AF  BWROTP 0f  BWOTP 1f
	spi.send_command(COMMAND_PSR);
	spi.send_data(0x1f);

	spi.send_command(COMMAND_TRES);
	spi.send_data(SCREEN_WIDTH);         // HRES: Horizontal resolution
	spi.send_data(SCREEN_HEIGHT / 256);  // VRES: Vertical resolution
	spi.send_data(SCREEN_HEIGHT % 256);

    // WBmode: VBDF 17|D7  VBDW 97  VBDB 57  WBRmode: VBDF F7  VBDW 77  VBDB 37  VBDR B7
	spi.send_command(COMMAND_CDI);
	spi.send_data(0x97);

    // Reinit after sleep/wake
    lut_ready = false;
}

// Look-Up Time
void Epd2in9d::lut_tables() {
    const uint8_t EPD_2IN9D_lut_vcom1[] = {
        0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
    };
    spi.send_command(COMMAND_LUTC);
    spi.send_array(EPD_2IN9D_lut_vcom1, sizeof EPD_2IN9D_lut_vcom1);

    const uint8_t EPD_2IN9D_lut_ww1[] = {
        0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    spi.send_command(COMMAND_LUTWW);
    spi.send_array(EPD_2IN9D_lut_ww1, sizeof EPD_2IN9D_lut_ww1);

    const uint8_t EPD_2IN9D_lut_bw1[] = {
        0x80, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    spi.send_command(COMMAND_LUTBW);
    spi.send_array(EPD_2IN9D_lut_bw1, sizeof EPD_2IN9D_lut_bw1);

    const uint8_t EPD_2IN9D_lut_wb1[] = {
        0x40, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    spi.send_command(COMMAND_LUTWB);
    spi.send_array(EPD_2IN9D_lut_wb1, sizeof EPD_2IN9D_lut_wb1);

    const uint8_t EPD_2IN9D_lut_bb1[] = {
        0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    spi.send_command(COMMAND_LUTBB);
    spi.send_array(EPD_2IN9D_lut_bb1, sizeof EPD_2IN9D_lut_bb1);
}

void Epd2in9d::init_lut() {
    if (lut_ready) {
        return;
    }
    lut_ready = true;

    spi.send_command(COMMAND_PWR);
    spi.send_data(0x03);
    spi.send_data(0x00);
    spi.send_data(0x2b);
    spi.send_data(0x2b);
    spi.send_data(0x03);

    spi.send_command(COMMAND_BTST);
    spi.send_data(0x17);  // BT_PHA[7:0]
    spi.send_data(0x17);  // BT_PHB[7:0]
    spi.send_data(0x17);  // BT_PHC[5:0]

    spi.send_command(COMMAND_PON);
    read_busy();

    // LUT from OTP，128x296
    spi.send_command(COMMAND_PSR);
    spi.send_data(0xbf);

    // 3a 100HZ   29 150Hz 39 200HZ	31 171HZ
    spi.send_command(COMMAND_PLL);
    spi.send_data(0x3a);

    spi.send_command(COMMAND_TRES);
    spi.send_data(SCREEN_WIDTH);         // HRES: Horizontal Resolution
    spi.send_data(SCREEN_HEIGHT / 256);  // VRES: Vertical Resoltion
    spi.send_data(SCREEN_HEIGHT % 256);

    spi.send_command(COMMAND_VDCS);
    spi.send_data(0x12);

    spi.send_command(COMMAND_CDI);
    spi.send_data(0x97);

    lut_tables();
}

void Epd2in9d::refresh_screen(void) {
    spi.send_command(COMMAND_DRF);
    delay_ms(10);  // Must be at least 200us
    read_busy();
}

// Voodoo follows:
//
// The reference manual for the 2.9" display defines the command 0x10 for "Data
// Start Transmission 1" and does not define any command 0x13.  However, the
// Waveshare examples always transmit in two passes: first a "zero" pass with
// 0x10 followed by a data pass with 0x13.
//
// Experimentally, using 0x10 on its own does not seem to do anything, while
// using 0x13 on its own is not entirely reliable.
//
// There is a note in the reference manual: 'In B/W mode, this command writes
// "OLD" data to SRAM.'  My best guess is that the undocumented 0x13 is in-fact
// a "Data Start Transmission 2" to write the "NEW" data.
//
// It is at least consistent in that the 0x10 "OLD" data does not seem to be
// required for a partial update, only the (undocumented) 0x13 "NEW" data.

#define PIXEL_BLACK  0
#define PIXEL_WHITE -1

#define WIDTH_BYTES  ((SCREEN_WIDTH + 7) / 8)
#define SCREEN_BYTES (WIDTH_BYTES * SCREEN_HEIGHT)
static const uint8_t black_buffer[SCREEN_BYTES] = {PIXEL_BLACK};

// Fully refresh display
void Epd2in9d::display(const uint8_t* data) {
    spi.send_command(COMMAND_DTM1);
    spi.send_array(black_buffer, SCREEN_BYTES);

    spi.send_command(COMMAND_DTM2);
    spi.send_array(data, SCREEN_BYTES);

    refresh_screen();
}

// Partially update display
void Epd2in9d::update(const uint8_t* data) {
    init_lut();
    spi.send_command(COMMAND_PTIN);
    spi.send_command(COMMAND_PTL);
    spi.send_data(0);                        // HRST: Horizontal Start
    spi.send_data(SCREEN_WIDTH - 1);         // HRED: Horizontal End

    spi.send_data(0);                        // VRST: Vertical Start
    spi.send_data(0);
    spi.send_data(SCREEN_HEIGHT / 256);      // VRED: Vertical End
    spi.send_data(SCREEN_HEIGHT % 256 - 1);

    spi.send_command(COMMAND_DTM2);
    spi.send_array(data, SCREEN_BYTES);
    refresh_screen();
}

// This file is part of the Raccoon's Centaur Mods (RCM).
//
// RCM is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RCM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
