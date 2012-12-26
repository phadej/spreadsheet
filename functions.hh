/** \file Function objects. */

#ifndef FUNCTIONS_HH
#define FUNCTIONS_HH

#include <string>
#include <vector>
#include <set>

#include "definitions.hh"
#include "exceptions.hh"
#include "cellindex.hh"
#include "table.hh"

/** Parent class for all functions, and also operators. */
class function {
public:
	function(const std::string &name) : m_name(name) {}
	virtual ~function() {}

	/** Apply function, lazy application. */
	virtual double apply(const std::vector<astnode *> &parameters, const environment &env, std::set<cellindex> &evaluation_stack) const = 0;

	/** Return the name of the function. */
	const std::string &str() const {
		return m_name;
	}
private:
	std::string m_name;
};

/** Strict function object. 
 * Almost all functions are strict on all parameters.
 * But for example `IF` is really strict only on first parameter,
 * so eg A1=IF(0,A1,2) will evaluate to 2 if `IF` is lazy.
 * On other hand, if `IF` would be defined as strict function, the example above
 * should fail with circular reference error.
 */
class strict_function : public function {
public:
	strict_function(const std::string &name) : function(name) {}

	/** Evaluates parameters to vector of doubles and applies function on that vector. */
	double apply(const std::vector<astnode *> &parameters, const environment &env, std::set<cellindex> &evaluation_stack) const;

	/** Apply function on vector of doubles. */
	virtual double apply(const std::vector<double> &parameters) const = 0;
};

/** Plus operator, addition ie SUM */
class plus_function : public strict_function {
public:
	plus_function() : strict_function("+") {}
	double apply(const std::vector<double> &parameters) const;
};

/** Substitution. */
class minus_function : public strict_function {
public:
	minus_function() : strict_function("-") {}
	double apply(const std::vector<double> &parameters) const;
};

/** Multiplication ie PRODUCT. */
class mul_function : public strict_function {
public:
	mul_function() : strict_function("*") {}
	double apply(const std::vector<double> &parameters) const;
};

/** Division. */
class div_function : public strict_function {
public:
	div_function() : strict_function("/") {}
	double apply(const std::vector<double> &parameters) const;
};

/** Average ie AVG. */
class avg_function : public strict_function {
public:
	avg_function() : strict_function("avg") {}
	double apply(const std::vector<double> &parameters) const;
};

/** Conditional ie IF. */
class if_function : public function {
public:
	if_function() : function("if") {}
	double apply(const std::vector<astnode *> &parameters, const environment &s, std::set<cellindex> &evaluation_stack) const;
};

/** Template to lift c++ functions into spreadsheet. They are strict as c++ is strict anyways. */
class lifted_unary_function : public strict_function {
public:
	lifted_unary_function(const std::string &name, double (*f)(double)) : strict_function(name), m_f(f) {}
	double apply(const std::vector<double> &parameters) const {
		// We expect three arguments
		if (parameters.size() != 1) {
			throw evaluation_error(str() + " requires 1 parameter");
		}

		return m_f(parameters[0]);
	}
protected:
	double (*m_f)(double);
};

#endif
