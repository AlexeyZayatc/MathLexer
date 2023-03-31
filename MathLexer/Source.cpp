#include <iostream>
#include <fstream>

#include "Token.h"
#include "DFA.h"

int main() {
	set<char> alphabet = getAlphabet();
	delta rules = makeRules();
	set<State> endStates;
	endStates.insert(State::ENDOFFILE);
	DFA forMath(alphabet, rules, State::NONE, endStates);
	std::string mathing("(_abc  +  bcd)  *  25 / 10  +5.5 ");
	vector<Token> programTokens = forMath.getTokensFromString(mathing);
	std::cout << " | LEXEM           | TYPE            | ROW             | COLUMN          | \n";
	for (auto& token : programTokens)
		std::cout << token << '\n';
	
	return 0;
}