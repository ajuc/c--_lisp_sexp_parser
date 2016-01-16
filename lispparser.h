#ifndef AJUC_LISP_PARSER_LISP_PARSER_H
#define AJUC_LISP_PARSER_LISP_PARSER_H

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <memory>

#define SYNTAX_ERROR(line_no, x) { std::cerr << (line_no) << ": " <<  x << std::endl; exit(-1); }

/**
 * Using this library is simple:
 *	the only function useful for end user is parse(input stream)
 *	
 */
namespace AjucLispParser {

class Expression {
    public:
	Expression();
	virtual ~Expression();
	virtual const std::string toString() const = 0;
    private:
	
};

class Float : public Expression {
    public:
	Float(const std::string& text);
	~Float();
	const std::string toString() const;
	const float value() const;
    private:
	float _value;
};


class String : public Expression {
    public:
	String(const std::string& text);
	~String();
	const std::string toString() const;
	const std::string value() const;
    private:
	std::string _value;
};

class Identifier : public Expression {
    public:
	Identifier(const std::string& text);
	~Identifier();
	const std::string toString() const;
	const std::string name() const;
    private:
	std::string _name;
};

class Atom : public Expression {
    public:
	Atom(const std::string& text);
	~Atom();
	const std::string toString() const;
	const std::string name() const;
    private:
	std::string _name;
};

class List : public Expression {
    public:
	List();
	List(std::vector< std::shared_ptr< Expression> > elements);
	~List();
	const std::string toString() const;
	std::shared_ptr<Expression> first() const;
	std::shared_ptr<Expression> item(const int index) const;
	const int size() const;
	bool empty() const;
    private:
	std::vector< std::shared_ptr< Expression> > elements;
};

/** Parses input into expression. */
std::shared_ptr<Expression> parse(std::istringstream& input, int& line_no, bool topLevel);
std::shared_ptr<Expression> parse(std::istringstream& input);

};

#endif /*AJUC_LISP_PARSER_LISP_PARSER_H*/
