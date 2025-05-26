#include <bits/stdc++.h>
#include "ItemSetGenerator.h"
using namespace std;

class ParsingTable {
private:
    map<string, vector<vector<string>>> productions;
    vector<set<Item>> itemSets;
    map<pair<int, string>, int> transitions;
    map<string, set<string>> follow;
    string startSymbol;
    map<int, map<string, string>> actionTable;
    map<int, map<string, int>> gotoTable;

public:
    ParsingTable(
        const map<string, vector<vector<string>>>& productions,
        const vector<set<Item>>& itemSets,
        const map<pair<int, string>, int>& transitions,
        const map<string, set<string>>& follow,
        const string& startSymbol
    );

    void generateParseTable();
    map<int, map<string, string>> getActionTable();
    map<int, map<string, int>> getGotoTable();

};
