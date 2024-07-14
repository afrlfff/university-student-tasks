#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>

#include "include\BinarySearchTree.h"
#include "include\AVLTree.h"
#include "include\RedBlackTree.h"

#include "include\getTime.h"

#define KEY_TYPE int

// Sizes of input data
const int MIN_COUNT_OF_ELEMENTS =  1;
const int MAX_COUNT_OF_ELEMENTS = 250000;
// Maximum value of key
const KEY_TYPE MAX_VALUE_OF_KEY = 5000; // don't set <100

// Count of points in graph
const int COUNT_OF_POINTS = 50;
// difference between current sizes of input data
constexpr int STEP = (MAX_COUNT_OF_ELEMENTS - MIN_COUNT_OF_ELEMENTS + 1) / COUNT_OF_POINTS;

// coefficient to generate all int numbers by rand() function
constexpr int RAND_COEFFICIENT = 2147483647 / RAND_MAX;

const bool WRITE_TO_FILE = false;

const std::vector<std::string> FOLDERS = {
    "RandomKeys",
    "SortedKeys"
};

const std::vector<std::string> OUTPUT_FILES = {
    "bst_heights.txt",
    "avl_heights.txt",
    "rbt_heights.txt",
    "avl_insertion_times.txt",
    "rbt_insertion_times.txt",
    "avl_removal_times.txt",
    "rbt_removal_times.txt"
};

int main()
{
    BinarySearchTree<int> myBST;
    AVLTree<int> myAVL;
    RedBlackTree<int> myRBT;

    std::ofstream tempFile;
    if (WRITE_TO_FILE){
        for (std::string folder : FOLDERS)
        {
            for (std::string file : OUTPUT_FILES)
            {
                // clear files data
                tempFile.open("results\\" + folder + "\\" + file);
                tempFile.close();
            }
        }
    }

    KEY_TYPE currentKey = 0;
    double currentTime;
    std::string currentPath;

    srand(time(0)); // Initialize random seed for rand()

    for (std::string folder : FOLDERS)
    {
        myBST.clear();
        myAVL.clear();
        myRBT.clear();
        currentPath = "results\\" + folder + "\\";

        // Enumerate all sizes of Trees
        // 1, STEP, 2*STEP, ..., MAX_COUNT_OF_ELEMENTS
        for (int i = 1, i_prev = 0; i < MAX_COUNT_OF_ELEMENTS + 1; i_prev = i, i += (STEP - (i == 1)))
        {
            std::cout << "Current size: " << i << std::endl;

            if (folder == "RandomKeys") {
                // Insert random keys to the Tree
                for (int j = i_prev; j < i; j++) {
                    currentKey = (rand() * RAND_COEFFICIENT) % (MAX_VALUE_OF_KEY + 1);

                    myBST.insert(currentKey);
                    myAVL.insert(currentKey);
                    myRBT.insert(currentKey);
                }
            } else if (folder == "SortedKeys") {
                // Insert sorted keys to the Tree
                for (int j = i_prev; j < i; j++) {
                    // myBST.insert(currentKey);
                    myAVL.insert(currentKey);
                    myRBT.insert(currentKey);

                    currentKey += (rand() * RAND_COEFFICIENT) % (MAX_VALUE_OF_KEY / 100) + 1;
                }
            }

            // Output current heights

            if (folder == "RandomKeys") {
                std::cout << "\tCurrent heights: "
                << myBST.height() << ", " << myAVL.height() << ", " << myRBT.height()
                << std::endl;
            } else if (folder == "SortedKeys") {
                std::cout << "\tCurrent heights: "
                << i << ", " << myAVL.height() << ", " << myRBT.height()
                << std::endl;
            }
            
            if (WRITE_TO_FILE) {
                tempFile.open(currentPath + OUTPUT_FILES[0], std::ios::app);
                if (folder == "RandomKeys")
                    tempFile << i << " " << myBST.height() << std::endl;
                else if (folder == "SortedKeys")
                    tempFile << i << " " << i << std::endl;
                tempFile.close();
                tempFile.open(currentPath + OUTPUT_FILES[1], std::ios::app);
                tempFile << i << " " << myAVL.height() << std::endl;
                tempFile.close();
                tempFile.open(currentPath + OUTPUT_FILES[2], std::ios::app);
                tempFile << i << " " << myRBT.height() << std::endl;
                tempFile.close();
            }

            // Get random key to the Tree
            if (folder == "RandomKeys") {
                // + 1 to generate numbers from 0 to MAX_VALUE_OF_KEY
                currentKey = (rand() * RAND_COEFFICIENT) % (MAX_VALUE_OF_KEY + 1);
            } else if (folder == "SortedKeys") {
                currentKey += (rand() * RAND_COEFFICIENT) % (MAX_VALUE_OF_KEY / 100) + 1;
            }

            if (WRITE_TO_FILE) {
                // Output insertion and removal times
                    // AVL
                currentTime = getInsertionTime(myAVL, currentKey);
                tempFile.open(currentPath + OUTPUT_FILES[3], std::ios::app);
                tempFile << i << " " << std::fixed << currentTime << std::endl;
                tempFile.close();

                currentTime = getRemovalTime(myAVL, currentKey);
                tempFile.open(currentPath + OUTPUT_FILES[5], std::ios::app);
                tempFile << i << " " << std::fixed << currentTime << std::endl;
                tempFile.close();

                    // RBT
                currentTime = getInsertionTime(myRBT, currentKey);
                tempFile.open(currentPath + OUTPUT_FILES[4], std::ios::app);
                tempFile << i << " " << std::fixed << currentTime << std::endl;
                tempFile.close();

                currentTime = getRemovalTime(myRBT, currentKey);
                tempFile.open(currentPath + OUTPUT_FILES[6], std::ios::app);
                tempFile << i << " " << std::fixed << currentTime << std::endl;
                tempFile.close();
            }
        }   
    }

    return 0;
}