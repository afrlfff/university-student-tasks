import os
import jpeg_compressor as jpeg

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))

#input_path = os.path.join(CURRENT_DIR, "..", "input", "raw", "color.raw")
#compressed_path = os.path.join(CURRENT_DIR, "..", "compressed", "color_compressed.bin")
#output_path = os.path.join(CURRENT_DIR, "..", "output", "color_restored.jpg")

#jpeg.compress(input_path, compressed_path)
#jpeg.decompress(compressed_path, output_path)


IMGS = ["color", "normal", "yellow"]
Q = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]

for img in IMGS:
    for q in Q:
        input_path = os.path.join(CURRENT_DIR, "..", "input", "raw", f"{img}.raw")
        compressed_path = os.path.join(CURRENT_DIR, "..", "compressed", f"{img}_compressed_Q{q}.bin")
        output_path = os.path.join(CURRENT_DIR, "..", "output", f"{img}_restored_Q{q}.jpg")
        jpeg.compress(input_path, compressed_path, quality=q)
        jpeg.decompress(compressed_path, output_path, quality=q)
        print(f"{img}.raw | Q={q} - done.")
