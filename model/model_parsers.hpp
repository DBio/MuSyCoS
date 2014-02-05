/*
* Copyright (C) 2012-2014 - Adam Streck
*/
#ifndef MODEL_PARSERS_HPP
#define MODEL_PARSERS_HPP

#include "model.hpp"

namespace ModelParsers {
	const string spec_name_form("([[:alpha:]]\\w*)"); 
	const string assign_form("'=");
	const string ext_literal_form(spec_name_form + "(\\{\\d+(,\\d+)*\\})?");
	const string clause_from("((\\d*\\*)?" + ext_literal_form + "(\\*" + ext_literal_form + ")*)");
	const string line_form(spec_name_form + assign_form + clause_from + "(\\+" + clause_from + ")*");

	void control_syntax(const vector<string> & lines) {
		regex control{ line_form };
		for (const size_t i : crange(0u, lines.size())) 
			if (!regex_match(lines[i], control))
				throw runtime_error("Line " + to_string(i) + ": invalid format \"" + lines[i] + "\"");
	}

	const string num_literal_form = "(\\d+)[^\\},]";
	int findMaxNumLiteral(string line) {
		smatch clause_matches;
		vector<string> string_literals;
		// Obtain all the clauses (only the whole match).
		while (regex_search(line, clause_matches, regex(num_literal_form))) {
			string_literals.emplace_back(clause_matches[1]); // Capture the first (and only) submatch
			line = clause_matches.suffix().str();
		}
		if (string_literals.empty())
			return 1;

		vector<int> numeric_literals(string_literals.size());
		rng::transform(string_literals, numeric_literals.begin(), [](const string & s) {return stoi(s); });
		return *rng::max_element(numeric_literals);
	}

	const string rule_form = "=(.*)";
	Specie obtainSpecie(string line) {	
		smatch match_name;
		regex_search(line, match_name, regex(spec_name_form));
		smatch match_rule;
		regex_search(line, match_rule, regex(rule_form));

		return{ match_name[0].str(), findMaxNumLiteral(line), match_rule[1].str() };
	}

	const Specie& testName(const vector<Specie> & species, const string & current_specie) {
		auto tested = rng::find_if(species, [&current_specie](const Specie & spec) {return spec.name.compare(current_specie) == 0; });
		if (tested == species.end())
			throw runtime_error("Specie \"" + current_specie + "\" not found on the left side.");
		else
			return *tested;
	}

	void testValues(const Specie & tested, string values_set) {
		smatch match_numbers;
		while (regex_search(values_set, match_numbers, regex("\\d+"))) {
			if (stoi(match_numbers[0]) > tested.max_val)
				throw runtime_error("Value " + match_numbers[0].str() + " is greater than maximal derived value of the specie " + tested.name);
			values_set = match_numbers.suffix().str();
		}
	}

	void control_semantics(const vector<Specie> & species) {
		for (const Specie specie : species) {
			string specie_rule = specie.rule;
			smatch match_clauses;
			while (regex_search(specie_rule, match_clauses, regex(ext_literal_form))) {
				Specie tested = testName(species, match_clauses[1]);
				testValues(tested, match_clauses[2]);
				specie_rule = match_clauses.suffix().str();
			}
		}
	}
};

#endif