#include "generator_defs.hpp"

/* Parse the program options - if help or version is required, terminate the program immediatelly. */
bpo::variables_map parseProgramOptions(int argc, char ** argv) {
	bpo::variables_map result;

	// Declare the supbported options.
	bpo::options_description visible("Other options");
	visible.add_options()
		("help,h", "display help")
		("version,v", "display version")
		;
	bpo::options_description invisible;
	invisible.add_options()
		("execution_values", bpo::value<vector<int> >()->required(), "parameters k, i, j")
		;
	bpo::options_description all;
	all.add(visible).add(invisible);
	bpo::positional_options_description pos_decr; pos_decr.add("execution_values", 3);
	bpo::store(bpo::command_line_parser(argc, argv).options(all).positional(pos_decr).run(), result);

	if (result.count("help")) {
		cout << "pasm_generator k i j" << endl;
		cout << "    m: number of species in the model" << endl;
		cout << "    j: number of regulators per specie" << endl;
		cout << "    l: size of the domain (each specie can attain value from the set {0,..,j-1})" << endl;
		cout << visible << "\n";
		exit(0);
	}

	if (result.count("version")) {
		cout << "Version 0.0.0.0" << "\n";
		exit(0);
	}

	bpo::notify(result);

	return result;
}

/* test correctness of the parameters */
void testParams(const vector<int> & params) {
	if (rng::find_if(params, [](const int i) {return i < 1; }) != params.end())
		throw invalid_argument("Only natural numbers (without zero) are allowed.");
	if (params[0] < params[1])
		throw invalid_argument("More regulators required than species existing.");
}