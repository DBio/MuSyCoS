/*
* Copyright (C) 2012-2014 - Adam Streck
*/
#pragma once
#include "../general/common_functions.hpp"

struct Specie {
	string name;
	int max_val;
	string rule;
};

struct Model {
	string name; ///< Name of the model, based on the model path.
	int max_value; ///< Maximal values any specie can attain.
	vector<Specie> species; ///< All the species and their related rules.

	size_t findIndex(const string & name) const {
		for (const size_t i : cscope(species))
			if (species[i].name == name)
				return i;
		return INF;
	}
};

