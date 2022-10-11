This project is work in progress. Will be updating the README in a couple of days on how to use it. This is an open source project for anyone who wants to fabricate PCBs at home at a very low cost and high precision for quick debugging and prototyping. Will be making many more advancements in the future. I'll also upload a working Video of the entire process in action soon. 



How to build PlatformIO based project
=====================================

1. [Install PlatformIO Core](https://docs.platformio.org/page/core.html)
2. Download [development platform with examples](https://github.com/platformio/platform-atmelavr/archive/develop.zip)
3. Extract ZIP archive
4. Run these commands:

```shell
# Change directory to example
$ cd platform-atmelavr/examples/arduino-blink

# Build project
$ pio run

# Upload firmware
$ pio run --target upload

# Build specific environment
$ pio run -e uno

# Upload firmware for the specific environment
$ pio run -e uno --target upload

# Clean build files
$ pio run --target clean
```
