#include <iostream>
#include <fstream>

#include "Token.h"
#include "DFA.h"
#include "Parser.h"

int main() {
	set<char> alphabet = getAlphabet();
	delta rules = makeRules();
	set<State> endStates;
	endStates.insert(State::ENDOFFILE);
	DFA forMath(alphabet, rules, State::NONE, endStates);
	std::string mathing(" --1  +   5    -   _abc    ");  //_abc =-25.f
	vector<Token> programTokens = forMath.getTokensFromString(mathing);
	std::cout << " | LEXEM           | TYPE            | ROW             | COLUMN          | \n";
	for (auto& token : programTokens)
		std::cout << token << '\n';
	
	Parser myParser(programTokens);
	BinOp* start = static_cast<BinOp*>(myParser.expression());
	start->print();
	std::cout <<"\n calc:" << start->calc();
	delete start;
	return 0;
}