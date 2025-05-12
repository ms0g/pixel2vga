# pixel2vga
pixel2vga is a command-line tool designed to convert RGB Color values to the VGA 256-Color palette.

<img src="assets/lenna.png" alt="image" width="402" height="auto"> <img src="assets/lenna256.png" alt="image" width="402" height="auto">
<img src="assets/johnlennon.png" alt="image" width="402" height="auto"> <img src="assets/johnlennon256.png" alt="image" width="402" height="auto">
<img src="assets/neo.jpg" alt="image" width="402" height="auto"> <img src="assets/neo256100.jpg" alt="image" width="402" height="auto">
<img src="assets/pris.jpg" alt="image" width="402" height="auto"> <img src="assets/pris256100.jpg" alt="image" width="402" height="auto">

## Usage
```bash
➜  ~ pixel2vga -h
OVERVIEW: A RGB to VGA Color converter 

USAGE: pixel2vga [options] <image file>

OPTIONS:
  -f, --format          File format[jpg <quality 0-100>?/png/bmp/tga/raw]
  -o, --outfile         Output file name
  -h, --help            Display available options
  -v, --version         Display the version of this program
```
```bash
➜  ~ pixel2vga -f jpg -o neo256100.jpg neo.jpg
```
Change JPG quality
```bash
➜  ~ pixel2vga -f jpg 1 -o pris2561.jpg pris.jpg
```
<img src="assets/pris2561.jpg" alt="image" width="402" height="auto">

```bash
➜  ~ pixel2vga -f jpg 10 -o pris25610.jpg pris.jpg
```
<img src="assets/pris25610.jpg" alt="image" width="402" height="auto">

## License
This project is licensed under the BSD 3-Clause License. See the LICENSE file for details.

