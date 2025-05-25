// AugmentedGrammar.cpp
#include "AugmentGrammar.h"

AugmentGrammar::AugmentGrammar(map<string, vector<vector<string>>> prod, const string& start)
    : productions(prod), originalStart(start) {}

void AugmentGrammar::addAugmentedRule() {
    vector<string> rhs = { originalStart };
    productions["S'"] = { rhs };
}

map<string, vector<vector<string>>> AugmentGrammar::getAugmentedProductions() {
    return productions;
}
