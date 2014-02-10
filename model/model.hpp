/*
 * Copyright (c) 2014 - Adam Streck 
 * MuSyCoS - Multivalued Synchronous Networks Constraint Solver 
 * MuSyCoS is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3. 
 * MuSyCoS is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>. 
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> 
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

