//ItemSetGenerator.h
#include<bits/stdc++.h>
using namespace std;

struct Item {
    string lhs;
    vector<string> rhs;
    size_t dot;
    string lookahead;

    bool operator<(const Item& other) const;
    bool operator==(const Item& other) const;
};

class ItemSetGenerator {
public:
    ItemSetGenerator(const map<string, vector<vector<string>>>& prod,
                     const map<string, set<string>>& first);

    void generateItemSets();
    void displayItemSets(ostream& os) const;
    const vector<set<Item>>& getItemSets() const;
    const map<pair<int, string>, int>& getTransitions() const;

private:
    map<string, vector<vector<string>>> productions;
    map<string, set<string>> first;
    vector<set<Item>> itemSets;
    map<pair<int, string>, int> transitions;

    set<Item> closure(const set<Item>& items);
    set<Item> gotoFunction(const set<Item>& items, const string& symbol);
};