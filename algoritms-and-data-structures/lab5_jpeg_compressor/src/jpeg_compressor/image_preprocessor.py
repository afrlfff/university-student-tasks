from PIL import Image
import numpy as np
import image_utils as iu

def preprocess(input_path, output_path, Q):
    """
    Perform JPEG preprocessing on a raw image file to prepare it for entropy coding.

    Args:
        input_path (str): The path to the raw image file.
        output_path (str): The path to save the preprocessed image as a temporary binary file.

    Notes:
        This function performs the necessary steps to prepare the raw image file for entropy coding
        The output is a temporary binary file that can be used as input for the entropy coder.
    """

    img = iu.raw_to_img(input_path)
    width, height = img.size

    # read pixels
    pixels = np.array(img.getdata())
    pixels = pixels.reshape(height, width, 3)

    # tranform to YCbCr
    y, cb, cr = iu.rgb_to_ycbcr(pixels[:, :, 0], pixels[:, :, 1], pixels[:, :, 2])

    # downsampling
    cb = iu.downsample(cb, 2, 2, 'average')
    cr = iu.downsample(cr, 2, 2, 'average')

    # pad the image before splitting to blocks
    if (y.shape[1] % 8 != 0) or (y.shape[0] % 8 != 0):
        y = iu.zero_padding8(y)
    if (cb.shape[1] % 8 != 0) or (cb.shape[0] % 8 != 0):
        cb = iu.zero_padding8(cb)
        cr = iu.zero_padding8(cr)

    # split into 8x8 blocks
    y_blocks = iu.split_into_8x8(y)
    cb_blocks = iu.split_into_8x8(cb)
    cr_blocks = iu.split_into_8x8(cr)

    # convert values to int8 before DCT
    normalize_uint8_to_int8 = lambda value: np.int8(value - 128)
    y_blocks = np.apply_along_axis(normalize_uint8_to_int8, 1, y_blocks)
    cb_blocks = np.apply_along_axis(normalize_uint8_to_int8, 1, cb_blocks)
    cr_blocks = np.apply_along_axis(normalize_uint8_to_int8, 1, cr_blocks)

    # apply DCT to each block
    y_blocks = np.array([iu.dct(block) for block in y_blocks])
    cb_blocks = np.array([iu.dct(block) for block in cb_blocks])
    cr_blocks = np.array([iu.dct(block) for block in cr_blocks])

    # quantization
    quantization_matrix = iu.calculate_quantization_matrix(Q)
    y_blocks = np.array([iu.quantize(block, quantization_matrix) for block in y_blocks]).astype(np.int8)
    cb_blocks = np.array([iu.quantize(block, quantization_matrix) for block in cb_blocks]).astype(np.int8)
    cr_blocks = np.array([iu.quantize(block, quantization_matrix) for block in cr_blocks]).astype(np.int8)

    # zigzag traversal
    y_zigzags = np.array([iu.zigzag_traversal(block) for block in y_blocks])
    cb_zigzags = np.array([iu.zigzag_traversal(block) for block in cb_blocks])
    cr_zigzags = np.array([iu.zigzag_traversal(block) for block in cr_blocks])

    # write to file 
    with open(output_path, 'wb') as f:
        f.write(width.to_bytes(2, byteorder='little', signed=False))
        f.write(height.to_bytes(2, byteorder='little', signed=False))

        f.write(len(y_zigzags).to_bytes(4, byteorder='little', signed=False)) # TODO: can be calculated from width and height 
        f.write(len(cb_zigzags).to_bytes(4, byteorder='little', signed=False)) # TODO: can be calculated from width and height
        for y_zigzag in y_zigzags:
            for coeff in y_zigzag:
                f.write(coeff.tobytes()) # 1 byte for np.int8 value
        for cb_zigzag in cb_zigzags:
            for coeff in cb_zigzag:
                f.write(coeff.tobytes())
        for cr_zigzag in cr_zigzags:
            for coeff in cr_zigzag:
                f.write(coeff.tobytes())


