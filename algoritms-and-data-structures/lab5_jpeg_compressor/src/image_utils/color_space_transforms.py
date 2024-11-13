import numpy as np

def rgb_to_ycbcr(R, G, B):
    Y = (0.299 * R + 0.587 * G + 0.114 * B)
    Cb = (-0.168736 * R - 0.331264 * G + 0.5 * B + 128)
    Cr = (0.5 * R - 0.418688 * G - 0.081312 * B + 128)

    return Y.astype(np.uint8), Cb.astype(np.uint8), Cr.astype(np.uint8)

def ycbcr_to_rgb(Y, Cb, Cr):
    # to avoid overflows
    Y = Y.astype(np.float16)
    Cb = Cb.astype(np.float16)
    Cr = Cr.astype(np.float16)

    R = Y + 1.402 * (Cr - 128)
    G = (Y - 0.344136 * (Cb - 128) - 0.714136 * (Cr - 128))
    B = (Y + 1.772 * (Cb - 128))

    # to avoid negative numbers and overflows
    R = np.clip(R, 0, 255)
    G = np.clip(G, 0, 255)
    B = np.clip(B, 0, 255)

    return R.astype(np.uint8), G.astype(np.uint8), B.astype(np.uint8)