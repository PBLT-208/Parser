#include "ItemSetGenerator.h"
#include <bits/stdc++.h>
using namespace std;

bool Item::operator<(const Item &other) const
{
    return tie(lhs, rhs, dot, lookahead) < tie(other.lhs, other.rhs, other.dot, other.lookahead);
}

bool Item::operator==(const Item &other) const
{
    return lhs == other.lhs && rhs == other.rhs && dot == other.dot && lookahead == other.lookahead;
}

ItemSetGenerator::ItemSetGenerator(
    const map<string, vector<vector<string>>> &prod,
    const map<string, set<string>> &firstMap) : productions(prod), first(firstMap) {}

void ItemSetGenerator::generateItemSets()
{
    // Initialize with augmented start symbol by default it is set to S' in augment grammar
    Item startItem = {"S'", productions.at("S'")[0], 0, "$"};
    set<Item> startSet = closure({startItem});
    itemSets.push_back(startSet);

    // Collect all grammar symbols
    set<string> grammarSymbols;
    for (const auto &production : productions)
    {
        grammarSymbols.insert(production.first); // Non-terminals
        for (const auto &rule : production.second)
        {
            for (const auto &symbol : rule)
            {
                grammarSymbols.insert(symbol);
            }
        }
    }

    // Generate all item sets
    bool changed = true;
    while (changed)
    {
        changed = false;
        vector<set<Item>> currentItemSets = itemSets;

        for (size_t stateId = 0; stateId < itemSets.size(); ++stateId)
        {
            for (const auto &symbol : grammarSymbols)
            {
                set<Item> newState = gotoFunction(itemSets[stateId], symbol);

                if (!newState.empty())
                {
                    // Check if this state already exists
                    auto existingState = find(
                        currentItemSets.begin(),
                        currentItemSets.end(),
                        newState);

                    int targetStateId;
                    if (existingState == currentItemSets.end())
                    {
                        currentItemSets.push_back(newState);
                        targetStateId = currentItemSets.size() - 1;
                        changed = true;
                    }
                    else
                    {
                        targetStateId = distance(currentItemSets.begin(), existingState);
                    }

                    // Record the transition
                    transitions[{static_cast<int>(stateId), symbol}] = targetStateId;
                }
            }
        }

        itemSets = currentItemSets;
    }
}

const vector<set<Item>> &ItemSetGenerator::getItemSets() const
{
    return itemSets;
}

const map<pair<int, string>, int> &ItemSetGenerator::getTransitions() const
{
    return transitions;
}

set<Item> ItemSetGenerator::closure(const set<Item> &items)
{
    set<Item> closureSet = items;
    bool changed = true;

    while (changed)
    {
        changed = false;
        set<Item> newItems;

        for (const auto &item : closureSet)
        {
            if (item.dot < item.rhs.size())
            { // means if dot position is not rightmost.
                const string &nextSymbol = item.rhs[item.dot];

                if (productions.find(nextSymbol) != productions.end())
                { // next symbol non-terminal
                    // get remaining symbols after the dot to find first
                    vector<string> remainingSymbols(
                        item.rhs.begin() + item.dot + 1,
                        item.rhs.end());
                    remainingSymbols.push_back(item.lookahead);

                    // find first-set of remaining symbol
                    set<string> firstRemaining;
                    for (const auto &sym : remainingSymbols)
                    {
                        if (productions.find(sym) == productions.end())
                        { // terminal
                            firstRemaining.insert(sym);
                            break;
                        }
                        else
                        { // non-terminal
                            firstRemaining.insert(
                                first.at(sym).begin(),
                                first.at(sym).end());
                            if (first.at(sym).find("ε") == first.at(sym).end())
                            { // if no epsilon it means we done with first part
                                break;
                            }
                        }
                    }

                    // Add new items for each production of the next symbol
                    for (const auto &production : productions.at(nextSymbol))
                    {
                        for (const auto &lookahead : firstRemaining)
                        {
                            Item newItem = {
                                nextSymbol,
                                production,
                                0, // dot at beginning
                                lookahead};

                            if (closureSet.find(newItem) == closureSet.end()) // not pre-computed
                            {
                                newItems.insert(newItem);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }

        closureSet.insert(newItems.begin(), newItems.end());
    }

    return closureSet;
}
set<Item> ItemSetGenerator::gotoFunction(const set<Item> &items, const string &symbol)
{
    set<Item> movedItems;

    for (const auto &item : items)
    {
        if (item.dot < item.rhs.size() && item.rhs[item.dot] == symbol)
        {
            Item movedItem = item;
            movedItem.dot++;
            movedItems.insert(movedItem);
        }
    }

    return closure(movedItems);
}

void ItemSetGenerator::displayItemSets(ostream &os) const
{
    for (size_t stateId = 0; stateId < itemSets.size(); ++stateId)
    {
        cout << "I" << stateId << ":\n";
        for (const auto &item : itemSets[stateId])
        {
            cout << item.lhs << " -> ";

            // Print production with dot
            for (size_t pos = 0; pos < item.rhs.size(); ++pos)
            {
                if (pos == item.dot)
                    cout << ". ";
                cout << item.rhs[pos] << " ";
            }

            // Handle dot at end case
            if (item.dot == item.rhs.size())
                cout << ". ";

            cout << ", " << item.lookahead << "\n";
        }
        cout << "\n";
    }
}