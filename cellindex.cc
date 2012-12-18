#include <stack>
#include <cctype>
#include <stdexcept>

#include <iostream>
#include <sstream>

#include "cellindex.hh"

// accepts only columns A..Z
static std::pair<unsigned int, unsigned int> parsecellindex(const std::string &s) {
	if (s.size() < 2) throw std::runtime_error("invalid cell index -- too short");

	bool first = true;

	unsigned int col = 0;
	unsigned int row = 0;

	for (char c : s) {
		if (first) {
			if (!std::isupper(c)) throw std::runtime_error("invalid cell index -- doesnt start with upper character -- " + s);
			col = c - 'A';
			first = false;
		} else {
			if (!std::isdigit(c)) throw std::runtime_error("invalid cell index -- rest is not digits -- " + s);
			row = row * 10 + c - '0';
		}
	}

	return std::make_pair(col, row - 1);
}

cellindex::cellindex(const std::string &s) : col(parsecellindex(s).first), row(parsecellindex(s).second) {}
cellindex::cellindex(const char *s) : col(parsecellindex(s).first), row(parsecellindex(s).second) {}

/** Conversion from cell name to pair of integers is tricky, especially the column part.
 * We cannot apply base conversion algorithm, because textual representation is not positional.
 * Eg. `A1` means (0, 0) especially `A -> 1`, but in `BA1` `A -> 0` and `B -> 2` (not 1).
 *
 * \url http://en.wikipedia.org/wiki/Positional_notation#Base_conversion
 */
std::ostream &operator<< (std::ostream &os, const cellindex &i) {
	std::stack<int> colchars;
	unsigned int col = i.col;

	if (col == 0) {
		colchars.push(0);
	}

	while (col > 0) {
		colchars.push(col % 26);
		col /= 26;
	}

	if (colchars.size() == 1) {
		os << (char)(colchars.top() + 'A');
	} else {
		os << (char)(colchars.top() + 'A' - 1);
		colchars.pop();

		while (!colchars.empty()) {
			os << (char)(colchars.top() + 'A');
			colchars.pop();
		}
	}

	return os << (i.row + 1);
}
