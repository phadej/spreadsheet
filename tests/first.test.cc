#include "table.hh"
#include "functions.hh"
#include "ast.hh"
#include "parser.hh"
#include "exceptions.hh"

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>

double pi(double) {
	return 3.141592653589793238462643383279;
}

void test() {
	table<std::string> input;

	input.set(cellindex(0,0), "0");
	input.set(1,25, "1");
	input.set(25,1, "2");
	input.set(26,2, "3");
	input.set(51,3, "4");
	input.set(52,4, "5");
	input.set(676,999, "6");

	input.set(cellindex("A2"), "A2");
	input.set("C11","C11");
	input.set("Z1", "Z1");

	input.set("B1", "=1+2*3");
	input.set("B2", "=B1 + 3");
	input.set("B3", "=B1 * B2");
	input.set("B4", "=SUM(B1, B2, B3)");
	input.set("B5", "=AVG(B2, 10 + 2 * 5, SUM(B2, 0))");

	input.set("C1", "=PI(0)");
	input.set("C2", "=SIN(0)");
	input.set("C3", "=COS(0)");
	input.set("C4", "=SIN(PI(0)/2)");
	input.set("C5", "=COS(PI(0)/2)+2");
	input.set("C6", "=SIN(0.5)*SIN(0.5) + COS(0.5)*COS(0.5)");

	input.set("D1", "=3/4");
	input.set("D2", "=3-4");

	std::cout << "INPUT:" << std::endl;
	for (auto &p : input) {
		std::cout << p.first << ": " << p.second << std::endl;
	}
	std::cout << std::endl;

	functionmap functions;
	functions["+"] = new plus_function();
	functions["-"] = new minus_function();
	functions["*"] = new mul_function();
	functions["/"] = new div_function();
	functions["SUM"] = new plus_function();
	functions["AVG"] = new avg_function();

	functions["PI"] = new lifted_unary_function("pi", pi);
	functions["SIN"] = new lifted_unary_function("sin", sin);
	functions["COS"] = new lifted_unary_function("cos", cos);

	table<astnode *> compiled;

	std::cout << "COMPILING:" << std::endl;
	for (auto &p : input) {
		try {
			compiled.set(p.first, parse(p.second, functions));
		} catch (const not_formula_error &e) {
			std::cout << "ERROR compiling " << p.first << " -- " << e.what() << std::endl;
		}
	}
	std::cout << std::endl;

	std::cout << "COMPILED:" << std::endl;
	for (auto &p : compiled) {
		std::cout << p.first << ": " << (p.second == nullptr ? "(null)" : p.second->str()) << std::endl;
	}
	std::cout << std::endl;

	std::cout << "EVALUATED:" << std::endl;
	for (auto &p : compiled) {
		std::cout << p.first << ": " << evaluate(p.second, compiled) << std::endl;
	}
	std::cout << std::endl;

	// CLEANUP
	for (auto &p : functions) {
		delete p.second;
	}

	for (auto &p : compiled) {
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
