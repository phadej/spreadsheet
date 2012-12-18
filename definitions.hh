/** \file Forward declarations and helper definitions. */

#ifndef DEFINITIONS_HH
#define DEFINITIONS_HH

#include <map>
#include <sstream>

// Forward declarations
class function;
class astnode;
class environment;

/// Functions
typedef std::map<std::string, function *> functionmap;

/** Helper that converts any printable value to `std::string`. */
template <typename T>
std::string to_string(const T &t) {
	std::ostringstream oss;
	oss << t;
	return oss.str();
}

#endif
