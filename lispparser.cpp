#include "lispparser.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>

namespace AjucLispParser {

Expression::Expression() {
	//nothing to do
};
Expression::~Expression() {
	//nothing to do
};

Float::Float(const std::string& text) {
	std::istringstream ss(text);
	float f;
	ss >> f;
	(this->_value) = f;
}
Float::~Float() {
}
const std::string Float::toString() const {
	std::ostringstream ss;
	ss << this->_value;
	return ss.str();
}
const float Float::value() const {
	return this->_value;
}

String::String(const std::string& text) {
	this->_value = text;
}
String::~String() {
}
const std::string String::toString() const {
	return this->_value;
}
const std::string String::value() const {
	return this->_value;
}


Identifier::Identifier(const std::string& text) {
	this->_name = text;
}
Identifier::~Identifier() {
}
const std::string Identifier::toString() const {
	return this->_name;
}
const std::string Identifier::name() const {
	return this->_name;
}

Atom::Atom(const std::string& text) {
	this->_name = text;
}
Atom::~Atom() {
}
const std::string Atom::toString() const {
	return ":" + this->_name;
}
const std::string Atom::name() const {
	return this->_name;
}


List::List() {
}
List::List(const std::vector<const Expression* > elements) {
	this->elements = elements;
}
List::~List() {
}
const std::string List::toString() const {
	std::string result("( ");
	for (int i=0; i<elements.size(); i++) {
		if (i>0) {
			result += ", ";
		}
		result = result + elements[i]->toString();
	}
	result += " )";
	return result;
}
const Expression* List::first() const {
	return elements[0];
}
const Expression* List::item(const int index) const {
	return elements[index];
}
const int List::size() const {
	return elements.size();
}
bool List::empty() const {
	return size()==0;
}

bool whitespace(char c) {
	return c == ' ' || c == '\n' ||
		c == '\t' || c == ',';
}

bool digit(char c) {
	return c >= '0' && c <= '9';
}

bool openParenthenesis(char c) {
	return c == '('||
		   c == '['||
		   c == '{';
}

bool closeParenthenesis(char c) {
	return c == ')' ||
		   c == ']' ||
		   c == '}';
}

bool parenthenesis(char c) {
	return openParenthenesis(c) || closeParenthenesis(c);
}

bool parenMatches(char opening, char closing) {
	return	(opening == '(' && closing == ')') ||
		(opening == '[' && closing == ']') ||
		(opening == '{' && closing == '}');
}

bool alphanumeric(char c) {
	return !whitespace(c) && !parenthenesis(c);
}

bool doubleQuote(char c) {
	return c == '"';
}

bool colon(char c) {
	return c == ':';
}

bool dotIn(const std::string& buffer) {
	return buffer.find(".", 0) != std::string::npos;
}

bool dot(char c) {
	return c == '.';
}
const Expression* parse(std::istringstream& input) {
	int line_no = 1;
	return parse(input, line_no, true);
}

const Expression* parse(std::istringstream& input, int& line_no, bool topLevel) {
	std::string buffer("");
	char c;
	
	while (input.good()) {
		input.get(c);
		if (whitespace(c)) {
			if (c == '\n') {
				line_no++;
			}
			if (buffer.empty()) {
				;//nothing to do
			} else {
				if (digit(buffer[0])) {
					return new Float(buffer);
				} else if (doubleQuote(buffer[0])) {
					buffer += c;
				} else if (colon(buffer[0])) {
					return new Atom(buffer.substr(1));
				} else if (alphanumeric(buffer[0])) {
					return new Identifier(buffer);
				//} else {
				//	return new Identifier(buffer);
				}
			}
		} else if (digit(c)) {
			buffer += c;
		} else if (dot(c)) {
			if (buffer.empty()) {
				buffer += c;
			} else {
				if (digit(buffer[0])) {
					if (!dotIn(buffer)) {
						buffer += c;
					} else {
						SYNTAX_ERROR(line_no, "Second dot found in Float literal " << buffer + c );
					}
				} else {
					buffer += c;
				}
			}
		} else if (colon(c)) {
			if (buffer.empty()) {
				buffer += c;
			} else {
				SYNTAX_ERROR(line_no, "Colon found in the middle of token " << buffer + c );
			}
		} else if (doubleQuote(c)) {
			if (buffer.empty()) {
				buffer += c;
			} else {
				if (doubleQuote(buffer[0])) {
					buffer += c;
					return new String(buffer);
				} else {
					SYNTAX_ERROR(line_no, "Double-quote found in the middle of token " << buffer + c );
				}
			}
		} else if (parenthenesis(c)) { //TODO - uwaga - tu cale zamotanie jest
			if (buffer.empty()) {
				if (openParenthenesis(c)) {
					//zaczyna sie lista
					std::vector<const Expression*> list;
					char c2;
					input.get(c2);
					while (!closeParenthenesis(c2)) {
						input.unget();
						const Expression* tmpExp = parse(input, line_no, false);
						if (tmpExp) {
						    list.push_back(tmpExp);
						} else {
						    input.unget();
						}
						input.get(c2);
					}
					if (!parenMatches(c, c2)) {
						SYNTAX_ERROR(line_no, "Parenthenesis " << c << " is closed by not matching paren : " << c2);
					}
					return new List(list);
				} else {//closeParenthenesis(c)
					if (topLevel) {
					    SYNTAX_ERROR(line_no, "Found unmatched close parenthenesis " << c);
					} else {
					    return NULL;//oznacza, ze trzeba wyjsc
					}
				}
			} else {
				//koniec tokena jakiegos
				//cofnijmy c do strumienia, i zwrocmy expression
				//albo to byl nawias wewnatrz stringa i po prostu go dodajemy
				
				if (digit(buffer[0])) {
					input.unget();
					return new Float(buffer);
				} else if (doubleQuote(buffer[0])) {
					buffer += c; //nie robimy unget, bo to nawias wewnatrz stringa
				} else if (colon(buffer[0])) {
					input.unget();
					return new Atom(buffer);
				} else if (alphanumeric(buffer[0])) {
					input.unget();
					return new Identifier(buffer);
				}  else {
					SYNTAX_ERROR(line_no, "Situation impossible : buffer == " << buffer);
				}
			}
		} else if (alphanumeric(c)) {
			if (buffer.empty()) {
				buffer += c;
			} else {
				if (!digit(buffer[0])) {
					buffer += c;
				} else {
					SYNTAX_ERROR(line_no, "Non-digit found in the middle of Float literal " << buffer + c );
				}
			}
		} else {
			SYNTAX_ERROR(line_no, "Unrecognised char " << c);
		}
	}
}

}