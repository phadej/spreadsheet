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
	functions["IF"] = new if_function();

	spreadsheet s(functions);

	s.set("A1", "=A2");
	s.set("A2", "=A1");

	s.set("B1", "=B2+B3");
	s.set("B2", "5");
	s.set("B3", "=SUM(B1,B2)");

	s.set("C1", "=C1");

	s.set("D1", "=IF(0,D1,1)");
	s.set("D2", "=IF(0,2,D2)");
	s.set("D3", "=IF(1,3,D3)");
	s.set("D4", "=IF(1,D4,4)");
	s.set("D5", "=IF(0,5,6)");
	s.set("D6", "=IF(1,7,8)");

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
