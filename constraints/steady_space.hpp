#include "../general/definitions.hpp"
#include "basic_space.hpp"

#pragma once

class SteadySpace : public Gecode::Space, public BasicSpace<int> {
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

	void boundSpecie(const size_t specie_no, const int max_val) {
		Gecode::rel(*this, species[specie_no] <= max_val);
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
};