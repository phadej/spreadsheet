#include "ast.hh"
#include "functions.hh"
#include "exceptions.hh"

#include <iostream>

double evaluate(const astnode *node, const table<astnode *> &s) {
	// Create helper structures and evaluate the node.
	std::set<cellindex> evaluation_stack;
	environment env(s);
	return node->evaluate(env, evaluation_stack);
}

/** Helper RAII class for preventing circular references = infinite loops. */
class find_lookup {
public:
	/** In constructor we check that cellindex is not presend in stack, and than insert it into (push). */
	find_lookup(const cellindex &index, std::set<cellindex> &stack) : index(index), stack(stack) {
		if (stack.find(index) != stack.end()) {
			throw evaluation_error("circular reference");
		} else {
			stack.insert(index);
		}
	}

	/** In destructor we remove index from stack (pop). */
	~find_lookup() {
		stack.erase(index);
	}

private:
	const cellindex &index;
	std::set<cellindex> &stack;
};

double environment::find(const cellindex &index, std::set<cellindex> &evaluation_stack) const {
	// raii find lookup, so we cannot forget to remove index from stack.
	find_lookup fl(index, evaluation_stack);

	auto iter = s.find(index);
	if (iter == s.end()) {
		throw evaluation_error("not formula or number cell -- " + to_string(index));
	} else {
		double ret = iter->second->evaluate(s, evaluation_stack);
		return ret;
	}
}

double astnode_cell::evaluate(const environment &env, std::set<cellindex> &evaluation_stack) const {
	return env.find(index, evaluation_stack);
}

double astnode_call::evaluate(const environment &env, std::set<cellindex> &evaluation_stack) const {
	return m_function->apply(m_parameters, env, evaluation_stack);
}

std::ostream &astnode_call::write(std::ostream &os) const {
	os << '(' << m_function->str();
	for (auto &rand : m_parameters) {
		os << ' ';
		rand->write(os);
	}
	os << ')';
	return os;
}
