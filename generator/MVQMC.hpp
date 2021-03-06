/*
 * Copyright (c) 2014 - Adam Streck 
 * MuSyCoS - Multivalued Synchronous Networks Constraint Solver 
 * MuSyCoS is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3. 
 * MuSyCoS is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>. 
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> 
 */
#pragma once
#include "generator_defs.hpp"

namespace MVQMC {
	/* Return a vector of combined values, if possible, otherwise return an empty vector. */
	PMin combine(PMin & a, PMin & b) {
		PMin result;

		vector<bool> different(a.size());
		rng::transform(a, b, different.begin(), not_equal_to<vector<int> >()); // Get true if the vectors of values are equal
		const size_t diff_count = rng::count(different, true);
		if (diff_count == 0) {
			throw runtime_error("Trying to merge duplicated vectors.");
		}
		if (diff_count == 1) {
			result = a;
			size_t diff_position = distance(different.begin(), rng::find(different, true));
			rng::set_difference(b[diff_position], a[diff_position], inserter(result[diff_position], result[diff_position].end()));
			rng::sort(result[diff_position]);
		}

		return result;
	}

	/* Return a vector of combinations of the next order. Those who were not possible to merge are left in the reference to input. */
	vector<PMin> next_merge(vector<PMin> & original) {
		vector<PMin> compacted;
		vector<bool> success(original.size(), false);

		for (auto it1 = original.begin(); it1 != original.end(); it1++) {
			for (auto it2 = it1 + 1; it2 != original.end(); it2++) {
				PMin combined = combine(*it1, *it2);
				if (!combined.empty()) {
					if (rng::find(compacted, (combined)) == compacted.end())
						compacted.emplace_back(combined);
					success[distance(original.begin(), it1)] = true;
					success[distance(original.begin(), it2)] = true;
				}
			}
		}

		// Replace the original vector with the leftovers
		vector<PMin> leftovers;
		for (const size_t i : cscope(original))
			if (!success[i])
				leftovers.emplace_back(original[i]);
		original.resize(leftovers.size());
		rng::copy(leftovers, original.begin());

		return compacted;
	}

	/* Create representation of the vector with don't cares. */
	vector<PMin> compactize(vector<vector<int> > original) {
		vector<PMin> result;

		// Convert input
		vector<PMin> current(original.size());
		rng::transform(original, current.begin(), [](const vector<int> & member) {
			PMin result;
			for (const int i : member) result.emplace_back(PLit{ i });
			return result;
		});

		return current;

		// Make compact
		vector<PMin> compacted;
		do {
			compacted = next_merge(current);
			rng::copy(current, inserter(result, result.end()));
			current = compacted;
		} while (!compacted.empty());

		return result;
	}
}