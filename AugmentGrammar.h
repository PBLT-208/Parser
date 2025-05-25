// AugmentedGrammar.h
#include<bits/stdc++.h>
using namespace std;

class AugmentGrammar {
public:
    AugmentGrammar(map<string, vector<vector<string>>> prod, const string& start);
    void addAugmentedRule();
    map<string, vector<vector<string>>> getAugmentedProductions();

private:
    map<string, vector<vector<string>>> productions;
    string originalStart;
};
