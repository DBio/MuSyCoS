/*
 * Copyright (c) 2014 - Adam Streck 
 * MuSyCoS - Multivalued Synchronous Networks Constraint Solver 
 * MuSyCoS is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3. 
 * MuSyCoS is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>. 
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> 
 */
#pragma once

#include "../general/definitions.hpp"

template<typename ValType>
class BasicSpace {
protected:
	using LNE = Gecode::LinIntExpr;

	/* Take a vector of expressions and apply a binary function on those (or don't if there is only a single expression) */
	LNE applyOnSet(vector<LNE> exprs, LNE(*b_fun)(const LNE&, const LNE&)) {
		LNE result;
		if (exprs.size() == 1) {
			result = exprs[0];
		}
		else if (exprs.size() > 1) {
			result = b_fun(exprs[0], exprs[1]);
			for (const size_t i : crange(2u, exprs.size()))
				result = b_fun(result, exprs[i]);
		}
		return result;
	}

	/* Obtain integers from a set string expression (of the form {n_1,...,n_k} */
	vector<int> getNumbers(string set) {
		// If no numbers are specified, return 1 by defaults
		if (set.empty())
			return{ 1 };
		// Else get all the numbers in the set and return them as integers
		vector<string> parsed = getAllMatches("\\d+", set);
		vector<int> result(parsed.size());
		rng::transform(parsed, result.begin(), boost::lexical_cast<int, string>);
		return result;
	}

	/* Take a Post algebra literal and convert it into a Gecode expression */
	LNE convertLiteral(const Gecode::IntVarArray & spec_vars, const Model & model, const string & ext_literal, const int max_val) {
		// Obtain index of the variable
		smatch matches;
		regex_search(ext_literal, matches, regex(ModelParsers::spec_name_form));
		int spec_no = model.findIndex(matches[0].str());
		// Obtain values the variable can attain
		auto numbers = getNumbers(matches.suffix().str());
		// Create constraint stating possible values the specie can have
		Gecode::BoolExpr lit_present{ spec_vars[spec_no] == numbers[0] };
		for_each(numbers.begin() + 1, numbers.end(), [&lit_present, &spec_vars, spec_no](const int n) {
			lit_present = lit_present || spec_vars[spec_no] == n;
		});
		// Return an expression saying: "if the value is bigger than 
		return LNE{ Gecode::ite(lit_present, max_val, 0) };
	}

	/* Take a Post algebra clause and convert it into a Gecode expression */
	LNE convertClause(const Gecode::IntVarArray & spec_vars, const Model & model, const string & clause, const int max_val) {
		// Obtain the literals and the numeric literal, if present
		vector<string> literals;
		boost::split(literals, clause, boost::is_any_of("*"));
		int num_literal = regex_match(literals[0], regex("\\d")) ? stoi(literals[0]) : -1;
		vector<LNE> exprs(literals.size());
		// Convert all literals except possibly the first one which may be a number
		auto begin_pos = begin(literals) + (num_literal != -1 ? 1 : 0);
		transform(begin_pos, end(literals), exprs.begin(), [&spec_vars, &model, max_val, this](const string & ext_literal){
			return this->convertLiteral(spec_vars, model, ext_literal, max_val);
		});
		// Place back the numeric literal if there is one
		if (num_literal != -1)
			exprs.back() = LNE(num_literal);

		return applyOnSet(exprs, Gecode::min);
	}

	/* Take a Post algebra rule in the DNF and convert it into a Gecode expression */
	LNE convertRule(const Gecode::IntVarArray & spec_vars, const Model & model, const Specie & specie) {
		vector<string> clauses;
		boost::split(clauses, specie.rule, boost::is_any_of("+"));

		vector<LNE> exprs(clauses.size());
		rng::transform(clauses, exprs.begin(), [&spec_vars, &model, &specie, this](const string & clause){
			return this->convertClause(spec_vars, model, clause, specie.max_val);
		});

		return applyOnSet(exprs, Gecode::max);
	}

public:
	virtual void boundSpecie(const size_t specie_no, const int max_val) = 0;

	virtual vector<ValType> getSolution() const = 0;

	virtual void print() const = 0;
};