import numpy as np

def _downsample_by_removing(img, kx, ky):
    return img[::ky, ::kx]

def _downsample_by_average(img, kx, ky):
    height, width = img.shape

    new_img = np.zeros((height // ky, width // kx), dtype=img.dtype)
    for y in range(0, height, ky):
        for x in range(0, width, kx):
            block = img[y : y + ky, x : x + kx]
            new_img[y // ky, x // kx] = np.mean(block)
    return new_img

def _downsample_by_median(img, kx, ky):
    height, width = img.shape

    new_img = np.zeros((height // ky, width // kx), dtype=img.dtype)
    for y in range(0, height, ky):
        for x in range(0, width, kx):
            block = img[y : y + ky, x : x + kx]
            new_img[ky * y : ky * y + ky, kx * x:kx * x + kx] = np.median(block)
    return new_img

def downsample(img, kx, ky, key):
    if key == 'removing':
        return _downsample_by_removing(img, kx, ky)
    if key == 'average':
        return _downsample_by_average(img, kx, ky)
    if key == 'median':
        return _downsample_by_median(img, kx, ky)

__all__ = ['downsample']