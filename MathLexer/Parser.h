#pragma once
#include <map>
#include <set>
#include <vector>

#include "Token.h"

const std::map < std::string, float > floatNameTable = { {"_abc", -25.f}, {"bcd", -30.f}, {"i", 30}};


 struct Node {
	 Node(const std::string& val) {
		 value = val;
	 }
	 virtual void print() { std::cout << value; };
	 std::string value;
	 virtual float calc() = 0;
};


 //read about CRTP idiom
 // classes EXPRESSION/TERM/FACTOR?
 // idk <)DFsdagf
 class IntNumber : public Node {
 public:
	 IntNumber(const std::string& val);
	 virtual float calc() override final;
 private:
	 int m_value;
};

 class FloatNumber : public Node {
 public:
	 FloatNumber(const std::string& val);
	 virtual float calc() override final;
 private:
	 float m_value;

 };

 class Identifier : public Node {
 public:
	 Identifier(const std::string& val);
	 virtual  float calc() override final;
 };

 class BinOp : public Node {
 public:
	 BinOp(const std::string& val, Node* left, Node* right);
	 virtual float calc() override final;
	 virtual void print() override final;
 private:
	 Node* m_left;
	 Node* m_right;
 };


class Parser
{
public:
	Parser(const vector<Token>& tokens);
	Node* term();
	Node* factor();
	Node* expression();
	void nextToken();
private:
	vector<Token> m_tokens;
	size_t m_currentToken;
	const set<std::string> fOps{ "+","-"};
	const set<std::string> tOps{"*","/"};
};

