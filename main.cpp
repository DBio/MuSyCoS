#include "model/model.hpp"
#include "model/model_parsers.hpp"

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

void testPath(const bfs::path & path) {
	if (!bfs::exists(path))
		throw runtime_error("file does not exist");
	bfs::path model_extension = path.extension();
	if (model_extension.string().compare(MODEL_EXTENSION) != 0)
		throw runtime_error("suffix must be " + MODEL_EXTENSION);
}

vector<string> readModel(const bfs::path & path) {
	vector<string> result;

	ifstream model_file(path.string(), ios::in);
	string line;
	while (getline(model_file, line)) {
		if (line.empty())
			throw runtime_error("an empty line encountered, no empty lines allowed");
		result.push_back(line);
	}

	return result;
}

int main(int argc, char ** argv) {
	string model_path;
	try {
		auto program_options = parseProgramOptions(argc, argv);
		model_path = program_options["model"].as<string>();
	}
	catch (exception & e) {
		BOOST_LOG_TRIVIAL(error) << "An exception cought while parsing input options: " << e.what();
		exit(1);
	}
	
	Model model;
	try {
		bfs::path model_path(model_path);
		testPath(model_path);

		model.name = model_path.stem().string();

		vector<string> model_content = readModel(model_path);
		ModelParsers::control_syntax(model_content);
		model.species.resize(model_content.size());
		rng::transform(model_content, model.species.begin(), ModelParsers::obtainSpecie);
		ModelParsers::control_semantics(model.species);
	}
	catch (exception & e) {
		BOOST_LOG_TRIVIAL(error) << "An exception cought while reading the model file \"" << model_path << "\":" << e.what();
		exit(2);
	}


	return 0;
}
