#pragma once
#include <vector>
#include <iostream>

#include "SortFunctions.h"

void CallSort(std::vector<int>& A, int number)
{
    switch (number)
    {
    case 1:
        SortFunctions::SelectionSort(A, 0, A.size() - 1);
        break;
    case 2:
        SortFunctions::InsertionSort(A, 0, A.size() - 1);
        break;
    case 3:
        SortFunctions::BubbleSort(A, 0, A.size() - 1);
        break;
    case 4:
        SortFunctions::MergeSort(A, 0, A.size() - 1);
        break;
    case 5:
        SortFunctions::QuickSort(A, 0, A.size() - 1);
        break;
    case 6:
        SortFunctions::ShellSort1(A, 0, A.size() - 1);
        break;
    case 7:
        SortFunctions::ShellSort2(A, 0, A.size() - 1);
        break;
    case 8:
        SortFunctions::ShellSort3(A, 0, A.size() - 1);
        break;
    case 9:
        SortFunctions::HeapSort(A, 0, A.size() - 1);
        break;
    case 10:
        SortFunctions::TimSort(A, 0, A.size() - 1);
        break;
    case 11:
        SortFunctions::IntroSort(A, 0, A.size() - 1);
        break;
    default:
        break;
    }
}