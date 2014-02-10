/*
 * Copyright (c) 2014 - Adam Streck 
 * MuSyCoS - Multivalued Synchronous Networks Constraint Solver 
 * MuSyCoS is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3. 
 * MuSyCoS is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>. 
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> 
 */
#pragma once

#include "common_functions.hpp"

/* Parse the program options - if help or version is required, terminate the program immediatelly. */
bpo::variables_map parseProgramOptions(int argc, char ** argv) {
	bpo::variables_map result;

	// Declare the supbported options.
	bpo::options_description visible("Execution options");
	visible.add_options()
		("help,h", "display help")
		("version,v", "display version")
		("steady,s", "compute steady states")
		("reach,r", "test reachability")
		;
	bpo::options_description invisible;
	invisible.add_options()
		("model", bpo::value<string>()->required(), ("file holding the model, must have " + MODEL_EXTENSION + " suffix").c_str())
		;
	bpo::options_description all;
	all.add(visible).add(invisible);
	bpo::positional_options_description pos_decr; pos_decr.add("model", 1);
	bpo::store(bpo::command_line_parser(argc, argv).options(all).positional(pos_decr).run(), result);
	
	if (result.count("help")) {
		cout << visible << "\n";
		exit(0);
	}

	if (result.count("version")) {
		cout << VERSION << "\n";
		exit(0);
	}

	bpo::notify(result);

	return result;
}