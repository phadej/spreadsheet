/** \file Exceptions. */

#ifndef EXCEPTIONS_HH
#define EXCEPTIONS_HH

#include <stdexcept>

/** Errors during evaluation eg not found cell. */
struct evaluation_error : public std::runtime_error {
	evaluation_error(const std::string &what) : std::runtime_error(what) {}
};

/** Indicates that string is not a formula.
 * 
 * Thrown when string does not look like formula.
 */
struct not_formula_error : public std::runtime_error {
	not_formula_error(const std::string &what) : std::runtime_error(what) {}
};

/** Generic syntax error in formula parsing. */
struct syntax_error : public std::runtime_error {
	syntax_error(const std::string &what) : std::runtime_error(what) {}
};

#endif
