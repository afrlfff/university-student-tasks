#pragma once
#include <iostream>
#include <vector>
#include <cmath>

class SortFunctions
{
public:
	static void SelectionSort(std::vector<int>& A, int low, int high);
	static void InsertionSort(std::vector<int>& A, int low, int high);
	static void BubbleSort(std::vector<int>& A, int low, int high);
	static void MergeSort(std::vector<int>& A, int low, int high);
	static void QuickSort(std::vector<int>& A, int low, int high);
	static void ShellSort1(std::vector<int>& A, int low, int high);
	static void ShellSort2(std::vector<int>& A, int low, int high);
	static void ShellSort3(std::vector<int>& A, int low, int high);
	static void HeapSort(std::vector<int>& A, int low, int high);
	static void TimSort(std::vector<int>& A, int low, int high);
	static void IntroSort(std::vector<int>& A, int low, int high);
private:
	static void Heapify(std::vector<int>& A, int n, int i);
	static int QSPartition(std::vector<int>& A, int low, int high);
	static void MSMerge(std::vector<int>& A, int low, int mid, int high);
	static int GetMinRun(int n);
	static void TSMerge(std::vector<int>& A, int low, int mid, int high);
	static int choosePivot(std::vector<int>& A, int low, int high);
	static int ISpartition(std::vector<int>& A, int low, int high);
	static void introSort(std::vector<int>& A, int low, int high, int depthLimit);
};

// START IMPLEMENTATION

// =================================================================
// selection sort
void SortFunctions::SelectionSort(std::vector<int>& A, int low, int high)
{
	int maxInd;
	int i, j;

	for (i = low; i <= high; ++i)
	{
		maxInd = low;
		for (j = low + 1; j <= (high - i); ++j)
		{
			if (A[j] > A[maxInd]) {
				maxInd = j;
			}
		}
		std::swap(A[maxInd], A[high - i]);
	}
}

// =================================================================
// insertion sort
void SortFunctions::InsertionSort(std::vector<int>& A, int low, int high)
{
	int i, j;
	for (i = low + 1; i <= high; ++i)
	{
		for (j = i; j > low; --j)
		{
			if (A[j] < A[j - 1]) {
				std::swap(A[j], A[j - 1]);
			}
			else break;
		}
	}
}

// =================================================================
// bubble sort
void SortFunctions::BubbleSort(std::vector<int>& A, int low, int high)
{
	int i, j;
	for (i = low; i <= high; ++i)
	{
		for (j = low; j < (high - i); ++j)
		{
			if (A[j] > A[j + 1]) {
				std::swap(A[j], A[j+1]);
			}
		}
	}
}

// =================================================================
// merge sort
void SortFunctions::MergeSort(std::vector<int>& A, int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        MergeSort(A, low, mid);
        MergeSort(A, mid + 1, high);
        
        MSMerge(A, low, mid, high);
    }
}

void SortFunctions::MSMerge(std::vector<int>& A,int low, int mid, int high)
{
    int indA1 = low, indA2 = mid + 1, indR = 0;
    int resultLength = high - low + 1;
    std::vector<int> result(resultLength);

    while (indA1 < (mid + 1) && indA2 < (high + 1))
    {
        if (A[indA1] > A[indA2]) {
            result[indR] = A[indA2];
            ++indA2;
        } else {
            result[indR] = A[indA1];
            ++indA1;
        }
        ++indR;
    }
    if (indA1 == (mid + 1))
    {
        while (indR < resultLength) {
            result[indR] = A[indA2];
            ++indA2; ++indR;
        }
    }
    else // if (indA2 == high+1)
    {
        while (indR < resultLength) {
            result[indR] = A[indA1];
            ++indA1; ++indR;
        }
    }

    for (int i = 0; i < resultLength; ++i)
    {
        A[low + i] = result[i];
    }
}

// =================================================================
// quick sort
void SortFunctions::QuickSort(std::vector<int>& A, int low, int high)
{
    if (low < high)
    {
        int pivotIndex = QSPartition(A, low, high);

        QuickSort(A, low, pivotIndex - 1);
        QuickSort(A, pivotIndex + 1, high);
    }
}

int SortFunctions::QSPartition(std::vector<int>& A, int low, int high)
{   
    // to avoid stack overflow error on sorted (or back sorted) arrays
    std::swap(A[(high + low) / 2], A[high]);

    int pivot = A[high];
    int i = low;
    int j = high - 1;

    while (true) {
        while (i <= high && A[i] < pivot) ++i;
        while (j >= low && A[j] >= pivot) --j;

        if (i <= j) {
            std::swap(A[i], A[j]);
		}
		else break;
    }

    std::swap(A[i], A[high]);
    return i;
}

// =================================================================
// shell sort #1
void SortFunctions::ShellSort1(std::vector<int>& A, int low, int high)
{
	int size = (high - low + 1);
	int i, d = size / 2;
	while (d > 0)
	{
		for (i = low; i <= (high - d); ++i) {
			if (A[i] > A[i + d]) {
				std::swap(A[i], A[i+d]);
			}
		}
		d /= 2;
	}
	InsertionSort(A, low, high);
}

