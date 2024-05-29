# The Raccoon's Centaur Mods

RCM is a small collection of software-only enhancements for the DGT Centaur
electronic chessboard. It is written for personal use and made available only
on the off-chance that someone may find it useful.

You probably don't want it. If you're looking for mods to try on your Centaur
board, try one of these:

-   [DGT Centaur mods](https://github.com/DGTCentaurMods/DGTCentaurMods)
-   [Alistair DGT Centaur mods](https://github.com/Alistair-Crompton/DGTCentaurMods)

RCM is not a fork or derivative of either, but an entirely new development.

## License

[GPL-3.0-or-later](COPYING)

## Status

In development.

## Dependencies

-   Jansson
-   Libmicrohttpd
-   Libpng
-   Libpcre2
-   pigpio
-   SQLite

```bash
sudo apt-get install libjansson-dev libmicrohttpd-dev libpng-dev libpcre2-dev libpgipio-dev libsqlite3-dev
```

## Building

Install compiler

```bash
sudo apt-get install cmake
```

clone repo and cd into rcm directory

```bash
mkdir bin
cmake -DCMAKE_BUILD_TYPE=Debug -Bbin
cmake --build bin
bin/check
sudo bin/rcm
```

## References

-   [2.9inch e-Paper HAT (D) Manual](<https://www.waveshare.com/wiki/2.9inch_e-Paper_HAT_(D)>)
-   [2.9inch e-Paper (D) Specifications](https://files.waveshare.com/upload/b/b5/2.9inch_e-Paper_%28D%29_Specification.pdf)
-   [Pi GPIO](https://abyz.me.uk/rpi/pigpio/cif.html)

## Notes

```bash
sudo raspi-config
Choose Interface Options -> SPI -> Yes
Choose Interface Options -> Serial Port

sudo $(EDITOR) /boot/config.txt
dtparam=spi=on
enable_uart=1

sudo reboot

ls -al /dev/serial0
ls -al /dev/spi*
```
