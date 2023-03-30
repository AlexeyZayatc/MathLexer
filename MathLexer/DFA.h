#pragma once
#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <streambuf>

#include "Token.h"
using namespace std;
enum class State {
	NONE,
	SYMBOLS,
	NUMBER,
	IDENTIFIER,
	ENDOFFILE,
	ERROR
};

static const map<char, std::string> symbols = {
	{'+',"PLUS"},
	{'-',"MINUS"},
	{'*',"MULTIPLY"},
	{'/',"DIVISION"},
	{'%',"REMAINDER"},
	{'(',"LRBRACKET"},
	{')',"RRBRACKET"},
	{'.', "DECIMALPOINT"},
};
typedef map<State, map<char, State>> delta;


class DFA
{
public:
	DFA(const set<char>& alphabet, const delta& rules, const State& start, const set<State>& endStates);
	vector<Token> getTokensFromString(const std::string& mathString) const ;
private:
	set<char> mAlphabet;
	delta mRules;
	State mStart;
	set<State> mEnd;
};

//making rules) idk how to do it better btw
delta makeRules();
set<char> getAlphabet();