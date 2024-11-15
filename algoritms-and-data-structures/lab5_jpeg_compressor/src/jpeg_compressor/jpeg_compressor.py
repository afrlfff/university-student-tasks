import os
from jpeg_compressor.image_preprocessor import preprocess, restore
from jpeg_compressor.entropy_compressor import entropy_coder, entropy_decoder


def compress(input_path, output_path, quality=50):
    """
    Perform JPEG compression on a raw image file and save the compressed image as a binary file.

    Args:
        input_path (str): The path to the raw image file.
        output_path (str): The path to save the compressed image as a binary file.
    """
    path_to_temp = os.path.join(os.path.dirname(output_path), "temp.bin")

    preprocess(input_path, path_to_temp, Q=quality)
    entropy_coder(path_to_temp, output_path)

    os.remove(path_to_temp)

def decompress(input_path, output_path, quality=50):
    """
    Perform JPEG decompression on a compressed binary file and save the decompressed image as a JPEG file.

    Args:
        input_path (str): The path to the compressed binary file.
        output_path (str): The path to save the decompressed image as a JPEG file.
    """
    path_to_temp = os.path.join(os.path.dirname(output_path), "temp.bin")

    entropy_decoder(input_path, path_to_temp)
    restore(path_to_temp, output_path, Q=quality)

    os.remove(path_to_temp)

# import os
# compress
""" input_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "input", "raw", "color.raw")
output_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "intermediate", "colored_to_compress.bin")
compress(input_path, output_path) """

# decompress
""" input_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "intermediate", "colored_decompressed.bin")
output_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "output_img", "colored_restored.jpg")
decompress(input_path, output_path) """