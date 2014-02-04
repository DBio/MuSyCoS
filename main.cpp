#include "general/common_functions.hpp"

/* Parse the program options - if help or version is required, terminate the program immediatelly. */
bpo::variables_map parseProgramOptions(int argc, char ** argv) {
	bpo::variables_map result;
	
	// Declare the supbported options.
	bpo::options_description visible("Execution options");
	visible.add_options()
		("help,h", "display help")
		("version,v", "display version")
		;
	bpo::options_description invisible;
	invisible.add_options()
		("model", bpo::value<string>()->required(), "file holding the model, must have .pan suffix")
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

void testPath(const bfs::path & path) {
	if (!bfs::exists(path))
		throw runtime_error("File does not exist: " + path.string());
	bfs::path model_extension = path.extension();
	if (model_extension.string().compare(MODEL_EXTENSION) != 0)
		throw runtime_error("File: " + path.string() + " has an invalid suffix - it must be " + MODEL_EXTENSION);
}

string readModel(const bfs::path & path) {
	string result;

	ifstream model_file(path.string(), ios::in);
	string line;
	while (getline() {

	}

	return result;
}

int main(int argc, char ** argv) {
	bpo::variables_map program_options;
	try {
		program_options = parseProgramOptions(argc, argv);
	}
	catch (exception & e) {
		BOOST_LOG_TRIVIAL(error) << "An exception cought while parsing input options: " << e.what();
		exit(1);
	}
	
	string model;
	string model_name;
	try {
		bfs::path model_path(program_options["model"].as<string>());
		testPath(model_path);
		model = readModel(model_path);
		model_name = model_path.stem().string();
	}
	catch (exception & e) {
		BOOST_LOG_TRIVIAL(error) << "An exception cought while reading the model file: " << e.what();
		exit(2);
	}
	return 0;
}
