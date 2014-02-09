#include "generator/generator_defs.hpp"
#include "generator/generator_parser.hpp"
#include "generator/MVQMC.hpp"

/* Generate unique names for k species. */
vector<string> generateNames(const int m) {
	vector<string> result(m);

	const size_t len = (m / 26) + (m % 26 == 0 ? 0 : 1);
	string name(len, 'a');
	string min(len, 'a');
	string max(len, 'z');
	result[0] = min;
	generate(result.begin()+1, result.end(), [&name, &min, &max]() {iterate(max, min, name); return name; });

	return result;
}

/* pick k random numbers out of the set */
template<typename Object>
vector<Object> pickK(vector<Object> & set, const int k) {
	if (k > set.size())
		throw runtime_error("Trying to get k different numbers out of n, but k > n");
	rng::random_shuffle(set);
	return vector<Object>(set.begin(), set.begin() + k);
}

/* Distribute the conformations on j inputs in between l vectors for values */
vector<vector<vector<int> > > distributeConditions(const int j, const int l) {
	vector<vector<vector<int> > > result(l);

	random_device rd;
	vector<int> current(j, 0), min(j, 0), max(j, l - 1);
	do {
		result[rd() % l].emplace_back(current);
	} while (iterate(max, min, current));

	return result;
}

/* For the conditions multiplied by values from [1,l] create a compact representation. */
map<int, vector<PMin> > compactizeConditions(vector<vector<vector<int> > > native) {
	map<int, vector<PMin> > result;

	for (const size_t i : crange(1u, native.size())) {
		result.insert({ i, MVQMC::compactize(move(native[i])) });
	}

	return result;
}

string textRange(const PLit & plit) {
	string result = { '{' };
	rng::for_each(plit, [&result](PVal pval){result += to_string(pval) + ","; });
	result.back() = '}';
	return result;
}

string textMinterm(const int val, const PMin & pmin, const vector<string> & names, const vector<size_t> & sources) {
	string result = to_string(val);

	for (const size_t i : cscope(pmin)) 
		result += "*" + names[sources[i]] + textRange(pmin[i]);

	return result;
}

string createLine(const vector<string> & names, const size_t target, const vector<size_t> & sources, map<int, vector<PMin> > & conditions) {
	string result;

	result = names[target] + "'=0*" + names[target] + "+";
	rng::for_each(conditions, [&names, &sources, &result](const pair<int, vector<PMin>> & condition){
		const int val = condition.first;
		rng::for_each(condition.second, [&names, &sources, &result, &val](const PMin & pmin) {
			result += textMinterm(val, pmin, names, sources) + "+";
		});
	});

	return result.substr(0, result.size() - 1);
}

int main(int argc, char ** argv) {
	bpo::variables_map program_options;
	vector<int> params;
	try {
		program_options = parseProgramOptions(argc, argv);
		params = program_options["execution_values"].as<vector<int> >();
		testParams(params);
	}
	catch (exception & e) {
		BOOST_LOG_TRIVIAL(error) << "An exception occurred while parsing input options: " << e.what();
		exit(1);
	}
	const int m = params[0];
	const int j = params[1];
	const int l = params[2];

	try {
		auto names = generateNames(m);
		size_t spec_no = 0;
		vector<size_t> regulators(names.size());
		rng::generate(regulators, [&spec_no]{return spec_no++; });
		fstream fout("model_" + to_string(m) + "_" + to_string(j) + "_" + to_string(l) + MODEL_EXTENSION, ios::out);

		for (const size_t spec_id : cscope(names)) {
			auto sources = pickK(regulators, j);
			auto conditions = distributeConditions(j, l);
			auto compact = compactizeConditions(move(conditions));
			fout << createLine(names, spec_id, sources, compact) << endl;
		}
	}
	catch (exception & e) {
		BOOST_LOG_TRIVIAL(error) << "An exception occurred while building the graph " << e.what();
		exit(2);
	}

	return 0;
}