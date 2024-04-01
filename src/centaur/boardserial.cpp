// Copyright (C) 2024 Eric Sessoms
// See license at end of file

// Serial communication with the field array on a DGT Centaur board
//
// This implementation is based on the reverse engineering efforts of
// [EdNekebno](https://github.com/EdNekebno) and
// [wormstein](https://github.com/wormstein) in the original
// [DGTCentaurMods](https://github.com/DGTCentaurMods/DGTCentaurMods) project,
// licensed [GPL-3.0-or-later]
// (https://github.com/DGTCentaurMods/DGTCentaurMods/blob/37ca9c25ab4fd34acffabe0bc665e30e4e9d89b0/LICENSE.md).

#include "boardserial.h"
#include "../utility/sleep.h"

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <ctime>
#include <stdexcept>

#include <sys/file.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

// Shutdown serial connection to board
BoardSerial::~BoardSerial() {
    if (fd >= 0) {
        flock(fd, LOCK_UN);
        close(fd);
    }
}

static int open_serial(const char* serial_port) {
    const auto fd = open(serial_port, O_RDWR);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    if (flock(fd, LOCK_EX | LOCK_NB) != 0) {
        perror("flock");
        close(fd);
        return -1;
    }

    struct termios serial;
    if (tcgetattr(fd, &serial) != 0) {
        perror("tcgetattr");
        goto error;
    }

    cfmakeraw(&serial);
    cfsetspeed(&serial, B1000000);

    // Blocking read with 0.2s timeout
    serial.c_cc[VMIN]  = 0;
    serial.c_cc[VTIME] = 2; // deciseconds

    if (tcsetattr(fd, TCSAFLUSH, &serial) != 0) {
        perror("tcsetattr");
        goto error;
    }

    return fd;

error:
    flock(fd, LOCK_UN);
    close(fd);
    return -1;
}

static int read_serial(int fd, uint8_t* buf, int len) {
    assert(fd >= 0);
    assert(buf != NULL && len >= 0);

    auto receiving = buf;
    auto remaining = len;

    auto total_read = 0;
    auto num_read   = 0;
    while (num_read < remaining) {
        num_read = read(fd, receiving, remaining);
        if (num_read == 0) {
            break;
        }
        if (num_read < 0) {
            if (errno != EAGAIN) {
                perror("read");
                goto error;
            }
            num_read = 0;
        }
        total_read += num_read;
        receiving  += num_read;
        remaining  -= num_read;
        assert(remaining >= 0);
    }

    return total_read;

error:
    printf("read_serial error: flushing\n");
    if (tcflush(fd, TCIFLUSH) != 0) {
        perror("tcflush");
    }
    return 0;
}

// Drop any data currently on the line
// N.B., This does not query the board for any queued data (field events,
// button presses), so that data remains queued.  We're only concerned with
// getting communications up and running.
static void clear_serial(int fd) {
    // Wait for any pending writes to complete
    tcdrain(fd);
    sleep_ms(100);

    // Read anything there is to read
    uint8_t buf[256];
    while (read_serial(fd, buf, sizeof buf) > 0) {
        // Discard bufferred data
        tcflush(fd, TCIFLUSH);
        sleep_ms(100);
    }
}

static int checksum(const uint8_t* buf, int len) {
    assert(buf && len >= 0);
    unsigned sum = 0;
    for (auto i = 0; i != len; ++i) {
        sum += buf[i];
    }
    return sum % 128;
}

static bool valid_checksum(const uint8_t* buf, int len) {
    return buf[len - 1] == checksum(buf, len - 1);
}

// Read packet with defined length
static int read_packet(int fd, uint8_t* data, int length) {
    assert(fd >= 0);
    assert(data != NULL && length >= 256);

    // Read packet header {id, 0|1, len}
    const auto HEADER_LEN = 3;

    auto receiving  = data;
    auto packet_len = HEADER_LEN;
    auto total_read = 0;
    auto num_read   = 0;
    const auto timeout = time(NULL) + 2;
    while (total_read < packet_len && time(NULL) < timeout) {
        num_read    = read_serial(fd, receiving, length);
        total_read += num_read;
        receiving  += num_read;
        length     -= num_read;
        if (total_read >= HEADER_LEN) {
            packet_len = data[HEADER_LEN - 1];
        }
    }

    if (total_read != packet_len || !valid_checksum(data, total_read)) {
        printf("bad packet\n");
        return 0;
    }

    // printf("READ[%d]:", total_read);
    // for (int i = 0; i != total_read; ++i) {
    //     printf(" %d", data[i]);
    // }
    // printf("\n");

    return total_read;
}

