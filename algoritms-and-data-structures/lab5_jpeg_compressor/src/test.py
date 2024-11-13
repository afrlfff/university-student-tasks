import os
import jpeg_compressor as jpeg

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))

input_path = os.path.join(CURRENT_DIR, "..", "input", "raw", "color.raw")
compressed_path = os.path.join(CURRENT_DIR, "..", "compressed", "color_compressed.bin")
output_path = os.path.join(CURRENT_DIR, "..", "output", "color_restored.jpg")

jpeg.compress(input_path, compressed_path)
jpeg.decompress(compressed_path, output_path)
