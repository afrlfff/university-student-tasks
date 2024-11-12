#pragma once

#include <iostream>
#include <algorithm>

#include "StringL.h"
#include "Array.h"

struct Suffix
{
	int index; // To store original index
	int rank1; // To store ranks and next rank pair
    int rank2; // To store ranks and next rank pair
    Suffix(const int index, const int rank1, const int rank2): index(index), rank1(rank1), rank2(rank2) {}
	Suffix() = default;
};

// comparsion
static int _suffix_cmp(const Suffix& a, const Suffix& b)
{
	return (a.rank1 == b.rank1)? (a.rank2 < b.rank2 ? 1: 0):
			(a.rank1 < b.rank1 ? 1: 0);
}


// build suffix array of string after pushing endChar to the back with no making a new string in memory
template <typename charType>
Array<int> buildSuffixArray(const StringL<charType>& txt, const charType endChar)
{
	const size_t NEW_SIZE = txt.size() + 1; // length of new text (with endChar at the end)

	// Inicialize suffixes
	Array<Suffix> suffixes(NEW_SIZE);
	for (size_t i = 0; i < txt.size() - 1; ++i) {
		suffixes.push_back(Suffix(i, txt[i], txt[i + 1]));
	}
	suffixes.push_back(Suffix(txt.size() - 1, txt[txt.size() - 1], endChar));
	suffixes.push_back(Suffix(txt.size(), endChar, -1));

	// Sort in ascending
	std::sort(suffixes.begin(), suffixes.end(), _suffix_cmp);

	// Get sorted suffixes (i.e suffix array) 
	Array<int> ind(NEW_SIZE, 0);
	for (size_t k = 4; k < 2*NEW_SIZE; k = k*2)
	{
		int rank = 0;
		int prev_rank = suffixes[0].rank1;
		suffixes[0].rank1 = rank;
		ind[suffixes[0].index] = 0;

		for (size_t i = 1; i < NEW_SIZE; ++i)
		{
			if (suffixes[i].rank1 == prev_rank &&
				suffixes[i].rank2 == suffixes[i-1].rank2)
			{
				prev_rank = suffixes[i].rank1;
				suffixes[i].rank1 = rank;
			} else {
				prev_rank = suffixes[i].rank1;
				suffixes[i].rank1 = ++rank;
			}
			ind[suffixes[i].index] = i;
		}

		int nextindex;
		for (size_t i = 0; i < NEW_SIZE; ++i) {
			nextindex = suffixes[i].index + k/2;
			suffixes[i].rank2 = (nextindex < NEW_SIZE) ?
								(suffixes[ind[nextindex]].rank1) : -1;
		}

		std::sort(suffixes.begin(), suffixes.end(), _suffix_cmp);
	}
	ind.free_memory();

	// Concatenate suffixes to an array
	Array<int> suffixArr(NEW_SIZE);
	for (size_t i = 0; i < NEW_SIZE; ++i)
		suffixArr.push_back(suffixes[i].index);


	return suffixArr;
}


// END