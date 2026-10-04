# BMP ASCII

BMP ASCII is a command-line tool that converts 24-bit uncompressed BMP images into ASCII art.
The generated ASCII art can either be printed directly to the terminal or saved to a file.

## Usage

    bmp-ascii.exe <bmp-image-path> [options]

The `<bmp-image-path>` argument is required.

## Options

| Short | Long | Description |
| --- | --- | --- |
| `-h` | `--hirez` | Enables the high-resolution ASCII palette |
| `-l` | `--luma` | Enables luma approximation for image grayscale detection |
| `-n` | `--no-print` | Disables terminal printing |
| `-r` | `--resolution <value>` | Sets the resolution of the generated ASCII art. Default: `5` |
| `-s` | `--savepath <value>` | Specifies the path where the generated ASCII art will be saved |

> **NOTE:** When `--no-print` is specified, a filepath must be provided using `--savepath`.

> **NOTE:** `--resolution` is inverted, a lower value produces higher quality.

## Building
### Requirements
- GCC
- GNU Make

### Building from source
```bash
git clone https://github.com/TuttiAnkka/BMP-Ascii.git
cd BMP-Ascii
make
```


