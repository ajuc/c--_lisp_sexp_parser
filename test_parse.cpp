#include "lispparser.h"
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
	std::string tmp(argv[1]);
	std::istringstream input(tmp);
	const std::shared_ptr<const AjucLispParser::Expression> e = AjucLispParser::parse(input);
	std::cout << e->toString() << std::endl;
	return 0;
}