// =================================================================
// shell sort #2
void SortFunctions::ShellSort2(std::vector<int>& A, int low, int high)
{
	const int SIZE = 28;
	int steps[] = {
		1, 5, 19, 41, 109, 209, 505, 929, 2161, 3905, 8929, 16001,
		36289, 64769, 146305, 260609, 587521, 1045505, 2354689,
		4188161, 9427969, 16764929, 37730305, 67084289, 150958081,
		268386305, 603906049, 1073643521
	};
	/*
		if (steps.size() % 2 == 0)
			d = 9 * pow(2, steps.size()) - 9 * pow(2, steps.size() / 2) + 1;
		else
			d = 8 * pow(2, steps.size()) - 6 * pow(2, (steps.size() + 1) / 2) + 1;
	*/
	int i, d, ind;
	for (ind = -1; (ind + 1) < SIZE; ++ind)
	{
		if (steps[ind + 1] > (high - low + 1)) {
			break;
		}
	}
	while (ind > 0)
	{
		d = steps[ind];
		for (i = low; i <= (high - d); ++i) {
			if (A[i] > A[i + d]) {
				std::swap(A[i], A[i + d]);
			}
		}
		--ind;
	}
	InsertionSort(A, low, high);
}

// =================================================================
// shell sort #3
void SortFunctions::ShellSort3(std::vector<int>& A, int low, int high)
{
	const int SIZE = 9;
	int steps[SIZE] = { 1, 4, 10, 23, 57, 132, 301, 701, 1750 };
	int i, d, ind;
	for (ind = -1; (ind + 1) < SIZE; ++ind)
	{
		if (steps[ind + 1] > (high - low + 1)) {
			break;
		}
	}

	while (ind > 0)
	{
		d = steps[ind];
		for (i = low; i <= (high - d); ++i) {
			if (A[i] > A[i + d]) {
				std::swap(A[i], A[i + d]);
			}
		}
		--ind;
	}
	InsertionSort(A, low, high);
}

// =================================================================
// heap sort
void SortFunctions::HeapSort(std::vector<int>& A, int low, int high)
{
    if (low >= high)
        return;

    int i, size = (high - low + 1);
    for (i = (high + 1) / 2 - 1; i >= low; --i)
    {
        Heapify(A, size, i);
    }

    for (i = high; i >= low; --i)
    {
        std::swap(A[0], A[i]);

        Heapify(A, i, 0);
    }
}

void SortFunctions::Heapify(std::vector<int>& A, int n, int i)
{
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = i;

    if (left < n && A[left] > A[largest])
        largest = left;
    if (right < n && A[right] > A[largest])
        largest = right;

    if (largest != i)
    {
        std::swap(A[largest], A[i]);
        Heapify(A, n, largest);
    }
}

// =================================================================
// tim sort
void SortFunctions::TimSort(std::vector<int>& A, int low, int high)
{
    if (low >= high)
        return;

    int i, size = high - low + 1;
	int mid, right;

    int minRun = GetMinRun(size);
    for (i = 0; i < size; i += minRun)
        InsertionSort(A, i, std::min(minRun, size - i) - 1);

    for (int sizeRun = minRun; sizeRun < size; sizeRun *= 2)
    {
        for (i = 0; i < size; i += 2 * sizeRun)
        {
            mid = std::min(i + sizeRun - 1, size - 1);
            right = std::min(i + 2 * sizeRun - 1, size - 1);
            TSMerge(A, i, mid, right);
        }
    }
}

int SortFunctions::GetMinRun(int n)
{
    int r = 0;
    while (n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

void SortFunctions::TSMerge(std::vector<int>& A, int low, int mid, int high)
{
    int sizeLeft = mid - low + 1;
    int sizeRight = high - mid;

    std::vector<int> tempLeft(sizeLeft);
    std::vector<int> tempRight(sizeRight);

    for (int i = 0; i < sizeLeft; i++)
        tempLeft[i] = A[low + i];
    for (int i = 0; i < sizeRight; i++)
        tempRight[i] = A[mid + 1 + i];

    int i = 0, j = 0, k = low;
    while (i < sizeLeft && j < sizeRight)
    {
        if (tempLeft[i] <= tempRight[j])
            A[k++] = tempLeft[i++];
        else
            A[k++] = tempRight[j++];
    }
    while (i < sizeLeft)
        A[k++] = tempLeft[i++];
    while (j < sizeRight)
        A[k++] = tempRight[j++];
}

// =================================================================
// intro sort
void SortFunctions::IntroSort(std::vector<int>& A, int low, int high) {
    int depthLimit = 2 * log(high - low + 1);
    introSort(A, 0, high - low, depthLimit);
}

int SortFunctions::choosePivot(std::vector<int>& A, int low, int high) {
    int mid = (low + high) / 2;

    if (A[low] > A[high]) {
        std::swap(A[low], A[high]);
    }
    if (A[low] > A[mid]) {
        std::swap(A[low], A[mid]);
    }
    if (A[mid] > A[high]) {
        std::swap(A[mid], A[high]);
    }

    std::swap(A[mid], A[high - 1]);
    return high - 1;
}

int SortFunctions::ISpartition(std::vector<int>& A, int low, int high) {
    int pivotIndex = choosePivot(A, low, high);
    int pivot = A[pivotIndex];

    int i = low;
    int j = high - 1;

    while (true) {
        while (A[++i] < pivot) {}
        while (A[--j] > pivot) {}
        if (i < j) {
            std::swap(A[i], A[j]);
        }
        else break;
    }

    std::swap(A[i], A[high - 1]);
    return i;
}

void SortFunctions::introSort(std::vector<int>& A, int low, int high, int depthLimit) {
	static const int INSERTION_THRESHOLD = 16;
    while (high - low > INSERTION_THRESHOLD) {
        if (depthLimit == 0) {
            SortFunctions::HeapSort(A, low, high);
            return;
        }
        --depthLimit;

        int pivotIndex = ISpartition(A, low, high);
        introSort(A, pivotIndex + 1, high, depthLimit);

        high = pivotIndex;
    }

    SortFunctions::InsertionSort(A, low, high);
}


// END IMPLEMENTATION