def restore(input_path, output_path, Q):
    """
    Perform JPEG postprocessing on a binary file obtained from entropy decoding to recover the original jpeg image.

    Args:
        input_path (str): The path to the binary file obtained from entropy decoding.
        output_path (str): The path to save the recovered raw image.

    Notes:
        This function performs the necessary steps to recover the original raw image from the binary file obtained from entropy decoding.
        The output is a raw image file that can be used for further processing or display.
    """

    # read from file
    with open(input_path, 'rb') as f:
        width = int.from_bytes(f.read(2), byteorder='little')
        height = int.from_bytes(f.read(2), byteorder='little')

        num_y_zigzags = int.from_bytes(f.read(4), byteorder='little')
        num_cb_zigzags = int.from_bytes(f.read(4), byteorder='little')

        y_zigzags = []
        for _ in range(num_y_zigzags):
            temp = []
            for __ in range(64):
                temp.append(int.from_bytes(f.read(1), byteorder='little', signed=True))
            y_zigzags.append(temp)
        cb_zigzags = []
        for _ in range(num_cb_zigzags):
            temp = []
            for __ in range(64):
                temp.append(int.from_bytes(f.read(1), byteorder='little', signed=True))
            cb_zigzags.append(temp)
        cr_zigzags = []
        for _ in range(num_cb_zigzags):
            temp = []
            for __ in range(64):
                temp.append(int.from_bytes(f.read(1), byteorder='little', signed=True))
            cr_zigzags.append(temp)
    y_zigzags = np.array(y_zigzags, dtype=np.int8)
    cb_zigzags = np.array(cb_zigzags, dtype=np.int8)
    cr_zigzags = np.array(cr_zigzags, dtype=np.int8)

    # inverse zigzag traversal
    y_blocks = np.array([iu.zigzag_traversal_inverse(traversal) for traversal in y_zigzags]).astype(np.int8)
    cb_blocks = np.array([iu.zigzag_traversal_inverse(traversal) for traversal in cb_zigzags]).astype(np.int8)
    cr_blocks = np.array([iu.zigzag_traversal_inverse(traversal) for traversal in cr_zigzags]).astype(np.int8)

    # dequantization
    quantization_matrix = iu.calculate_quantization_matrix(Q)
    y_blocks = np.array([iu.dequantize(block, quantization_matrix) for block in y_blocks])
    cb_blocks = np.array([iu.dequantize(block, quantization_matrix) for block in cb_blocks])
    cr_blocks = np.array([iu.dequantize(block, quantization_matrix) for block in cr_blocks])

    # inverse DCT
    y_blocks = np.array([iu.idct(block) for block in y_blocks])
    cb_blocks = np.array([iu.idct(block) for block in cb_blocks])
    cr_blocks = np.array([iu.idct(block) for block in cr_blocks])

    # convert values back to uint8
    normalize_int8_to_uint8 = lambda value: np.uint8(value + 128)
    y_blocks = np.apply_along_axis(normalize_int8_to_uint8, 1, y_blocks)
    cb_blocks = np.apply_along_axis(normalize_int8_to_uint8, 1, cb_blocks)
    cr_blocks = np.apply_along_axis(normalize_int8_to_uint8, 1, cr_blocks)

    # merge 8x8 blocks back to image
    y = iu.merge_blocks(y_blocks, width, height)
    cb = iu.merge_blocks(cb_blocks, width // 2, height // 2)
    cr = iu.merge_blocks(cr_blocks, width // 2, height // 2)

    # remove padding if it has been done
    if y.shape[1] > width:
        y = y[:, :width]
    if y.shape[0] > height:
        y = y[:height, :]
    if cb.shape[1] > width:
        cb = cb[:, :width]
        cr = cr[:, :width]
    if cb.shape[0] > height:
        cb = cb[:height, :]
        cr = cr[:height, :]

    # upsampling
    cb = iu.upsample(cb, 2, 2, 'duplication')
    cr = iu.upsample(cr, 2, 2, 'duplication')

    # transform to RGB
    r, g, b = iu.ycbcr_to_rgb(y, cb, cr)

    # save image
    new_img = np.dstack((r, g, b))
    Image.fromarray(new_img).save(output_path)


# import os
# compress
""" input_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "input", "raw", "color.raw")
output_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "intermediate", "colored_to_compress.bin")
compress(input_path, output_path) """

# decompress
""" input_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "intermediate", "colored_decompressed.bin")
output_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "output_img", "colored_restored.jpg")
decompress(input_path, output_path) """