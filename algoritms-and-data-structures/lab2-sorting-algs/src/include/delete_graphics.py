import os

sortTypes = [
    "SortedArray",
    "BackSortedArray",
    "AlmostSortedArray",
    "RandomArray",
    "OnePictureForAllArrays"
]
filenames = [
    "IntroSort.png"
]

# data = folders = [folder, ..., folder] = [[file_data, ..., filedata], ...]

def delete_graphics():
    for filename in filenames:
        for sortType in sortTypes:
            current_dir = os.path.dirname(os.path.abspath(__file__))
            path = "..\\graphics\\" + sortType + "\\" + filename
            file_path = os.path.join(current_dir, path)

            os.remove(file_path)