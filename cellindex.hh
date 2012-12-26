/** \file Cell index. */

#ifndef CELLINDEX_HH
#define CELLINDEX_HH

#include <string>

/** Representing cell index. Pair of integers with few helper functions. 
 * Members are const, so this structure is non-mutable.
 */
struct cellindex {
	/** Constructor with numeric col and row. Not used commonly */
	cellindex(unsigned int col, unsigned int row) : col(col), row(row) {}

	/** Constructor is not explicit, so `std::string` can be implicitly converted to cellindex. */
	cellindex(const std::string &);

	/** Constructor is not explicit, so "B1" can be implicitly converted to cellindex. */
	cellindex(const char *);

	/** Less-than operator, so we can use cellindex as key for set or map. */
	bool operator<(const cellindex &other) const {
		if (col == other.col) {
			return row < other.row;
		}

		return col < other.col;
	}

	const unsigned int col;
	const unsigned int row;
};

std::ostream &operator<< (std::ostream &os, const cellindex &i);

#endif
