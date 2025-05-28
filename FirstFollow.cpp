#include "FirstFollow.h"
#include<bits/stdc++.h>

using namespace std;

FirstFollow::FirstFollow(const map<string, vector<vector<string>>>& prod)
    : productions(prod) {}

void FirstFollow::computeFirst() {
    // Initialize FIRST sets for all non-terminals
    for (const auto& [nonTerminal, _] : productions) {
        // empty set initially
        first[nonTerminal] = {};
    }

    bool changed = true;// if no change is occuring in the first sets, then we stop looping
    while (changed) {
        changed = false;
        for (const auto& [nonTerminal, rules] : productions) {  // loop the productions of all symbols
            for (const auto& rule : rules) { // iterate through rules of a symbol
                set<string> result = computeFirstOfString(rule);// compute first of each rule
                size_t before = first[nonTerminal].size();// record size to see if change has happened in first set of table
                first[nonTerminal].insert(result.begin(), result.end());// insert the first set of symbol into first table
                if (first[nonTerminal].size() > before) { // if first set updated, then continue loop 
                    changed = true;
                }
            }
        }
    }
}

set<string> FirstFollow::computeFirstOfString(const vector<string>& str) const {
    set<string> result;
    // if no symbol, then first is epsilon
    if (str.empty()) {
        result.insert("ε");
        return result;
    }


    size_t strSize=str.size();
    for (size_t i = 0; i < strSize; ++i) {
        const string& symbol = str[i];
        // Terminal symbol if not in productions
        if (productions.find(symbol) == productions.end()) {
            result.insert(symbol);
            // first is found, so break
            break;
        }

        // if non-terminal, then find first of that non-terminal and insert in result (can be empty)
        const auto& firstSet = first.at(symbol);
        result.insert(firstSet.begin(), firstSet.end());

        // if no epsilon rule
        if (!firstSet.count("ε")) {
            break;
        }

        // If epsilon is in FIRST(symbol), continue to next symbol
        // insert epsilon only if every previous symbol (deriving this symbol's first) has epsilon in their first set
        if (i == strSize - 1) {
            result.insert("ε");
        } else {
            result.erase("ε");
        }
    }
    return result;
}

void FirstFollow::computeFollow(const string& startSymbol) {
    for (const auto& [nonTerminal, _] : productions) {
        follow[nonTerminal] = {};// intialize empty follow sets for all symbols
    }
    follow[startSymbol].insert("$"); // start symbol always has $ as follow

    bool changed = true;
    while (changed) {// loop until no new changes in follow table
        changed = false;
        for (const auto& [lhs, rules] : productions) { // looping through production rules of every symbol
            for (const auto& rule : rules) { // looping through rules of a symbol
                for (size_t i = 0; i < rule.size(); ++i) {
                    const string& B = rule[i];// first symbol of rule
                    // skip terminals as follow is to be calculated only for non-terminals
                    if (productions.find(B) == productions.end()) continue;

                    size_t before = follow[B].size();// calculate size to track change

                    if (i + 1 < rule.size()) {
                        // see the next symbol of non-terminal
                        vector<string> beta(rule.begin() + i + 1, rule.end()); 
                        set<string> firstBeta = computeFirstOfString(beta);// find first of that symbol
                        for (const string& f : firstBeta) {
                        // if there is following symbol, then insert into follow set
                            if (f != "ε") {
                                follow[B].insert(f);
                            }
                        }
                        // if epsilon is in first set, then follow of lhs is to be added to follow of B
                        if (firstBeta.count("ε")) { 
                            follow[B].insert(follow[lhs].begin(), follow[lhs].end());
                        }
                    } else {
                        // if last RHS symbol, then follow will be equal to follow of LHS
                        follow[B].insert(follow[lhs].begin(), follow[lhs].end());
                    }

                    if (follow[B].size() > before) {// if size increased then loop continues
                        changed = true;
                    }
                }
            }
        }
    }
}

// Defination to display First and Follow sets
void FirstFollow::displayFirstFollow(ostream& os) const {
    os << "\nFIRST sets:\n";
    for (const auto& [nonTerminal, firstSet] : first) {
        os << "FIRST(" << nonTerminal << ") = { ";
        for (const string& symbol : firstSet) {
            os << symbol << " ";
        }
        os << "}\n";
    }

    os << "\nFOLLOW sets:\n";
    for (const auto& [nonTerminal, followSet] : follow) {
        os << "FOLLOW(" << nonTerminal << ") = { ";
        for (const string& symbol : followSet) {
            os << symbol << " ";
        }
        os << "}\n";
    }
}

map<string, set<string>> FirstFollow::getFirst() const {
    return first;
}

map<string, set<string>> FirstFollow::getFollow() const {
    return follow;
}
