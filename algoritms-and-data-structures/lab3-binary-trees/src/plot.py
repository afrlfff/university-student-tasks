import matplotlib.pyplot as plt
import numpy as np
import os

# Путь к папке с файлами
PATHS = [
    [
         "bst_heights.txt",
         "avl_heights.txt",
         "rbt_heights.txt"
    ],
    [
        "avl_insertion_times.txt",
        "rbt_insertion_times.txt"
    ],
    [
        "avl_removal_times.txt",
        "rbt_removal_times.txt"
    ]
]
FOLDERS = {
    "RandomKeys",
    "SortedKeys",
}

START_BOLD = r"$\bf{"
END_BOLD = "}$"
LABELS = ["BST", "AVL", "RBT"]
CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
COLORS = ['blue', 'green', 'red']
SHOW_GRPH = False

INTERPOLATION_COEFF_HEIGHT = [12, 1.335, 1.15, 0, 1.03, 1.82]


for folder in FOLDERS:
    
    # TREE HEIGHTS
    ind = 0
    for file_path in PATHS[0]:
        current_path = os.path.join(CURRENT_DIR, "..\\results\\" + folder + "\\" + file_path)
        file = open(current_path, 'r')

        SIZES = []
        TIMES = []
        for line in file.readlines():
            s_size, s_time = line.split()
            SIZES.append(int(s_size))
            TIMES.append(float(s_time))

        # START INTERPOLATION
        if (ind % 3 == 0) and (INTERPOLATION_COEFF_HEIGHT[ind] > 0):
            if folder == "RandomKeys":
                y_interpolation = np.log2(np.array([(float(i) ** INTERPOLATION_COEFF_HEIGHT[ind]) for i in SIZES]))
                plt.plot(SIZES, y_interpolation, color='black')
            if folder == "SortedKeys":
                y_interpolation = np.log2(np.array([(float(i) ** INTERPOLATION_COEFF_HEIGHT[ind - 3]) for i in SIZES]))
                plt.plot(SIZES, y_interpolation, color='black')
        # END INTERPOLATION

        plt.plot(SIZES, TIMES, label=LABELS[ind], color=COLORS[ind])
        ind += 1

    plt.title(START_BOLD + "Tree" + END_BOLD + " " +
              START_BOLD + "heights" + END_BOLD + " " +
              START_BOLD + "on" + END_BOLD + " " +
              START_BOLD + folder[:-4] + END_BOLD + " " + 
              START_BOLD + folder[-4:] + "." + END_BOLD)
    plt.xlabel("Count of elements")
    plt.ylabel("Height")
    if folder == "SortedKeys":
        plt.ylim(0, 250000)
    else:
        plt.ylim(0, 215)
    plt.legend()
    if SHOW_GRPH:
        plt.show()
    else:
        current_path = os.path.join(CURRENT_DIR, "..\\png\\" + folder + "\\" + "tree_heights.png")
        plt.savefig(current_path)
        plt.clf()

    # TREE HEIGHTS ZOOMED

    ind = 0
    for file_path in PATHS[0]:
        current_path = os.path.join(CURRENT_DIR, "..\\results\\" + folder + "\\" + file_path)
        file = open(current_path, 'r')

        SIZES = []
        TIMES = []
        for line in file.readlines():
            s_size, s_time = line.split()
            SIZES.append(int(s_size))
            TIMES.append(float(s_time))

        # START INTERPOLATION
        if (ind % 3 != 0) and (INTERPOLATION_COEFF_HEIGHT[ind] > 0):
            if folder == "RandomKeys":
                y_interpolation = np.log2(np.array([(float(i) ** INTERPOLATION_COEFF_HEIGHT[ind]) for i in SIZES]))
                plt.plot(SIZES, y_interpolation, color='black')
            if folder == "SortedKeys":
                y_interpolation = np.log2(np.array([(float(i) ** INTERPOLATION_COEFF_HEIGHT[ind - 3]) for i in SIZES]))
                plt.plot(SIZES, y_interpolation, color='black')
        # END INTERPOLATION

        plt.plot(SIZES, TIMES, label=LABELS[ind], color=COLORS[ind])
        ind += 1

    plt.title( START_BOLD + "Tree" + END_BOLD + " " +
              START_BOLD + "heights" + END_BOLD + " " +
              START_BOLD + "zoomed" + END_BOLD + " " +
              START_BOLD + "on" + END_BOLD + " " +
              START_BOLD + folder[:-4] + END_BOLD + " " + 
              START_BOLD + folder[-4:] + "." + END_BOLD)
    plt.xlabel("Count of elements")
    plt.ylabel("Height")
    plt.ylim(0, 40)
    plt.legend()
    if SHOW_GRPH:
        plt.show()
    else:
        current_path = os.path.join(CURRENT_DIR, "..\\png\\" + folder + "\\" + "tree_heights_zoomed.png")
        plt.savefig(current_path)
        plt.clf()
   

    
    # INSERTION TIMES

    ind = 1
    for file_path in PATHS[1]:
        current_path = os.path.join(CURRENT_DIR, "..\\results\\" + folder + "\\" + file_path)
        file = open(current_path, 'r')

        SIZES = []
        TIMES = []
        for line in file.readlines():
            s_size, s_time = line.split()
            SIZES.append(int(s_size))
            TIMES.append(float(s_time))

        # Полиномиальная интерполяция
        coefficients = np.polyfit(SIZES, TIMES, deg=3)  # Меняйте deg на нужную степень полинома

        # Создание массива значений X для построения графика
        X = np.linspace(min(SIZES), max(SIZES), 100)

        # Вычисление значений Y с использованием коэффициентов полинома
        Y = np.polyval(coefficients, X)

        plt.plot(SIZES, TIMES, label=LABELS[ind], color=COLORS[ind])
        plt.plot(X, Y, color='black')
        ind += 1

    plt.title( START_BOLD + "Tree" + END_BOLD + " " +
            START_BOLD + "Insertion" + END_BOLD + " " + 
            START_BOLD + "times" + END_BOLD + " " + 
            START_BOLD + "on" + END_BOLD + " " +
            START_BOLD + folder[:-4] + END_BOLD + " " + 
            START_BOLD + folder[-4:] + "." + END_BOLD)
    plt.xlabel("Count of elements")
    plt.ylabel("Time [seconds]")
    plt.legend()
    if SHOW_GRPH:
        plt.show()
    else:
        current_path = os.path.join(CURRENT_DIR, "..\\png\\" + folder + "\\" + "tree_insertions.png")
        plt.savefig(current_path)
        plt.clf()

    # REMOVAL TIMES

    ind = 1
    for file_path in PATHS[2]:
        current_path = os.path.join(CURRENT_DIR, "..\\results\\" + folder + "\\" + file_path)
        file = open(current_path, 'r')

        SIZES = []
        TIMES = []
        for line in file.readlines():
            s_size, s_time = line.split()
            SIZES.append(int(s_size))
            TIMES.append(float(s_time))
        
        # Полиномиальная интерполяция
        coefficients = np.polyfit(SIZES, TIMES, deg=3)  # Меняйте deg на нужную степень полинома

        # Создание массива значений X для построения графика
        X = np.linspace(min(SIZES), max(SIZES), 100)

        # Вычисление значений Y с использованием коэффициентов полинома
        Y = np.polyval(coefficients, X)

        plt.plot(SIZES, TIMES, label=LABELS[ind], color=COLORS[ind])
        plt.plot(X, Y, color='black')
        ind += 1

    plt.title( START_BOLD + "Tree" + END_BOLD + " " +
            START_BOLD + "Removal" + END_BOLD + " " + 
            START_BOLD + "times " + END_BOLD + " " +
            START_BOLD + "on" + END_BOLD + " " +
            START_BOLD + folder[:-4] + END_BOLD + " " + 
            START_BOLD + folder[-4:] + "." + END_BOLD)
    plt.xlabel("Count of elements")
    plt.ylabel("Time [seconds]")
    plt.legend()
    if SHOW_GRPH:
        plt.show()
    else:
        current_path = os.path.join(CURRENT_DIR, "..\\png\\" + folder + "\\" + "tree_removals.png")
        plt.savefig(current_path)
        plt.clf()
   