from PIL import Image
import numpy as np

def generate_image_in_normal_distribution(width, height, path=""):
    mean = 128
    std = 128

    rgb_values = np.random.normal(mean, std, size=(height, width, 3))
    rgb_values = np.clip(rgb_values, 0, 255)
    rgb_values = rgb_values.astype(np.uint8)

    img = Image.fromarray(rgb_values)

    if path != "":
        img.save(path)
    
    return img