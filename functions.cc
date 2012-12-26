#include "functions.hh"
#include "ast.hh"
#include "exceptions.hh"

double strict_function::apply(const std::vector<astnode *> &parameters, const environment &env, std::set<cellindex> &evaluation_stack) const {
	std::vector<double> double_parameters;
	for (astnode *node : parameters) {
		double_parameters.push_back(node->evaluate(env, evaluation_stack));
	}
	return this->apply(double_parameters);
}

double plus_function::apply(const std::vector<double> &parameters) const {
	double ret = 0;
	for (double d : parameters) {
		ret += d;
	}
	return ret;
}

double avg_function::apply(const std::vector<double> &parameters) const {
	if (parameters.size() == 0) return 0;

	double ret = 0;
	for (double d : parameters) {
		ret += d;
	}
	return ret / parameters.size();
}

double minus_function::apply(const std::vector<double> &parameters) const {
	if (parameters.size() == 0) { return 1; }
	if (parameters.size() == 1) { return -parameters[0]; }

	double ret = parameters[0];
	for (auto iter = ++parameters.begin(); iter != parameters.end(); ++iter) {
		ret -= *iter;
	}
	return ret;
}

double mul_function::apply(const std::vector<double> &parameters) const {
	double ret = 1;
	for (double d : parameters) {
		ret *= d;
	}
	return ret;
}

double div_function::apply(const std::vector<double> &parameters) const {
	if (parameters.size() == 0) { return 1; }
	if (parameters.size() == 1) { return 1/parameters[0]; }

	double ret = parameters[0];
	for (auto iter = ++parameters.begin(); iter != parameters.end(); ++iter) {
		ret /= *iter;
	}
	return ret;
}

double if_function::apply(const std::vector<astnode *> &parameters, const environment &env, std::set<cellindex> &evaluation_stack) const {
	// We expect three arguments
	if (parameters.size() != 3) {
		throw evaluation_error("if requires 3 parameters");
	}

	// We evaluate the first one.
	double test = parameters[0]->evaluate(env, evaluation_stack);

	// Than we evaluate second or third, depending on the `test` value.
	if (test != 0) {
		return parameters[1]->evaluate(env, evaluation_stack);
	} else {
		return parameters[2]->evaluate(env, evaluation_stack);
	}
}
