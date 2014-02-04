/*
* Copyright (C) 2012-2014 - Adam Streck
*/
#ifndef MODEL_HPP
#define MODEL_HPP

#include "../general/common_functions.hpp"

struct Specie {
	string name;
	int max_val;
	string rule;
};

struct Model {
	string name; ///< Name of the model, based on the model path.
	vector<Specie> species; ///< All the species and their related rules.
};

#endif