import numpy as np

def _upsample_by_duplication(img, kx, ky):
    return np.repeat(np.repeat(img, ky, axis=0), kx, axis=1)

def upsample(img, kx, ky, key):
    if key == 'duplication':
        return _upsample_by_duplication(img, kx, ky)
    
__all__ = ['upsample']