#include "ParsingTable.h"
#include <bits/stdc++.h>
using namespace std;

ParsingTable::ParsingTable(
    const map<string, vector<vector<string>>> &prod,
    const vector<set<Item>> &itemSets,
    const map<pair<int, string>, int> &transitions,
    const map<string, set<string>> &follow,
    const string &startSymbol) : productions(prod), itemSets(itemSets), transitions(transitions), follow(follow), startSymbol(startSymbol) {}

void ParsingTable::generateParseTable()
{
    // Create production index mapping
    map<pair<string, vector<string>>, int> productionIndex;
    int productionNumber = 0;
    for (const auto &production : productions)
    {
        const string &lhs = production.first;
        for (const auto &rhs : production.second)
        {
            productionIndex[{lhs, rhs}] = productionNumber++;
        }
    }

    // Build ACTION and GOTO tables
    for (size_t stateId = 0; stateId < itemSets.size(); ++stateId)
    {
        for (const auto &item : itemSets[stateId])
        {
            if (item.dot < item.rhs.size())
            {
                // Shift or Goto operation
                const string &symbol = item.rhs[item.dot];
                auto transitionIter = transitions.find({static_cast<int>(stateId), symbol});

                if (transitionIter != transitions.end())
                {
                    if (productions.find(symbol) != productions.end())
                    {
                        // Non-terminal - GOTO
                        gotoTable[stateId][symbol] = transitionIter->second;
                    }
                    else
                    {
                        // Terminal - SHIFT
                        actionTable[stateId][symbol] = "s" + to_string(transitionIter->second);
                    }
                }
            }
            else
            {
                // Reduce or Accept operation
                if (item.lhs == "S'" && item.rhs.size() == 1 && item.rhs[0] == startSymbol)
                {
                    // Accept action
                    actionTable[stateId]["$"] = "acc";
                }
                else
                {
                    auto productionKey = make_pair(item.lhs, item.rhs);
                    int ruleNumber = productionIndex[productionKey];

                    for (const auto &lookahead : follow.at(item.lhs))
                    {
                        actionTable[stateId][lookahead] = "r" + to_string(ruleNumber);
                    }
                }
            }
        }
    }

    // Display ACTION table
    cout << "\nACTION Table:\n";
    for (const auto &stateActions : actionTable)
    {
        cout << "State " << stateActions.first << ": ";
        for (const auto &symbolAction : stateActions.second)
        {
            cout << symbolAction.first << "=" << symbolAction.second << " ";
        }
        cout << "\n";
    }

    // Display GOTO table
    cout << "\nGOTO Table:\n";
    for (const auto &stateGotos : gotoTable)
    {
        cout << "State " << stateGotos.first << ": ";
        for (const auto &symbolTarget : stateGotos.second)
        {
            cout << symbolTarget.first << "=" << symbolTarget.second << " ";
        }
        cout << "\n";
    }
}

map<int, map<string, string>> ParsingTable::getActionTable(){
    return actionTable;
}
map<int, map<string, int>> ParsingTable::getGotoTable(){
    return gotoTable;
}
