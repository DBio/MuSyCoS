/*
* Copyright (C) 2012-2014 - Adam Streck
*/
#ifndef MODEL_PARSERS_HPP
#define MODEL_PARSERS_HPP

#include "model.hpp"

namespace ModelParsers {
	const string spec_name("[[:alpha:]](\\w)*");
	const string assign("'=");
	const string ext_literal(spec_name + "(\\{\\d+(,\\d+)*\\})?");
	const string clause("(\\d*\\*)?" + ext_literal + "(\\*" + ext_literal + ")*");
	const string rule(spec_name + assign + clause + "(\\+" + clause + ")*");

	void control_syntax(const vector<string> & lines) {
		regex control{ rule };
		for (const size_t i : crange(0u, lines.size())) 
			if (!regex_match(lines[i], control))
				throw runtime_error("Line " + to_string(i) + ": invalid format \"" + lines[i] + "\"");
	}

	Specie obtainSpecie(string line) {
		Specie result;

		return result;
	}

	void control_semantics(const vector<Specie> & species) {

	}
};

#endif