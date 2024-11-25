import subprocess
import os

def entropy_coder(input_path, output_path):
    """
    Perform entropy coding on a binary file using an external compressor executable.

    Args:
        input_path (str): The path to the binary file to be compressed.
        output_path (str): The path to the binary file to save the compressed file.

    Notes:
        This function uses an external compressor executable to perform entropy coding.
        The executable is assumed to be located in the "compressor" directory within the current directory.
    """

    CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))

    exe_path = os.path.join(CURRENT_DIR, "..","compressor", "compressor.exe")
    subprocess.run([exe_path] + [input_path, output_path])
    

def entropy_decoder(input_path, output_path):
    """
    Perform entropy decoding on a binary file using an external decompressor executable.

    Args:
        input_path (str): The path to the binary file to be decompressed.
        output_path (str): The path to the binary file to save the decompressed file.

    Notes:
        This function uses an external decompressor executable to perform entropy decoding.
        The executable is assumed to be located in the "compressor" directory within the current directory.
    """

    CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
    exe_path = os.path.join(CURRENT_DIR, "..", "compressor", "decompressor.exe")
    subprocess.run([exe_path] + [input_path, output_path])