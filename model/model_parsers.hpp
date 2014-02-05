/*
* Copyright (C) 2012-2014 - Adam Streck
*/
#ifndef MODEL_PARSERS_HPP
#define MODEL_PARSERS_HPP

#include "model.hpp"

// @brief A namespace contaning functions that both control and read the input. All the controls are based on regular expressions.
namespace ModelParsers {
	const string spec_name_form("([[:alpha:]]\\w*)"); 
	const string assign_form("'=");
	const string ext_literal_form(spec_name_form + "(\\{\\d+(,\\d+)*\\})?");
	const string clause_from("((\\d*\\*)?" + ext_literal_form + "(\\*" + ext_literal_form + ")*)");
	const string line_form(spec_name_form + assign_form + clause_from + "(\\+" + clause_from + ")*");
	const string num_literal_form = "(\\d+)[^\\},]";
	const string rule_form = "=(.*)";

	/* Throw an exception if the model has syntactical errors. */
	void control_syntax(const vector<string> & lines) {
		regex control{ line_form };
		for (const size_t i : crange(0u, lines.size())) 
			if (!regex_match(lines[i], control))
				throw runtime_error("Line " + to_string(i) + ": invalid format \"" + lines[i] + "\"");
	}
	
	/* Obtain a literal with maximal value on the right hand side of the rule  */
	int findMaxNumLiteral(string line) {
		int result = numeric_limits<int>::min();

		// Find all the numbers
		smatch clause_matches;
		vector<string> string_literals;
		while (regex_search(line, clause_matches, regex(num_literal_form))) {
			string_literals.emplace_back(clause_matches[1]); // Capture the first (and only) submatch that holds the whole number only
			line = clause_matches.suffix().str();
		}
		// The default value is 1 (no numbers found)
		if (string_literals.empty())
			return 1;

		// Pick the biggest literal 
		rng::for_each(string_literals, [&result](const string & s) {result = max(result, stoi(s)); });
		return result;
	}

	
	/* Get all the specie data from the line (name, max value and rule) */
	Specie obtainSpecie(string line) {	
		smatch match_name;
		regex_search(line, match_name, regex(spec_name_form));
		smatch match_rule;
		regex_search(line, match_rule, regex(rule_form));

		return{ match_name[0].str(), findMaxNumLiteral(line), match_rule[1].str() };
	}

	/* Find a specie with the given name a renturn it, if there is such.
	 * @param required_specie	name of the specie that is being searched for.
	 */
	const Specie& testName(const vector<Specie> & species, const string & required_specie) {
		auto tested = rng::find_if(species, [&required_specie](const Specie & spec) {return spec.name.compare(required_specie) == 0; });
		if (tested == species.end())
			throw runtime_error("Specie \"" + required_specie + "\" not found on the left side.");
		else
			return *tested;
	}

	/* Throw an error if the set of values for a literal contains a bigger value than the literal can attain. */
	void testValues(const Specie & tested, string values_set) {
		smatch match_numbers;
		while (regex_search(values_set, match_numbers, regex("\\d+"))) {
			if (stoi(match_numbers[0]) > tested.max_val)
				throw runtime_error("Value " + match_numbers[0].str() + " is greater than maximal derived value of the specie " + tested.name);
			values_set = match_numbers.suffix().str();
		}
	}

	/* Throw an exception if the model has semantical error (the specie names or their values are not right) */
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