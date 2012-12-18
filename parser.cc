/** \file Parser implementaion  for formulas.
 *
 * Here is a grammar for the language accepted by the parser.
 *
 * formula: END   // end of input
 *          expr
 *
 * expr: term + expr
 *       term - expr
 *       term
 *
 * term: prim * term
 *       prim / term
 *       prim
 *
 * prim: NAME ( expr_list )
 *       NAME
 *       NUMBER
 *       ( expr )
 *       - prim
 *
 * See Bjarne Stroustrup - The C++ Programming Language 3rd Edition
 * Chapter 6 - Expressions and Statements
 * starting from section 6.1 A Desk Calculator
 */

#include "parser.hh"
#include "exceptions.hh"

#include <deque>
#include <cassert>

/** Token. */
class token {
public:
	enum type {
		END,
		NUMBER,
		NAME,
		PLUS = '+',
		MINUS = '-',
		MUL = '*',
		DIV = '/',
		LP = '(',
		RP = ')',
		COMMA = ','
	};

	type get_type() const {
		return m_type;
	}

	double get_number() const {
		assert(m_type == NUMBER);
		return m_number;
	}

	std::string get_name() const {
		assert(m_type == NAME);
		return m_name;
	}

	explicit token(type t) : m_type(t) {
		assert(m_type != NAME && m_type != NUMBER);
	}
	explicit token(double value) : m_type(NUMBER), m_number(value) {}
	explicit token(const std::string &value) : m_type(NAME), m_name(value) {}

private:
	type m_type;
	double m_number;
	std::string m_name;
};

/** Tokenize (run lexer) on input stream, change into deque of tokens. */
std::deque<token> tokenize(std::istream &is) {
	std::deque<token> tokenstream;
	double number_value;

	while (is) {
		char ch = 0;
		is >> ch;

		switch (ch) {
		case 0:
			break;

		case '+': case '-':
		case '*': case '/':
		case '(': case ')': case ',':
			tokenstream.push_back(token((token::type) ch));
			break;

		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case '.':
			is.putback(ch);
			is >> number_value;
			tokenstream.push_back(token(number_value));
			break;

		default:
			if (std::isalpha(ch)) {
				std::ostringstream oss;
				oss << ch;
				while (is.get(ch) && std::isalnum(ch)) oss << ch;
				is.putback(ch);
				tokenstream.push_back(token(oss.str()));
			} else {
				throw syntax_error("failed to tokenize");
			}
		}
	}

	tokenstream.push_back(token(token::END));

	return tokenstream;
}

// Forward declaration of parsing functions.
astnode *expr(std::deque<token> &tokenstream, const functionmap &fm);
astnode *term(std::deque<token> &tokenstream, const functionmap &fm);
astnode *prim(std::deque<token> &tokenstream, const functionmap &fm);

astnode *parse(const std::string &input, const functionmap &fm) {
	// first try if it a number
	{
		std::istringstream iss(input);
		double d;
		if (iss >> d && iss.eof()) {
			return new astnode_number(d);
		}
	}

	// it's not formula if not begins with =
	if (input.size() == 0 || input[0] != '=') {
		throw not_formula_error("not formula");
	}

	// parse the formula
	std::istringstream iss(input);
	char c;
	iss >> c; // strip =

	std::deque<token> tokenstream = tokenize(iss);

#if 0
	for (auto &t : tokenstream) {
		std::cerr << t.get_type();
		switch (t.get_type()) {
		case token::NUMBER:
			std::cerr << " NUMBER " << t.get_number();
			break;
		case token::NAME:
			std::cerr << " NAME " << t.get_name();
			break;
		}
		std::cerr << std::endl;
	}
#endif

	return expr(tokenstream, fm);
}

function *findfm(const functionmap &m, const std::string &key) {
	auto iter = m.find(key);
	if (iter == m.end()) {
		throw syntax_error("no function -- " + key);
	}
	return iter->second;
}

astnode *expr(std::deque<token> &tokenstream, const functionmap &fm) {
	astnode *left = term(tokenstream, fm);

	while (true) {
		switch (tokenstream.front().get_type()) {
		case token::PLUS:
			tokenstream.pop_front();
			left = new astnode_call(findfm(fm, "+"), left, term(tokenstream, fm));
			break;
		case token::MINUS:
			tokenstream.pop_front();
			left = new astnode_call(findfm(fm, "-"), left, term(tokenstream, fm));
			break;
		default:
			return left;
		}
	}
}

astnode *term(std::deque<token> &tokenstream, const functionmap &fm) {
	astnode *left = prim(tokenstream, fm);

	while (true) {
		switch (tokenstream.front().get_type()) {
		case token::MUL:
			tokenstream.pop_front();
			left = new astnode_call(findfm(fm, "*"), left, prim(tokenstream, fm));
			break;
		case token::DIV:
			tokenstream.pop_front();
			left = new astnode_call(findfm(fm, "/"), left, prim(tokenstream, fm));
			break;
		default:
			return left;
		}
	}
}

astnode *prim(std::deque<token> &tokenstream, const functionmap &fm) {
	astnode *ret;

	switch (tokenstream.front().get_type()) {
	case token::NUMBER:
		ret = new astnode_number(tokenstream.front().get_number());
		tokenstream.pop_front();
		return ret;
	case token::NAME:
	{
		token t = tokenstream.front();
		tokenstream.pop_front();
		std::string string_value = t.get_name();

		if (tokenstream.front().get_type() != token::LP) {
			ret = new astnode_cell(cellindex(string_value));
		} else {
			tokenstream.pop_front();  // eat (
			std::vector<astnode *> rands;
			if (tokenstream.front().get_type() == token::RP) {
				tokenstream.pop_front(); // eat )

			} else {
				rands.push_back(expr(tokenstream, fm));
				while (tokenstream.front().get_type() == token::COMMA) {
					tokenstream.pop_front();
					rands.push_back(expr(tokenstream, fm));
				}
				if (tokenstream.front().get_type() != token::RP) {
					throw syntax_error("cannot parse, no matching closing parenhece");
				}
				tokenstream.pop_front(); // eat )
			}
			ret = new astnode_call(findfm(fm, string_value), rands);
		}
		return ret;
	}
	case token::LP:
		tokenstream.pop_front();
		ret = expr(tokenstream, fm);
		if (tokenstream.front().get_type() != token::RP) {
			throw syntax_error("cannot parse, no matching closing paren");
		}
		tokenstream.pop_front(); // eat )
		return ret;
	default:
		throw syntax_error("Cannot parse formula");
	}
}
