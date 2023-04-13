#include "Parser.h"



IntNumber::IntNumber(const std::string& val) : Node(val)
{
	m_value = std::stoi(value);
}
FloatNumber::FloatNumber(const std::string& val) : Node(val)
{
	m_value = std::stof(value);
}
Identifier::Identifier(const std::string& val) : Node(val)
{
}
BinOp::BinOp(const std::string& val, Node* left, Node* right) : Node(val) , m_left(left), m_right(right) {}

float IntNumber::calc()
{
	return m_value;
}
float FloatNumber::calc() {
	return m_value;
}
float Identifier::calc() {
	if (!floatNameTable.contains(value)) 
		throw "Undefined identifier!";
	return floatNameTable.at(value);
}
float BinOp::calc() {
	float rCalc = 0;
	switch (value.at(0)) {
	case '+':
		return m_left->calc() + m_right->calc();
	case '*':
		return m_left->calc() * m_right->calc();
	case '-':
		return m_left->calc() - m_right->calc();
	case '/':
		rCalc = m_right->calc();
		if (rCalc == 0) throw "Division by zero!";
		return m_left->calc() / rCalc;
	default:
		return 0;
	}
}

void BinOp::print()
{
	std::cout << value << "(";
	m_left->print();
	std::cout << ",";
	m_right->print();
	std::cout << ")";

}

Parser::Parser(const vector<Token>& tokens) : m_tokens(tokens), m_currentToken(0) {}
Node* Parser::factor() {
	if (m_tokens.at(m_currentToken).mLexemType == "float") {
		std::string num = m_tokens.at(m_currentToken).mLexem;
		nextToken();
		return new FloatNumber(num);
	}
	else if (m_tokens.at(m_currentToken).mLexemType == "integer") {
		std::string num = m_tokens.at(m_currentToken).mLexem;
		nextToken();
		return new IntNumber(num);
	}
	else if (m_tokens.at(m_currentToken).mLexemType == "Identifier") {
		std::string id = m_tokens.at(m_currentToken).mLexem;
		nextToken();
		return new Identifier(id);
	}
	else if (m_tokens.at(m_currentToken).mLexem[0] == '(') {
		nextToken();
		auto e = expression();
		if (m_tokens.at(m_currentToken).mLexem[0] != ')')
			throw "No ) bracket at pos " + m_currentToken;
		nextToken();
		return e;
	}
	else  if (m_tokens.at(m_currentToken).mLexem[0] == '-') {
		Node* left = new IntNumber("-1");
		nextToken();
		left = new BinOp("*", left, term());
		return left;
	}
}

Node* Parser::term() {
	auto left = factor();
	std::string op = m_tokens.at(m_currentToken).mLexem;
	while (tOps.contains(op)) {
		nextToken();
		left = new BinOp(op, left, term());
		op = m_tokens.at(m_currentToken).mLexem;
	}
	return left;
}

Node* Parser::expression() {
	auto left = term();
	std::string op = m_tokens.at(m_currentToken).mLexem;
	while (fOps.contains(op)) {
		nextToken();
		left = new BinOp(op, left, term());
		op = m_tokens.at(m_currentToken).mLexem;
	}
	return left;
}

void Parser::nextToken()
{
	if (m_currentToken >= m_tokens.size())
		throw "Length exception";
	++m_currentToken;
}
