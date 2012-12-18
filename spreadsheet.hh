/** \file Spreadsheet class. */

#ifndef SPREADSHEET_HH
#define SPREADSHEET_HH

#include <set>

#include "table.hh"
#include "ast.hh"

/** Class encapsulating almost all spreadsheet actions.
 * You need to provide functionmap with function used in the spreadsheet.
 *
 * \sa function
 */
class spreadsheet {
public:
	spreadsheet(const functionmap &fm) : m_function_map(fm) {}

	~spreadsheet() {
		for (auto &p : asts) {
			delete p.second;
		}
	}

	/** Set cell value. */
	void set(const cellindex &i, const std::string &s);

	/** Get non evaluated cell value. */
	std::string get(const cellindex &i) const;

	/** Get evaluated cell value. */
	std::string evaluate(const cellindex &i) const;

	/** Clear cell value. */
	void erase(const cellindex &i);

	/** Get indexes of all non empty cells. */
	std::set<cellindex> non_empty_cells() const;

private:
	spreadsheet(const spreadsheet &);
	spreadsheet &operator=(const spreadsheet &);

	table<std::string> inputs;
	table<astnode *> asts;

	/** cells with syntax error in formula, second part is error message */
	std::map<cellindex, std::string> syntax_errors;

	const functionmap &m_function_map;
};

#endif
