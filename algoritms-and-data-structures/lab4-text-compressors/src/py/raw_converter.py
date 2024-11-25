"""
This module contains functions for image processing.
It can convert JPEG images to RAW files and restore JPEG images from RAW files.

Then we will apply codecs on raw files to make their size smaller.
"""

# Libraries
from PIL import Image
import os
import numpy as np


# Constants
mode2ind = {
    "L": 0,
    "RGB": 1
}
ind2mode = {value: key for key, value in mode2ind.items()}
PATH_TO_CURRENT_FILE = os.path.dirname(os.path.abspath(__file__))
JPEG_FOLDER = os.path.join(PATH_TO_CURRENT_FILE, "..", "input", "jpg")
RAW_FOLDER = os.path.join(PATH_TO_CURRENT_FILE, "..", "input", "raw")


# Functions
def jpg_to_raw(path_jpg, path_raw):
    image = Image.open(path_jpg)

    width, height = image.size
    mode = image.mode
    pixels = image.getdata()

    with open(path_raw, "wb") as f:
        # write header
        f.write(width.to_bytes(2, byteorder='little'))
        f.write(height.to_bytes(2, byteorder='little'))
        f.write(mode2ind[mode].to_bytes(1, byteorder='little'))

        # write pixels
        if mode == "L":
            for pixel in pixels:
                f.write(pixel.to_bytes(1, byteorder='little'))
        elif mode == "RGB":
            for pixel in pixels:
                f.write(pixel[0].to_bytes(1, byteorder='little'))
                f.write(pixel[1].to_bytes(1, byteorder='little'))
                f.write(pixel[2].to_bytes(1, byteorder='little'))
        else:
            raise ValueError("Unknown image mode.")

def raw_to_jpg(path_raw, path_jpg):
    with open(path_raw, "rb") as f:
        width = int.from_bytes(f.read(2), byteorder='little')
        height = int.from_bytes(f.read(2), byteorder='little')
        mode = ind2mode[int.from_bytes(f.read(1), byteorder='little')]

        pixels = []
        if mode == "L":
            for _ in range(height):
                row_pixels = []
                for __ in range(width):
                    row_pixels.append(int.from_bytes(f.read(1), byteorder='little'))
                pixels.append(row_pixels)
        elif mode == "RGB":
            for _ in range(height):
                row_pixels = []
                for __ in range(width):
                    row_pixels.append([int.from_bytes(f.read(1), byteorder='little'),
                                      int.from_bytes(f.read(1), byteorder='little'),
                                      int.from_bytes(f.read(1), byteorder='little')])
                pixels.append(row_pixels)
        else:
            raise ValueError("Unknown image mode.")

        img = Image.fromarray(np.array(pixels).astype(np.uint8), mode)
        img.save(path_jpg)



# Code to convert all JPEG to RAW
#   |          |            |
#   V          V            V

""" for filename in os.listdir(JPEG_FOLDER):
    if filename.endswith(".jpg"):
        input_path = os.path.join(JPEG_FOLDER, filename)
        output_path = os.path.join(RAW_FOLDER, filename.replace(".jpg", ".raw"))
        jpg_to_raw(input_path, output_path) """


# Code to convert all RAW back to JPEG
#   |          |            |
#   V          V            V

""" for filename in os.listdir(RAW_FOLDER):
    if filename.endswith(".raw"):
        input_path = os.path.join(RAW_FOLDER, filename)
        output_path = os.path.join(JPEG_FOLDER, filename.replace(".raw", ".jpg"))
        raw_to_jpg(input_path, output_path) """