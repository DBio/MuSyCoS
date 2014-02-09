#include "generator_defs.hpp"

namespace MVQMC {
	/* Return a vector of combined values, if possible, otherwise return an empty vector. */
	PMin combine(PMin & a, PMin & b) {
		PMin result;

		vector<bool> different(a.size());
		rng::transform(a, b, different.begin(), equal_to<PMin>()); // Get true if the vectors of values are equal
		const size_t diff_count = rng::count(different, true);
		if (diff_count == 0) {
			throw runtime_error("Trying to merge duplicated vectors.");
		}
		if (diff_count == 1) {
			result = a;
			size_t diff_position = distance(different.begin(), rng::find(different, true));
			rng::set_difference(a[diff_position], b[diff_position], inserter(result, result.end()));
		}

		return result;
	}

	/* Return a vector of combinations of the next order. Those who were not possible to merge are left in the reference to input. */
	vector<PMin> next_merge(vector<PMin> & original) {
		vector<PMin> compacted;
		vector<PMin> leftovers;

		for (auto it1 = original.begin(); it1 != original.end(); it1++) {
			bool success = false;
			for (auto it2 = it1 + 1; it2 != original.end(); it2++) {
				PMin combined = combine(*it1, *it2);
				if (!combined.empty()) {
					success = false;
					compacted.emplace_back(combined);
				}
			}
			if (!success)
				leftovers.emplace_back(*it1);
		}

		original = leftovers;
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

		// Make compact
		vector<PMin> compacted;
		do {
			compacted = next_merge(current);
			rng::copy(current, inserter(result, result.end()));
			current = { compacted.begin(), unique(compacted.begin(), compacted.end()) };
		} while (!compacted.empty());

		return result;
	}
}