// Receive packet with specific ID
static int recv_packet(int fd, int id, uint8_t* data, int length) {
    const auto timeout = time(NULL) + 2;
    while (time(NULL) < timeout) {
        const auto num_read = read_packet(fd, data, length);
        if (num_read > 0 && data[0] == id) {
            return num_read;
        }
    }
    return 0;
}

static int write_serial(int fd, const uint8_t* buf, int len) {
    assert(fd >= 0);
    assert(buf != NULL && len >= 1);
    assert(valid_checksum(buf, len));

    auto sending   = buf;
    auto remaining = len;

    auto total_written = 0;
    auto num_written   = 0;
    while (num_written < remaining) {
        num_written = write(fd, sending, remaining);
        if (num_written == 0) {
            break;
        }
        if (num_written < 0) {
            if (errno != EAGAIN) {
                perror("write");
                goto error;
            }
            num_written = 0;
        }
        total_written += num_written;
        sending   += num_written;
        remaining -= num_written;
        assert(remaining >= 0);
    }

    // Wait for serial transmission to complete
    if (tcdrain(fd) != 0) {
        perror("tcdrain");
        goto error;
    }

    // printf("WRITE[%d]:", total_written);
    // for (int i = 0; i != total_written; ++i) {
    //     printf(" %d", buf[i]);
    // }
    // printf("\n");

    return total_written;

error:
    printf("write_serial error, flushing\n");
    if (tcflush(fd, TCOFLUSH) != 0) {
        perror("tcflush");
    }
    return 0;
}

void BoardSerial::read_address() {
    assert(fd >= 0);

    addr[0] = 0;
    addr[1] = 0;

    const uint8_t request[4] = {135, 0, 0, 7};
    write_serial(fd, request, 4);

    uint8_t buf[256];
    const auto timeout = time(NULL) + 2;
    while (time(NULL) < timeout) {
        if (recv_packet(fd, 135, buf, sizeof buf) != 6) {
            continue;
        }

        addr[0] = buf[3];
        addr[1] = buf[4];
        return;
    }

    printf("No response from serial\n");
    printf("Board communication has been disabled\n");
    return;
}

// Initialize serial connection to board
BoardSerial::BoardSerial() {
    // Open device
    fd = open_serial("/dev/serial0");
    if (fd < 0) {
        throw runtime_error("Failed to open serial port");
    }

    // Flush traffic
    clear_serial(fd);

    // See if there's anything there
    read_address();
    if (addr[0] == 0 && addr[1] == 0) {
        close(fd);
        throw runtime_error("Failed to read board address");
    }
}

static void add_checksum(uint8_t* buf, int len) {
    buf[len - 1] = checksum(buf, len - 1);
    assert(valid_checksum(buf, len));
}

void BoardSerial::build_packet(uint8_t* buf, int addr_pos, int len) {
    assert(buf != NULL);

    // Must have space available for both the address and the checksum
    assert(0 <= addr_pos && addr_pos + 3 <= len);

    // Check packet structure
    if (addr_pos == 3) {
        assert(buf[1] == 0);
        assert(buf[2] == len);
    }

    buf[addr_pos + 0] = addr[0];
    buf[addr_pos + 1] = addr[1];
    add_checksum(buf, len);
}

int BoardSerial::write_board(uint8_t* buf, int addr_pos, int len) {
    build_packet(buf, addr_pos, len);
    const auto num_written = write_serial(fd, buf, len);
    return num_written == len ? 0 : 1;
}

// Read battery and charging status
int BoardSerial::chargingstate() {
    uint8_t request[4] = {152};
    write_board(request, 1, sizeof request);

    uint8_t buf[256];
    const auto timeout = time(NULL) + 2;
    while (time(NULL) < timeout) {
        if (recv_packet(fd, 181, buf, sizeof buf) == 7) {
            break;
        }
    }

    return buf[5];
}

