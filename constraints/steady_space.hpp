#include "../model/model_parsers.hpp"
#include "basic_space.hpp"

#pragma once

class SteadySpace : public Gecode::Space, public BasicSpace<int> {
protected:
	Gecode::IntVarArray spec_vars; // Holds the actual values we are looking for

public:
	SteadySpace(const size_t specie_count, const int max_value) : spec_vars(*this, specie_count, 0, max_value) {
		branch(*this, spec_vars, Gecode::INT_VAR_SIZE_MIN(), Gecode::INT_VAL_MIN());
	}

	SteadySpace(bool share, SteadySpace & o)
		: Space(share, o) {
		spec_vars.update(*this, share, o.spec_vars);
	}

	virtual Space *copy(bool share) {
		return new SteadySpace(share, *this);
	}

	vector<int> getSolution() const {
		vector<int> result(spec_vars.size(), 0u);

		for (int i = 0; i < spec_vars.size(); i++)
			result[i] = spec_vars[i].val();

		return result;
	}

	void print() const {
		for (int i = 0; i < spec_vars.size(); i++)
			cout << spec_vars[i].val() << ' ';
	}

	/* Constraint the species to the maximal values they can attain. */
	void boundSpecie(const size_t specie_no, const int max_val) {
		Gecode::rel(*this, spec_vars[specie_no] <= max_val);
	}

	/* Take the model and turn it into a steady state constraint. */
	void applyModel(const Model & model) {
		for (const size_t i : cscope(model.species)) {		
			Gecode::rel(*this, spec_vars[i] == convertRule(spec_vars, model, model.species[i]));
		}
	}
};