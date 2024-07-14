#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "include/CallSort.h"
#include "include/Split.h"

void PrintArray(const std::vector<int>& A);

int main()
{
	std::string input;
	std::vector<std::string> buffers;
	std::vector<int> Array;
	int number;

	while (true)
	{
		std::cout << "Enter elements of array separated by spaces: ";

		while (true)
		{
			getline(std::cin, input);
			buffers = Split(input, ' ');

			try
			{
				Array.clear();
				for (int i = 0; i < buffers.size(); ++i)
					Array.push_back(stoi(buffers[i]));
				break;
			}
			catch (std::exception)
			{
				std::cout << "Cannot read array.\nTry again: ";
			}
		}
		std::cout << "Entered array: ";
		PrintArray(Array);

		std::cout << "Choose sorting method:\n" <<
			"\t1) SeletionSort;\n" <<
			"\t2) InsertionSort;\n" <<
			"\t3) BubbleSort;\n" <<
			"\t4) MergeSort;\n" <<
			"\t5) QuickSort;\n" <<
			"\t6) ShellSort1;\n" <<
			"\t7) ShellSort2;\n" <<
			"\t8) ShellSort3;\n" <<
			"\t9) HeapSort;\n" <<
			"\t10) TimSort;\n" <<
			"\t11) IntroSort;\n";

		while (true)
		{
			try
			{
				getline(std::cin, input);
				number = stoi(input);
				if (number > 11 || number < 1)
					throw new std::exception();
				break;
			}
			catch (std::exception)
			{
				std::cout << "Incorrect number was entered.";
			}
		}

		CallSort(Array, number);

		std::cout << "Sorted array: ";
		PrintArray(Array);
		std::cout << std::endl;

		std::cout << "Do you want to continue? (Yes/No)\nYour answer: ";
		while (true)
		{
			getline(std::cin, input);
			if (input != "Yes" && input != "yes" &&
				input != "No" && input != "no"
				)
			{
				std::cout << "Incorrect input. Try again: ";
				continue;
			}
			else break;
		}
		if (input == "Yes" || input == "yes") continue;
		else break;
	}
}

void PrintArray(const std::vector<int>& A)
{
    for (int i = 0; i < A.size(); ++i) {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;
}

