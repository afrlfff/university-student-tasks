import numpy as np

def quantize(dct_matrix, quantization_matrix):
    return np.round(dct_matrix / quantization_matrix)

def dequantize(quantized_matrix, quantization_matrix):
    return (quantized_matrix * quantization_matrix)