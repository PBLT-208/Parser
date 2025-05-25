// GrammarInput.h
#include<bits/stdc++.h>
using namespace std;

class GrammarInput {
public:
    void readGrammar();// function to read grammar from input grammar file
    void displayGrammar(ostream& os) const; // display grammar function
    map<string, vector<vector<string>>> getProductions() const;// getter function to get production rules of all non-terminals
    string getStartSymbol() const; // getter method to get start symbol of grammar

private:
    // map to store production rules of all symbols, map.first is non-terminal, map.second contains all rules for that symbol
    map<string, vector<vector<string>>> productions;
    string startSymbol; // start symbol of grammar
};
