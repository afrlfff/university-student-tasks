import numpy as np

def normalize_uint8_to_int8(value):
    return np.int8(value - 128)

def normalize_int8_to_uint8(value):
    return np.uint8(value + 128)