import numpy as np

def zero_padding8(img):
    height, width = img.shape

    new_width = width
    new_height = height
    while new_width % 8 != 0:
        new_width += 1
    while new_height % 8 != 0:
        new_height += 1

    new_img = np.zeros((new_height, new_width), dtype=img.dtype)
    new_img[0:height, 0:width] = img
    for h in range(height, new_height):
        for w in range(width, new_width):
            new_img[h, w] = 0

    return new_img