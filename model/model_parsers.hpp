/*
* Copyright (C) 2012-2014 - Adam Streck
*/
#ifndef MODEL_PARSERS_HPP
#define MODEL_PARSERS_HPP

#include "model.hpp"

namespace ModelParsers {
	const string spec_name("[:alpha:](\\w)*");
	const string assign("'=");
	const string ext_literal(spec_name + "(\\{\\d+\\})?");
	const string clause("(\\d*\\*)?" + ext_literal + "(\\*" + ext_literal + ")*");
	const string rule(spec_name + assign + clause + "(\\+" + clause + ")*");

	void control_syntax(const vector<string> & lines) {
		rng::for_each(lines, [](string line){
			line.erase(rng::remove_if(line, isspace), line.end());
			if (!regex_match(line, regex(rule))) 
				throw runtime_error("Invalid format of the line \"" + line + "\"");
		});
	}

	Specie obtainSpecie(string line) {
		Specie result;

		return result;
	}

	void control_semantics(const vector<Specie> & species) {

	}
};

#endif