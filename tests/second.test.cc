#include "spreadsheet.hh"
#include "functions.hh"

#include <iostream>

void test() {
	functionmap functions;
	functions["+"] = new plus_function();
	functions["-"] = new minus_function();
	functions["*"] = new mul_function();
	functions["/"] = new div_function();
	functions["SUM"] = new plus_function();
	functions["AVG"] = new avg_function();

	spreadsheet s(functions);

	s.set("A2", "A2");
	s.set("C11","C11");
	s.set("Z1", "Z1");

	s.set("B1", "=1+2*3");
	s.set("B2", "=B1 + 3");
	s.set("B3", "=B1 * B2");
	s.set("B4", "=SUM(B1, B2, B3)");
	s.set("B5", "=AVG(B2, 10 + 2 * 5, SUM(B2, 0))");
	s.set("B6", "3");

	s.set("C1", "=C2");

	s.set("D1", "=B1 + ");
	s.set("D2", "=FOO(1,1)");
	s.set("D3", "=#");
	s.set("D4", "=SUM(1,1");
	s.set("D5", "=(1+");
	s.set("D6", "=+");

	std::cout << "FIRST" << std::endl;
	for (const cellindex &i : s.non_empty_cells()) {
		std::cout << i << ": " << s.evaluate(i) << std::endl;
	}
	std::cout << std::endl;

	s.erase("D1");
	s.erase("D2");
	s.erase("D3");
	s.erase("D4");
	s.erase("D5");
	s.erase("D6");
	s.erase("D7");
	s.erase("Z1");
	s.erase("C1");

	std::cout << "SECOND" << std::endl;
	for (const cellindex &i : s.non_empty_cells()) {
		std::cout << i << ": " << s.evaluate(i) << std::endl;
	}

	for (auto &p : functions) {
		delete p.second;
	}
}

int main() {
	try {
		test();
	} catch (const std::exception &e) {
		std::cout << "FATAL: " << e.what() << std::endl;
		return 1;
	} catch (...) {
		std::cout << "CATCHED SOMETHING" << std::endl;
		return 1;
	}
}
