import numpy as np
from scipy.fft import dctn as DCT
from scipy.fft import idctn as IDCT


def dct(img_block):
    return DCT(img_block, norm='ortho').astype(np.float32)

def idct(dct_block):
    return IDCT(dct_block, norm='ortho')

""" def dct(img_block):
    if (img_block.shape[0] != img_block.shape[1]) or (img_block.shape[0] != 8):
        raise Exception("DCT block must be 8x8")
    
    def coeff(value):
        if(value == 0): return 1 / np.sqrt(2)
        else: return 1

    dct_block = np.zeros((8, 8), dtype=np.float32)  
    
    for i in range(8):
        for j in range(8):       
            dct_block[j][i]= 1/4 * coeff(i) * coeff(j) * np.sum( [ [img_block[y][x] * np.cos((2*x+1)*i*np.pi/16) * np.cos((2*y+1)*j*np.pi/16) for y in range(8)] for x in range(8) ])

    return dct_block
    

def idct(dct_block):
    if (dct_block.shape[0] != dct_block.shape[1]) or (dct_block.shape[0] != 8):
        raise Exception("IDCT block must be 8x8")

    def coeff(value):
        if(value == 0): return 1 / np.sqrt(2)
        else: return 1

    img_block = np.zeros((8, 8), dtype=np.float32)
    
    for x in range(8):
        for y in range(8):       
            img_block[y][x]=1/4 * np.sum( [ [coeff(j) * coeff(i) * dct_block[j][i] * np.cos((2*x+1)*i*np.pi/16) * np.cos((2*y+1)*j*np.pi/16) for j in range(8) ] for i in range(8) ] )

    return img_block """
    