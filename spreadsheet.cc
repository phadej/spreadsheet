#include "spreadsheet.hh"

#include "parser.hh"
#include "exceptions.hh"

void spreadsheet::set(const cellindex &i, const std::string &s) {
	erase(i);

	// Insert into inputs
	inputs.set(i, s);

	// Insert parsed astnode into asts
	try {
		asts.set(i, parse(s, m_function_map));
	} catch (const not_formula_error &e) {
		// if not formula, then it's not.
	} catch (const syntax_error &e) {
		syntax_errors.insert(std::make_pair(i, e.what()));
	}
}

std::string spreadsheet::get(const cellindex &i) const {
	auto inputs_iter = inputs.find(i);
	if (inputs_iter == inputs.end()) {
		// no input, return empty
		return "";
	}
	return inputs_iter->second;
}

std::string spreadsheet::evaluate(const cellindex &i) const {
	auto inputs_iter = inputs.find(i);
	if (inputs_iter == inputs.end()) {
		// no input, return empty
		return "";
	}

	auto syntax_errors_iter = syntax_errors.find(i);
	if (syntax_errors_iter != syntax_errors.end()) {
		return std::string("#SYNTAX_ERROR " + syntax_errors_iter->second);
	}

	auto asts_iter = asts.find(i);
	if (asts_iter == asts.end()) {
		// no ast, string value
		return inputs_iter->second;
	}

	try {
		// calling global evaluate!
		return to_string(::evaluate(asts_iter->second, asts));
	} catch (const evaluation_error &e) {
		return std::string("#EVAL_ERROR ") + e.what();
	}
}

void spreadsheet::erase(const cellindex &i) {
	// CLearing syntax error
	syntax_errors.erase(i);

	// Remove old one from asts, if exists
	auto iter = asts.find(i);
	if (iter != asts.end()) {
		delete iter->second;
	}

	// Remove from tables
	inputs.erase(i);
	asts.erase(i);
}

std::set<cellindex> spreadsheet::non_empty_cells() const {
	std::set<cellindex> ret;
	for (auto &p : inputs) {
		ret.insert(p.first);
	}
	return ret;
}
