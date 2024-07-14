#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <fstream>
#include <filesystem>

#include "include/CallSort.h"
#include "include/Split.h"

// OPTIONS

// max size of array
const int ARRAY_SIZE = 1000;
// index from which to start check RESULT_PATHS (from 0 to 10)
const int RESULT_PATHS_START_INDEX = 4;
// index to which to end check RESULT_PATHS (from 0 to 10)
const int RESULT_PATHS_END_INDEX = 4;
// for example, if both indexes are 4, it means that only QuickSort will be checked

// CONSTANTS

// difference between each size to sort
constexpr int SIZE_STEP = ARRAY_SIZE / 50;
// coefficient to generate all int numbers by rand() function 
constexpr int RAND_COEFFICIENT = 2147483648 / RAND_MAX;
// paths to result files
const std::string RESULT_PATHS[4][11] = {
	{
		"../results/SortedArray/SelectionSort.txt",
		"../results/SortedArray/InsertionSort.txt",
		"../results/SortedArray/BubbleSort.txt",
		"../results/SortedArray/MergeSort.txt",
		"../results/SortedArray/QuickSort.txt",
		"../results/SortedArray/ShellSort1.txt",
		"../results/SortedArray/ShellSort2.txt",
		"../results/SortedArray/ShellSort3.txt",
		"../results/SortedArray/HeapSort.txt",
		"../results/SortedArray/TimSort.txt",
		"../results/SortedArray/IntroSort.txt"
	},
	{
		"../results/BackSortedArray/SelectionSort.txt",
		"../results/BackSortedArray/InsertionSort.txt",
		"../results/BackSortedArray/BubbleSort.txt",
		"../results/BackSortedArray/MergeSort.txt",
		"../results/BackSortedArray/QuickSort.txt",
		"../results/BackSortedArray/ShellSort1.txt",
		"../results/BackSortedArray/ShellSort2.txt",
		"../results/BackSortedArray/ShellSort3.txt",
		"../results/BackSortedArray/HeapSort.txt",
		"../results/BackSortedArray/TimSort.txt",
		"../results/BackSortedArray/IntroSort.txt"
	},
	{
		"../results/AlmostSortedArray/SelectionSort.txt",
		"../results/AlmostSortedArray/InsertionSort.txt",
		"../results/AlmostSortedArray/BubbleSort.txt",
		"../results/AlmostSortedArray/MergeSort.txt",
		"../results/AlmostSortedArray/QuickSort.txt",
		"../results/AlmostSortedArray/ShellSort1.txt",
		"../results/AlmostSortedArray/ShellSort2.txt",
		"../results/AlmostSortedArray/ShellSort3.txt",
		"../results/AlmostSortedArray/HeapSort.txt",
		"../results/AlmostSortedArray/TimSort.txt",
		"../results/AlmostSortedArray/IntroSort.txt"
	},
	{
		"../results/RandomArray/SelectionSort.txt",
		"../results/RandomArray/InsertionSort.txt",
		"../results/RandomArray/BubbleSort.txt",
		"../results/RandomArray/MergeSort.txt",
		"../results/RandomArray/QuickSort.txt",
		"../results/RandomArray/ShellSort1.txt",
		"../results/RandomArray/ShellSort2.txt",
		"../results/RandomArray/ShellSort3.txt",
		"../results/RandomArray/HeapSort.txt",
		"../results/RandomArray/TimSort.txt",
		"../results/RandomArray/IntroSort.txt"
	}
};

