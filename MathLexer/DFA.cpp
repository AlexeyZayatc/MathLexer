#include "DFA.h"

DFA::DFA(const set<char>& alphabet, const delta& rules, const State& start, const set<State>& endStates)
	: mAlphabet(alphabet), mRules(rules), mStart(start), mEnd(endStates) {}

vector<Token> DFA::getTokensFromString(const std::string& mathString) const
{
	vector<Token> tokens;
	State currentState = mStart;
	State previousState;
	short currentChar;
	int curColumn = 0;
	string lexem;
	auto getNextChar = [&]() {
		if (curColumn < mathString.size())
			currentChar = mathString[curColumn];
		else
			currentChar = -200;
		curColumn += 1;
	};
	auto getTypeOfLexem = [&](const string& lxm) {
		if (lxm.size() == 1 && (symbols.contains(lxm[0])))
			return symbols.at(lxm[0]);
		else {
			if ((lxm[0] >= 'a' && lxm[0] <= 'z') || (lxm[0] >= 'A' && lxm[0] <= 'Z'))
				return std::string("Identifier");
			if (lxm.find('.') != string::npos)
				return std::string("float");
		}
		return std::string("integer");
	};
	while (currentState!=State::ENDOFFILE) {
		previousState = currentState;
		getNextChar();
		if (currentChar>=-128 && currentChar<=127) {
			currentState = mRules.at(currentState).at(currentChar);
		}
		else {
			currentState = State::ENDOFFILE;
		}
		if ((currentState != previousState || currentState == State::SYMBOLS) && !lexem.empty() && currentState!=State::ERROR)
		{
			string typeOfLexem = getTypeOfLexem(lexem);
			tokens.push_back(Token(lexem, typeOfLexem, 0, curColumn));
			lexem.clear();
		}
		if (currentState == State::NONE)
			lexem.clear();
		else
			lexem.push_back(currentChar);
		if (currentState == State::ERROR)
		{
			lexem.clear();
			string errorType;
			switch (previousState) {
			case State::IDENTIFIER:
				errorType = "Error while identifying identifier at pos ";
				break;
			case State::NUMBER:
				errorType = "Error while identifying number at pos ";
				break;
			case State::NONE:
			case State::SYMBOLS:
			default:
				errorType = "Error: unknown symbol at pos ";
				break;
			}
			tokens.push_back(Token("ERROR", errorType, 0, curColumn));
			return tokens;
		}
	}
	Token endOfFile("EOF", "EOF", 0, ++curColumn);
	if (currentState != State::ERROR) tokens.push_back(endOfFile);
	return tokens;
}

delta makeRules()
{
	delta rls;
	map<char, State> forNone;
	map<char, State> forIdentifier;
	map<char, State> forInteger;
	map<char, State> forSymbols;
	//for NONE state
	{
		for (int i = -128; i < 128; i++)
			forNone[char(i)] = State::ERROR;
		for (char ch = 'a'; ch <= 'z'; ch++)
			forNone[ch] = State::IDENTIFIER;
		for (char ch = 'A'; ch <= 'Z'; ch++)
			forNone[ch] = State::IDENTIFIER;
		forNone['_'] = State::IDENTIFIER;
		for (char ch = '0'; ch <= '9'; ch++)
			forNone[ch] = State::NUMBER;
		for (auto& sym : symbols)
			forNone[sym.first] = State::SYMBOLS;
		forNone[' '] = State::NONE;
		forNone['\n'] = State::NONE;
		forNone['\t'] = State::NONE;
		rls[State::NONE] = forNone;
	}
	//for identifier state
	{
		for (int i = -128; i < 128; i++)
			forIdentifier[char(i)] = State::ERROR;
		for (char ch = 'a'; ch <= 'z'; ch++)
			forIdentifier[ch] = State::IDENTIFIER;
		for (char ch = 'A'; ch <= 'Z'; ch++)
			forIdentifier[ch] = State::IDENTIFIER;
		for (char ch = '0'; ch <= '9'; ch++)
			forIdentifier[ch] = State::IDENTIFIER;
		for (auto& sym : symbols)
			forIdentifier[sym.first] = State::SYMBOLS;
		forIdentifier['_'] = State::IDENTIFIER;
		forIdentifier[' '] = State::NONE;

		rls[State::IDENTIFIER] = forIdentifier;
	}
	//for Integer
	{
		for (int i = -128; i < 128; i++)
			forInteger[char(i)] = State::ERROR;
		for (char ch = '0'; ch <= '9'; ch++)
			forInteger[ch] = State::NUMBER;
		for (auto& sym : symbols)
			forInteger[sym.first] = State::SYMBOLS;
		forInteger['.'] = State::NUMBER;
		forInteger[' '] = State::NONE;
		rls[State::NUMBER] = forInteger;
	}
	//for Symbol
	{
		forSymbols = forNone;
		rls[State::SYMBOLS] = forSymbols;
	}
	return rls;
}

set<char> getAlphabet() {
	set<char> result;
	for (int i = -128; i < 128; i++)
		result.insert(char(i));
	return result;
}