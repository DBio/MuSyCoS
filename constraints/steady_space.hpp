#include "../general/definitions.hpp"
#include "../model/model_parsers.hpp"
#include "basic_space.hpp"

#pragma once

class SteadySpace : public Gecode::Space, public BasicSpace<int> {
	using LNE = Gecode::LinIntExpr;

	/* Obtain integers from a set string expression (of the form {n_1,...,n_k} */
	vector<int> getNumbers(string set) {
		// If no numbers are specified, return 1 by defaults
		if (set.empty())
			return { 1 };
		// Else get all the numbers in the set and return them as integers
		vector<string> parsed = getAllMatches("\\d+", set);
		vector<int> result(parsed.size());
		rng::transform(parsed, result.begin(), boost::lexical_cast<int, string>);
		return result;
	}

	/* Take a Post algebra literal and convert it into a Gecode expression */
	LNE convertLiteral(const Model & model, const string & ext_literal, const int max_val) {
		// Obtain index of the variable
		smatch matches;
		regex_search(ext_literal, matches, regex(ModelParsers::spec_name_form));
		int spec_no = model.findIndex(matches[0].str());
		// Obtain values the variable can attain
		auto numbers = getNumbers(matches.suffix().str());
		// Create constraint stating possible values the specie can have
		Gecode::BoolExpr lit_present{ species[spec_no] == numbers[0] };
		for_each(numbers.begin() + 1, numbers.end(), [&lit_present, this, spec_no](const int n) {
			lit_present = lit_present || this->species[spec_no] == n;
		});
		// Return an expression saying: "if the value is bigger than 
		return LNE{ Gecode::ite(lit_present, max_val, 0) };
	}

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

	/* Take a Post algebra clause and convert it into a Gecode expression */
	LNE convertClause(const Model & model, const string & clause, const int max_val) {
		// Obtain the literals and the numeric literal, if present
		vector<string> literals;
		boost::split(literals, clause, boost::is_any_of("*"));
		int num_literal = regex_match(literals[0], regex("\\d")) ? stoi(literals[0]) : -1;
		vector<LNE> exprs(literals.size());
		// Convert all literals except possibly the first one which may be a number
		transform(literals.begin() + (num_literal != -1 ? 1 : 0), literals.end(), exprs.begin(), [&model, max_val, this](const string & ext_literal){
			return this->convertLiteral(model, ext_literal, max_val); 
		});
		// Place back the numeric literal if there is one
		if (num_literal != -1)
			exprs.back() = LNE(num_literal);

		return applyOnSet(exprs, Gecode::min);
	}

protected:
	Gecode::IntVarArray species; // Holds the actual values we are looking for

public:
	SteadySpace(const size_t specie_count, const int max_value) : species(*this, specie_count, 0, max_value) {
		branch(*this, species, Gecode::INT_VAR_SIZE_MIN(), Gecode::INT_VAL_MIN());
	}

	SteadySpace(bool share, SteadySpace & o)
		: Space(share, o) {
		species.update(*this, share, o.species);
	}

	virtual Space *copy(bool share) {
		return new SteadySpace(share, *this);
	}

	vector<int> getSolution() const {
		vector<int> result(species.size(), 0u);

		for (int i = 0; i < species.size(); i++)
			result[i] = species[i].val();

		return result;
	}

	void print() const {
		for (int i = 0; i < species.size(); i++)
			cout << species[i].val() << ' ';
	}

	/* Constraint the species to the maximal values they can attain. */
	void boundSpecie(const size_t specie_no, const int max_val) {
		Gecode::rel(*this, species[specie_no] <= max_val);
	}

	/* Take the model and turn it into a steady state constraint. */
	void applyModel(const Model & model) {
		for (const size_t i : cscope(model.species)) {
			const Specie & specie = model.species[i];

			vector<string> clauses;
			boost::split(clauses, specie.rule, boost::is_any_of("+"));

			vector<LNE> exprs(clauses.size());
			rng::transform(clauses, exprs.begin(), [&model, &specie, this](const string & clause){return this->convertClause(model, clause, specie.max_val); });
			
			Gecode::rel(*this, species[i] == applyOnSet(exprs, Gecode::max));
		}
	}
};