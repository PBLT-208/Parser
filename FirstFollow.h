// FirstFollow.h
#include<bits/stdc++.h>
using namespace std;

class FirstFollow {
public:
    FirstFollow(const map<string, vector<vector<string>>>& prod);// constructor for class
    void computeFirst();// to compute first of a symbol
    void computeFollow(const string& startSymbol);// to compute follow of a symbol
    void displayFirstFollow(ostream& os) const;// display the first and follow tables

    // getters methods for first and follow
    map<string, set<string>> getFirst() const;
    map<string, set<string>> getFollow() const;

private:
    map<string, vector<vector<string>>> productions;//all production rules, map.first is non-terminal, map.second are rules
    map<string, set<string>> first;// first set for each non-terminal
    map<string, set<string>> follow;// follow set for each non-terminal
    set<string> computeFirstOfString(const vector<string>& str) const;// function to find first of a rule

};