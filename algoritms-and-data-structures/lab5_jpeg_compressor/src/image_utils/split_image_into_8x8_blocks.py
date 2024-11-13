import numpy as np

def split_into_8x8(img):
    height, width = img.shape
    blocks = []

    for y in range(0, height, 8):
        for x in range(0, width, 8):
            block = np.array(img[y:y+8, x:x+8], dtype=img.dtype)
            blocks.append(block)
    return np.array(blocks)

def merge_blocks(blocks, width, height):
    pixels = np.zeros((height, width), dtype=np.uint8)
    
    block_size = blocks[0].shape[0]
    h = w = 0
    for block in blocks:
        pixels[h:h+block_size, w:w+block_size] = block
        if (w + block_size >= width):
            h += block_size
            w = 0
        else:
            w += block_size
    
    return pixels