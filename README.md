# BSSE Image Editor

A 24-bit BMP image editor built in C, using the [IUP](https://www.tecgraf.puc-rio.br/iup/) toolkit for the GUI. Built as a final project for BSSE 1849.

## Features

- **Open** — load any 24-bit BMP image (rejects other formats with an error popup instead of crashing)
- **Apply Grayscale** — convert the image to grayscale
- **Brightness Adjustment** — increase/decrease brightness by a user-specified offset
- **Invert selected image** — invert all pixel colors
- **Flip Horizontally / Flip Vertically**
- **Rotate image by 90 degrees**
- **Crop image** — crop to a user-specified X, Y, Width, and Height
- **Blur image** — box blur convolution
- **Increase Sharpness of the image** — sharpening convolution kernel
- **Undo last change** — step back through edit history
- **Save image** — write the current edited image out to a new BMP file, with a user-specified filename

Every editing operation keeps a history so **Undo** can step back to any previous state.

## How to Compile

Requires the IUP library installed on your system.

```bash
gcc BSSE_1849_Final_Project.c -liup -o image_editor.out
./image_editor.out
```

Tested on Ubuntu with IUP 3.32.

## How to Use

1. Run the program — an empty editor window opens.
2. Click **Open image** and select a 24-bit BMP file.
3. Use the buttons on the right panel to apply edits. Some operations (Brightness, Crop, Save) will pop up a small dialog asking for parameters.
4. Click **Undo last change** to revert the most recent edit.
5. Click **Save image** and enter a filename to save your result.

## Screenshots

**Main editor window**
![Main editor window](screenshots/01_main_window.png)

**Apply Grayscale**
![Grayscale applied](screenshots/02_grayscale.png)

**Brightness Adjustment dialog**
![Brightness Adjustment dialog](screenshots/03_brightness_dialog.png)

**Brightness applied**
![Brightness applied](screenshots/04_brightness_applied.png)

**Invert selected image**
![Invert applied](screenshots/05_invert.png)

**Flip Horizontally**
![Flip horizontally](screenshots/06_flip_horizontal.png)

**Flip Vertically**
![Flip vertically](screenshots/07_flip_vertical.png)

**Rotate image by 90 degrees**
![Rotate 90 degrees](screenshots/08_rotate_90.png)

**Crop image**
![Crop applied](screenshots/09_crop.png)

**Undo last change**
![After undo](screenshots/10_restored.png)

**Increase Sharpness of the image**
![Sharpen applied](screenshots/11_sharpen.png)

**Save image dialog**
![Save image dialog](screenshots/12_save_dialog.png)

**Error handling for invalid file**
![Invalid file error](screenshots/13_error_invalid_file.png)

## Known Limitations

- Save currently writes a full row of pixel data without re-adding BMP row padding for widths not divisible by 4 — works correctly for image widths where `width * 3` is already a multiple of 4.
- Crop does not currently validate that the requested rectangle fits inside the image bounds.

## Author

Shahariar Nafis — BSSE 1849
