import os
import matplotlib.pyplot as plt

from delete_graphics import delete_graphics

# ========================================= #
# =============== CONSTANTS =============== #
# ========================================= #

sortTypes = [
    "SortedArray",
    "AlmostSortedArray",
    "BackSortedArray",
    "RandomArray"
]
ARRAY_SIZE = 50000
STEP = int(ARRAY_SIZE / 50)
START_BOLD = r"$\bf{"
END_BOLD = "}$"

# [1, 500, 1000, ..., 50000]
SIZES = [1] + [i for i in range(STEP, ARRAY_SIZE + 1, STEP)]



# =============================================== #
# =============== HELP FUNCTIONS ================ #
# =============================================== #

# makes string equation like "y = 3.5352535 * 10^323 x"
def make_string_equation (k, str_x, accuracy):    
    k1 = k # float number from 1 to 9
    k2 = 0 # power of 10
    while(k1 < 1):
        k1 *= 10
        k2 -= 1

    k2_str = '' # string helps to output k2 above 10-number
    for simb in str(k2):
        k2_str += "$^" + simb + "$"

    result = "y = " + str(f"%.{accuracy}f" % k1) + \
        " * 10" + k2_str + str_x
    return result

# ========================================= #
# =============== PLOTTING ================ #
# ========================================= #

delete_graphics()

# Here you should delete the code below
# And paste code for each Sorting function

