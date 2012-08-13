#pragma once
#include "GeneralSuffixTree.h"
#include <string>

class Overlap
{
public:
	Overlap() {}
	Overlap(int, int, short int);
	int string_i;
	int string_j;
	short int overlap;
	Overlap* left;
	Overlap* right;
	std::string get_string(const GeneralSuffixTree&) const;
	bool left_edge_merged(int);
	bool right_edge_merged(int);
	bool match_contig(Overlap*);
	void merge_right(Overlap*);
	void merge_left(Overlap*);
};