int main()
{
    if (!std::filesystem::exists("../results")) {
        std::filesystem::create_directory("../results");
    }
    if(!std::filesystem::exists("../results/SortedArray")) {
        std::filesystem::create_directory("../results/SortedArray");
    }
    if (!std::filesystem::exists("../results/BackSortedArray")) {
        std::filesystem::create_directory("../results/BackSortedArray");
    }
    if (!std::filesystem::exists("../results/AlmostSortedArray")) {
        std::filesystem::create_directory("../results/AlmostSortedArray");
    }
    if (!std::filesystem::exists("../results/RandomArray")) {
        std::filesystem::create_directory("../results/RandomArray");
    }

	int startNumber;
	srand(time(0));

	// ========== Making Sorted Array ========== //

	startNumber = 5;
	std::vector<int> SortedArray; SortedArray.reserve(ARRAY_SIZE);
    SortedArray.push_back(startNumber);
	for (int i = 0; i < ARRAY_SIZE - 1; ++i) {
		SortedArray.push_back(SortedArray[i] + ((rand() * RAND_COEFFICIENT) % 100));
    }

	// ========== Making BackSorted Array ========== //

	startNumber = 100005;
	std::vector<int> BackSortedArray; BackSortedArray.reserve(ARRAY_SIZE);
    BackSortedArray.push_back(startNumber);
	for (int i = 0; i < ARRAY_SIZE - 1; ++i) {
        BackSortedArray.push_back(BackSortedArray[i] - ((rand() * RAND_COEFFICIENT) % 100));
    }

	// ========== Making Almost Sorted Array ========== //

	// step between elements which will be backsorted in already sorted array
	int stepTmp = ARRAY_SIZE / (ARRAY_SIZE * 0.05); // I made it to be 5%

	std::vector<int> AlmostSortedArray = SortedArray;
	for (int i = 0; i < ARRAY_SIZE; i += stepTmp) {
		std::swap(AlmostSortedArray[i], AlmostSortedArray[i + 1]);
    }

	// ========== Making Random Array ========== //

	std::vector<int> RandomArray; RandomArray.reserve(ARRAY_SIZE);
	for (int i = 0; i < ARRAY_SIZE; ++i) {
		RandomArray.push_back((rand() * RAND_COEFFICIENT) % 10000);
    }

	// ========== Making AllArrays ========== //

	std::vector<std::vector<int>*> AllArrays {
		&SortedArray,
		&BackSortedArray,
		&AlmostSortedArray,
		&RandomArray
	};

	// ========== Time measuring ========== //
	// ==========
    std::cout << "Start measuring..." << std::endl;

	std::vector<int>* CurrentArray;
	std::vector<int> CurrentArrayCopy;
	int repeat;
	double minTime;
    std::chrono::steady_clock::time_point startTime, endTime;
    std::chrono::duration<double> elapsed_seconds;
    std::ofstream file;

    int i, j, k, l;

	for (i = RESULT_PATHS_START_INDEX; i < (RESULT_PATHS_END_INDEX + 1); ++i)
	{
        std::cout << "Start measuring " << Split(Split(RESULT_PATHS[0][i], '/')[3], '.')[0] << std::endl;
		for(j = 0; j < AllArrays.size(); ++j)
		{
			CurrentArray = AllArrays[j];
			file.open(RESULT_PATHS[j][i]);

			for (k = 1; k <= ARRAY_SIZE; k += (SIZE_STEP - (k == 1)))
			{
				if (k < 10000)
					repeat = 100;
				else if (k >= 10000 && k < 20000)
					repeat = 50;
				else if (k >= 20000 && k < 35000)
					repeat = 25;
				else
					repeat = 25;

				minTime = 100000;
				for (l = 0; l < repeat; ++l)
				{
					copy(CurrentArray->begin(), CurrentArray->begin() + k, back_inserter(CurrentArrayCopy));
					startTime = std::chrono::steady_clock::now();
					CallSort(CurrentArrayCopy, i+1);
					endTime = std::chrono::steady_clock::now();
					elapsed_seconds = endTime - startTime;

					if (elapsed_seconds.count() < minTime)
						minTime = elapsed_seconds.count();

					CurrentArrayCopy.clear();
				}

				file << std::fixed << std::setprecision(8) << minTime << std::endl;
			}
			file.close();

            if (j == 0) {
                std::cout << "\tDone with SortedArray" << std::endl;
            } else if (j == 1) {
                std::cout << "\tDone with BackSortedArray" << std::endl;
            } else if (j == 2) {
                std::cout << "\tDone with AlmostSortedArray" << std::endl;
            } else if (j == 3) {
                std::cout << "\tDone with RandomArray" << std::endl;
            }
		}
        std::cout << "Done with " << Split(Split(RESULT_PATHS[0][i], '/')[3], '.')[0] << std::endl << std::endl;
	}
}
