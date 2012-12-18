/** \file Parser for tradicional expressions. */

#ifndef PARSER_HH
#define PARSER_HH

#include "definitions.hh"
#include "ast.hh"

/** Parse string into astnode tree.
 *
 * @throw not_formula_error
 * @throw syntax_error
 */
astnode *parse(const std::string &, const functionmap &);

#endif
