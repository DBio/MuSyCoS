/*
 * Copyright (c) 2014 - Adam Streck 
 * MuSyCoS - Multivalued Synchronous Networks Constraint Solver 
 * MuSyCoS is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3. 
 * MuSyCoS is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>. 
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> 
 */
#pragma once
#include "../constraints/space_solver.hpp"

ofstream initFile(const string & path_to_model, const string & model_name) {
	bfs::path model_path(path_to_model);
	string output_file_name = model_path.parent_path().string() + model_name + "_stable.csv";
	return ofstream(output_file_name, ios::out);
}

void outputLegend(const vector<Specie> & model_spec, ofstream & output_file) {
	string legend;
	rng::for_each(model_spec, [&legend](Specie spec){legend += spec.name + ","; });
	output_file << legend.substr(0, legend.size() - 1);
}

void outputResults(ofstream & output_file, SpaceSolver<SteadySpace> & solver) {
	vector<int> result = solver.next();
	while (!result.empty()) {
		output_file << endl;
		string line;
		rng::for_each(result, [&line](int i){line += to_string(i) + ","; });
		output_file << line.substr(0, line.size() - 1);
		result = solver.next();
	}
}