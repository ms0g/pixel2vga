# pixel2vga

## Overview
pixel2vga is a command-line tool designed to convert RGB Color values to the VGA 256-Color palette.

<img src="assets/lenna.png" alt="image" width="402" height="auto"> <img src="assets/lenna_vga.png" alt="image" width="402" height="auto">
<img src="assets/johnlennon.png" alt="image" width="402" height="auto"> <img src="assets/johnlennon_vga.png" alt="image" width="402" height="auto">
<img src="assets/neo.jpg" alt="image" width="402" height="auto"> <img src="assets/neo_vga.jpg" alt="image" width="402" height="auto">
<img src="assets/pris.jpg" alt="image" width="402" height="auto"> <img src="assets/pris_vga.jpg" alt="image" width="402" height="auto">

## Usage
```bash
➜  ~ pixel2vga -h
OVERVIEW: A RGB to VGA Color converter 

USAGE: pixel2vga [options] <image file>

OPTIONS:
  -f, --format          File format[jpg/png/bmp/tga/raw]
  -o, --outfile         Output file name
  -h, --help            Display available options
  -v, --version         Display the version of this program
```
## Run
```bash
➜  ~ pixel2vga -f jpg -o neo_vga.jpg neo.jpg
```
## License

This project is licensed under the BSD 3-Clause License. See the LICENSE file for details.

## Credits

Developed by M. Sami Gürpınar