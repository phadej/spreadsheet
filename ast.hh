/** \file Abstract syntax tree nodes and evaluation of them.
 *
 * Evaluation can be separated into other file thought.
 */

#ifndef AST_HH
#define AST_HH

#include "definitions.hh"
#include "table.hh"

#include <ostream>
#include <set>
#include <vector>

/** Evaluate astnode down to the double value */
double evaluate(const astnode *node, const table<astnode *> &s);

/** Parent class for all AST nodes. */
class astnode {
public:
	/** (S-expression) string representation of the node. */
	std::string str() {
		std::ostringstream oss;
		write(oss);
		return oss.str();
	}

	/** Write node to output stream.
	 *
	 * \sa astnode::str
	 */
	virtual std::ostream &write(std::ostream &os) const = 0;

	/** Evaluate node. */
	virtual double evaluate(const environment &env, std::set<cellindex> &evaluation_stack) const = 0;

	// Virtual destructor!
	virtual ~astnode() {}
};

/** Evaluation environment. Abstracts table of `astnode`. 
 *
 * \see astnode
 */
class environment {
public:
	/** Takes table of `astnode`. This might change.
	 *
	 * \sa astnode
	 */
	environment(const table<astnode *> &s) : s(s) {}

	/** Search for the cell in environment and evaluate it. */
	double find(const cellindex &index, std::set<cellindex> &evaluation_stack) const;

private:
	const table<astnode *> &s;
};

/** Scalar number eg 0 or 1. */
class astnode_number : public astnode {
public:
	astnode_number(double value) : value(value) {}
	std::ostream &write(std::ostream &os) const {
		return os << value;
	}
	double evaluate(const environment &env, std::set<cellindex> &evaluation_stack) const { return value; }
private:
	double value;
};

/** Cell node eg A1 or B2. */
class astnode_cell : public astnode {
public:
	astnode_cell(const cellindex &index) : index(index) {}
	std::ostream &write(std::ostream &os) const {
		return os << index;
	}
	double evaluate(const environment &env, std::set<cellindex> &evaluation_stack) const;
private:
	cellindex index;
};

/** The only compound ast node, the function (or operator) call.
 * Both operations (1 + 1) and function calls (SUM(1, 1)) are represented by this.
 * We could have separate node types for different operations, but in this way
 * structure is more light.
 */
class astnode_call : public astnode {
public:
	astnode_call(function *f, const std::vector<astnode *> &rands) : m_function(f), m_parameters(rands) {}
	astnode_call(function *f, astnode *left, astnode *right) : m_function(f) {
		m_parameters.push_back(left);
		m_parameters.push_back(right);
	}
	~astnode_call() {
		for (auto &parameter : m_parameters) {
			delete parameter;
		}
	}
	std::ostream &write(std::ostream &os) const;
	double evaluate(const environment &env, std::set<cellindex> &evaluation_stack) const;
private:
	function *m_function;
	std::vector<astnode *> m_parameters;
};

#endif