// Read current state of board fields
Bitmap BoardSerial::boardstate() {
    uint8_t request[7] = {240, 0, 7, 0, 0, 127, 0};
    write_board(request, 3, sizeof request);

    uint8_t buf[256];
    size_t num_read = 0;
    while (num_read < 128 + 7) {
        num_read = read_serial(fd, buf, sizeof buf);
    }

    Bitmap boardstate = 0;
    Bitmap mask = 1;

    auto resp = &buf[6];
    for (auto i = 0; i != 64; ++i) {
        const auto value = 256 * resp[2*i] + resp[2*i + 1];
        if (300 <= value && value <= 32000) {
            boardstate |= mask;
        }
        mask <<= 1;
    }

    return boardstate;
}

// Read field events from board
int BoardSerial::readdata(uint8_t* buf, int len) {
    assert(fd >= 0);
    assert(buf && len >= 256);

    uint8_t request[4] = {131};
    write_board(request, 1, sizeof request);

    const auto timeout = time(NULL) + 2;
    while (time(NULL) < timeout) {
        const auto num_read = read_packet(fd, buf, len);
        if (num_read >= 6 && (buf[0] == 131 || buf[0] == 133)) {
            return num_read;
        }
    }

    return 0;
}

void BoardSerial::buttons(Buttons& press, Buttons& release) {
    uint8_t request[4] = {148};
    write_board(request, 1, sizeof request);

    uint8_t buf[256];
    const auto timeout = time(NULL) + 2;
    while (time(NULL) < timeout) {
        if (recv_packet(fd, 177, buf, sizeof buf) >= 6) {
            break;
        }
    }

    if (buf[0] != 177 || buf[2] < 16) {
        press   = Buttons();
        release = Buttons();
        return;
    }

    const auto BUTTON_MASK = 0x5F;
    assert(buf[ 5] ==  0);
    assert(buf[ 6] == 20);
    assert(buf[ 7] == 10);
    assert(buf[ 8] ==  5);
    assert(buf[ 9] == (buf[ 9] & BUTTON_MASK));
    assert(buf[10] == (buf[10] & BUTTON_MASK));
    // buf[13] looks like duration, maybe deciseconds

    press   = Buttons(buf[ 9]);
    release = Buttons(buf[10]);
}

int BoardSerial::leds_off() {
    uint8_t request[7] = {176, 0, 7};
    return write_board(request, 3, sizeof request);
}

int BoardSerial::led_flash() {
    uint8_t request[10] = {176, 0, 10, 0, 0, 5, 10, 0, 1, 0};
    return write_board(request, 3, sizeof request);
}

int BoardSerial::led(int square) {
    const auto intensity = 5;
    uint8_t request[11] = {176, 0, 11, 0, 0, 5, 10, 1, intensity, uint8_t(square), 0};
    return write_board(request, 3, sizeof request);
}

int BoardSerial::led_array(const int* squares, int num_squares) {
    assert(squares && num_squares >= 0);

    const auto speed     = 3;
    const auto intensity = 5;
    uint8_t request[128] = {176, 0, 0, 0, 0, 5, speed, 0, intensity};

    auto len = 9;
    for (auto i = 0; i != num_squares; ++i) {
        request[len++] = squares[i];
    }
    request[len++] = 0;
    request[2] = len;
    return write_board(request, 3, len);
}

int BoardSerial::led_from_to(int from, int to) {
    const int squares[2] = {from, to};
    return led_array(squares, 2);
}

// pitch, duration pairs
static const uint8_t sound_data[] = {
    76,  8,          // SOUND_GENERAL
    76, 64,          // SOUND_FACTORY
    76,  8, 72,  8,  // SOUND_POWER_OFF
    72,  8, 76,  8,  // SOUND_POWER_ON
    78, 12, 72, 16,  // SOUND_WRONG
    72,  8,          // SOUND_WRONG_MOVE
};

// Index into sound_data
static const int sounds[] = {
     0,  // SOUND_GENERAL
     2,  // SOUND_FACTORY
     4,  // SOUND_POWER_OFF
     8,  // SOUND_POWER_ON
    12,  // SOUND_WRONG
    16,  // SOUND_WRONG_MOVE
    18,  // SOUND_NONE
};

int BoardSerial::play_sound(Sound sound) {
    if (sound < SOUND_GENERAL || SOUND_NONE <= sound) {
        return 0;
    }

    uint8_t request[10] = {177};
    auto len = 5;
    for (auto i = sounds[sound]; i != sounds[sound + 1]; ++i) {
        request[len++] = sound_data[i];
    }
    request[len++] = 0;
    request[2] = len;
    return write_board(request, 3, len);
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
