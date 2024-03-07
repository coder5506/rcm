// Copyright (C) 2024 Eric Sessoms
// See license at end of file

// This file provides serial communication with the field array on a
// DGT Centaur board.
//
// This implementation is based on the reverse engineering efforts of
// [EdNekebno](https://github.com/EdNekebno) and
// [wormstein](https://github.com/wormstein) in the original
// [DGTCentaurMods](https://github.com/DGTCentaurMods/DGTCentaurMods) project,
// licensed [GPL-3.0-or-later]
// (https://github.com/DGTCentaurMods/DGTCentaurMods/blob/37ca9c25ab4fd34acffabe0bc665e30e4e9d89b0/LICENSE.md).

#include "../boardserial.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include <errno.h>
#include <sys/file.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

static struct {
    int     fd;
    uint8_t addr[2];
} boardserial = {
    .fd   = -1,
    .addr = {0, 0},
};

// Shutdown serial connection to board
void boardserial_close(void) {
    if (boardserial.fd >= 0) {
        flock(boardserial.fd, LOCK_UN);
        close(boardserial.fd);
    }
    boardserial.fd = -1;
    boardserial.addr[0] = 0;
    boardserial.addr[1] = 0;
}

static int open_serial(const char *serial_port) {
    assert(serial_port != NULL && serial_port[0] != '\0');

    const int fd = open(serial_port, O_RDWR);
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

static int checksum(const uint8_t *buf, int len) {
    assert(buf && len >= 0);
    unsigned int sum = 0;
    for (int i = 0; i != len; ++i) {
        sum += buf[i];
    }
    return sum % 128;
}

static bool valid_checksum(const uint8_t *buf, int len) {
    return buf[len - 1] == checksum(buf, len - 1);
}

static int write_serial(int fd, const uint8_t *buf, int len) {
    assert(fd >= 0);
    assert(buf != NULL && len >= 1);
    assert(valid_checksum(buf, len));

    const uint8_t *sending   = buf;
    int            remaining = len;

    int total_written = 0;
    int num_written   = 0;
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

    printf("WRITE[%d]:", total_written);
    for (int i = 0; i != total_written; ++i) {
        printf(" %d", buf[i]);
    }
    printf("\n");

    return total_written;

error:
    printf("write_serial error, flushing\n");
    if (tcflush(fd, TCOFLUSH) != 0) {
        perror("tcflush");
    }
    return 0;
}

static int read_serial(int fd, uint8_t *buf, int len) {
    assert(fd >= 0);
    assert(buf != NULL && len >= 0);

    uint8_t *receiving = buf;
    int      remaining = len;

    int  total_read = 0;
    int  num_read   = 0;
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

// Read packet with defined length
static int read_packet(int fd, uint8_t *data, int length) {
    assert(fd >= 0);
    assert(data != NULL && length >= 256);

    // Read packet header = {id, 0|1, len}
    const int HEADER_LEN = 3;

    uint8_t *receiving  = data;
    int  packet_len = HEADER_LEN;
    int  total_read = 0;
    int  num_read   = 0;
    const time_t timeout = time(NULL) + 2;
    while (total_read < packet_len && time(NULL) < timeout) {
        num_read = read_serial(fd, receiving, length);
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

    printf("READ[%d]:", total_read);
    for (int i = 0; i != total_read; ++i) {
        printf(" %d", data[i]);
    }
    printf("\n");

    return total_read;
}

// Receive packet with specific ID
static int recv_packet(int fd, int id, uint8_t *data, int length) {
    const time_t timeout = time(NULL) + 2;
    while (time(NULL) < timeout) {
        const int num_read = read_packet(fd, data, length);
        if (num_read > 0 && data[0] == id) {
            return num_read;
        }
    }
    return 0;
}

static int read_address(void) {
    assert(boardserial.fd >= 0);

    boardserial.addr[0] = 0;
    boardserial.addr[1] = 0;

    const uint8_t request[4] = {135, 0, 0, 7};
    write_serial(boardserial.fd, request, 4);

    uint8_t buf[256];
    const time_t timeout = time(NULL) + 2;
    while (time(NULL) < timeout) {
        if (recv_packet(boardserial.fd, 135, buf, sizeof buf) != 6) {
            continue;
        }

        boardserial.addr[0] = buf[3];
        boardserial.addr[1] = buf[4];
        printf("Board address: %d %d\n", boardserial.addr[0], boardserial.addr[1]);
        return 0;
    }

    printf("No response from serial\n");
    printf("Board communication has been disabled\n");
    return 1;
}

static void add_checksum(uint8_t *buf, int len) {
    buf[len - 1] = checksum(buf, len - 1);
    assert(valid_checksum(buf, len));
}

static void build_packet(uint8_t *buf, int addr_pos, int len) {
    assert(buf != NULL);

    // Must have space available for both the address and the checksum
    assert(0 <= addr_pos && addr_pos + 3 <= len);

    // Check packet structure
    if (addr_pos == 3) {
        assert(buf[1] == 0);
        assert(buf[2] == len);
    }

    buf[addr_pos + 0] = boardserial.addr[0];
    buf[addr_pos + 1] = boardserial.addr[1];
    add_checksum(buf, len);
}

static int write_board(uint8_t *buf, int addr_pos, int len) {
    build_packet(buf, addr_pos, len);
    const int num_written = write_serial(boardserial.fd, buf, len);
    return num_written == len ? 0 : 1;
}

// Read field events from board
int boardserial_readdata(uint8_t *buf, int len) {
    assert(boardserial.fd >= 0);
    assert(buf && len >= 256);

    uint8_t request[4] = {131};
    write_board(request, 1, sizeof request);

    const time_t timeout = time(NULL) + 2;
    while (time(NULL) < timeout) {
        const int num_read = read_packet(boardserial.fd, buf, len);
        if (num_read >= 6 && (buf[0] == 131 || buf[0] == 133)) {
            return num_read;
        }
    }

    return 0;
}

// Read field events until idle
static int clear_data(void) {
    assert(boardserial.fd >= 0);

    uint8_t buf[256];
    const time_t timeout = time(NULL) + 3;
    while (time(NULL) < timeout) {
        if (boardserial_readdata(buf, sizeof buf) == 6 && buf[0] == 133) {
            return 0;
        }
    }

    printf("Failed to clear board data\n");
    return 1;
}

int boardserial_leds_off(void) {
    uint8_t request[7] = {176, 0, 7};
    return write_board(request, 3, sizeof request);
}

int boardserial_led_flash(void) {
    uint8_t request[10] = {176, 0, 10, 0, 0, 5, 10, 0, 1, 0};
    return write_board(request, 3, sizeof request);
}

int boardserial_led(int square) {
    const int intensity = 5;
    uint8_t request[11] = {176, 0, 11, 0, 0, 5, 10, 1, intensity, square, 0};
    return write_board(request, 3, sizeof request);
}

int boardserial_led_array(const int *squares, int num_squares) {
    assert(squares && num_squares >= 0);

    const int speed     = 3;
    const int intensity = 5;
    uint8_t request[128] = {[0] = 176, [5] = 5, [6] = speed, [8] = intensity};

    int len = 9;
    for (int i = 0; i != num_squares; ++i) {
        request[len++] = squares[i];
    }
    request[len++] = 0;
    request[2] = len;
    return write_board(request, 3, len);
}

int boardserial_led_from_to(int from, int to) {
    const int squares[2] = {from, to};
    return boardserial_led_array(squares, 2);
}

// Initialize serial connection to board
int boardserial_open(void) {
    boardserial.fd = open_serial("/dev/serial0");
    if (boardserial.fd < 0) {
        goto error;
    }

    if (read_address() != 0) {
        goto error;
    }

    // Drain events
    if (clear_data() != 0) {
        goto error;
    }

    if (boardserial_leds_off() != 0) {
        goto error;
    }

    return 0;

error:
    boardserial_close();
    return 1;
}

// Read current state of board fields
uint64_t boardserial_boardstate(void) {
    uint8_t request[] = {240, 0, 7, 0, 0, 127, 0};
    write_board(request, 3, sizeof request);

    uint8_t buf[256];
    size_t num_read = 0;
    while (num_read < 128 + 7) {
        num_read = read_serial(boardserial.fd, buf, sizeof buf);
    }

    uint64_t boardstate = 0;
    uint64_t mask = 1;

    const uint8_t *resp = &buf[6];
    for (int i = 0; i != 64; ++i) {
        const int value = 256 * resp[2*i] + resp[2*i + 1];
        if (300 <= value && value <= 32000) {
            boardstate |= mask;
        }
        mask <<= 1;
    }

    return boardstate;
}

// Read battery and charging status
int boardserial_chargingstate(void) {
    uint8_t request[4] = {152};
    write_board(request, 1, sizeof request);

    uint8_t buf[256];
    const time_t timeout = time(NULL) + 2;
    while (time(NULL) < timeout) {
        if (recv_packet(boardserial.fd, 181, buf, sizeof buf) == 7) {
            break;
        }
    }

    return buf[5];
}

#if 0
static int clear_serial(const struct BoardSerial *board) {
    assert(board != NULL && board->fd >= 0);

    uint8_t request[4] = {148};
    write_board(board, request, 1, sizeof request);

    uint8_t expected[6] = {177, 0, 6};
    build_packet(board, expected, 3, sizeof expected);

    uint8_t buf[256];
    const time_t timeout = time(NULL) + 3;
    while (time(NULL) < timeout) {
        if (recv_packet(board->fd, 177, buf, sizeof buf) == 6) {
            return 0;
        }
    }

    printf("Failed to clear serial\n");
    return 1;
}
#endif

#if 0
static int sleep_board(const struct BoardSerial *board) {
    uint8_t data[7] = {178, 0, 7, 0, 0, 10, 0};
    return write_board(board, data, 3, sizeof data);
}
#endif

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
