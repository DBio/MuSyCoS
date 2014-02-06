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
		;
	bpo::options_description invisible;
	invisible.add_options()
		("model", bpo::value<string>()->required(), ("file holding the model, must have " + MODEL_EXTENSION + " suffix").c_str())
		;
	bpo::options_description all;
	all.add(visible).add(invisible);
	bpo::positional_options_description pos_decr; pos_decr.add("model", 1);

	bpo::store(bpo::command_line_parser(argc, argv).options(all).positional(pos_decr).run(), result);
	bpo::notify(result);

	if (result.count("help")) {
		cout << visible << "\n";
		exit(0);
	}

	if (result.count("version")) {
		cout << "Version 0.0.0.0" << "\n";
		exit(0);
	}

	return result;
}