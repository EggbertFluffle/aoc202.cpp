#include <algorithm>
#include <cctype>
#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../utils/EggUtils.h"

std::string extract_parenthesis(std::string& str, std::size_t start = 0) {
	std::string out;
	if(str.at(start) != '(') return "";
	uint open = 0;
	for(std::size_t i = start; i < str.size(); i++) {
		switch (str.at(i)) {
			case '(':
				open++;
				break;
			case ')':
				open--;
				break;
		}
		if(open == 0) {
			out = str.substr(start + 1, i - start - 1);
			str.erase(start, i - start + 1);
			break;
		}
	}
	return out;
}

int extract_number(std::string& str, std::size_t start = 0) {
	if(str.empty()) throw std::invalid_argument("string is empty");
	if(start >= str.size()) throw std::out_of_range("start is out of bounds");
	std::string out;

	if(str.size() == 1) {
		out = str.substr(0, 1);
		str.erase(0, 1);
		return std::stoi(out);
	};

	for(std::string::iterator it = str.begin() + start; it != str.end(); it++) {
		if(!std::isalnum(*it)) {
			out = std::string(str.begin() + start, it);
			str.erase(str.begin() + start, it);
			break;
		}
	}
	return std::stoi(out.c_str());
}

class Expression {
	std::string origional;

	std::vector<Expression> exprs;
	std::vector<char> ops;

	bool isConstant;
	uint64_t constant;

public:
	Expression(uint64_t c) : 
		isConstant(true),
		constant(c) {}

	Expression(std::string e) :
		origional(e),
		exprs(),
		ops(),
		isConstant(false),
		constant(0)
	{
		while(e.size() != 0) {
			switch(e.at(0)) {
				case '+':
					ops.emplace_back('+');
					e.erase(e.begin());
					break;
				case '*':
					ops.emplace_back('*');
					e.erase(e.begin());
					break;
				case '(':
					exprs.emplace_back(Expression(extract_parenthesis(e)));
					break;
				default:
					exprs.emplace_back(Expression(extract_number(e)));
					break;
			}
		}
	}

	uint64_t evaluate() {
		if(isConstant) {
			return constant;
		}

		while(exprs.size() > 1) {
			std::size_t pos = 0;
			bool plusFound = false;

			for(std::size_t i = 0; i < ops.size(); i++) {
				if(ops.at(i) == '+') {
					plusFound = true;
					pos = i;
					break;
				}
			}

			if(!plusFound) {
				for(std::size_t i = 0; i < ops.size(); i++) {
					if(ops.at(i) == '*') {
						pos = i;
						break;
					}
				}
			}

			std::cout << "pos: " << pos << "\n";

			switch(ops.at(pos)) {
				case '+':
					exprs.at(pos) = Expression(exprs.at(pos).evaluate() + exprs.at(pos + 1).evaluate());
					break;
				case '*':
					exprs.at(pos) = Expression(exprs.at(pos).evaluate() * exprs.at(pos + 1).evaluate());
					break;
			}
			exprs.erase(exprs.begin() + pos + 1);
			ops.erase(ops.begin() + pos);
		}
		return exprs.front().evaluate();
	}
};



int main() {
	std::vector<std::string> input = egg::read_input_file("input.txt");
	// std::vector<std::string> input = egg::read_input_file("testInput.txt");

	uint64_t sum = 0;
	for(std::string exp: input) {
		sum += Expression(exp).evaluate();
	}
	std::cout << "SUM: " << sum << '\n';

	return 0;
}
