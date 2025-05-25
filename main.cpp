#include "GrammarInput.h"
#include "AugmentGrammar.h"
#include "FirstFollow.h"
#include "ParsingTable.h"
#include "InputParsing.h"
using namespace std;

int main() {
    GrammarInput grammar;

    // read the Context-Free-Grammar
    grammar.readGrammar();
    // grammar.displayGrammar(cout);  
    // Initialize productions and starting symbol
    map<string, vector<vector<string>>> prod = grammar.getProductions();
    string start = grammar.getStartSymbol();

    // Start parsing
    // Step 1: Augment the grammar 
    AugmentGrammar augmented_grammar(prod, start);
    augmented_grammar.addAugmentedRule();
    map<string, vector<vector<string>>> augment_prod = augmented_grammar.getAugmentedProductions();
    cout << "Augmented Grammar: \n";
    for (const auto& entry : augment_prod) {
        cout << entry.first << " -> ";
        for (size_t i = 0; i < entry.second.size(); ++i) {
            for (const string& sym : entry.second[i]) {
                cout << sym << " ";
            }
            if (i != entry.second.size() - 1) cout << "| ";
        }
        cout << "\n";
    }

    // Step 2: Compute First and Follow sets for all symbols
    FirstFollow first_follow(augment_prod);
    first_follow.computeFirst();
    first_follow.computeFollow("S'");

    map<string, set<string>> first = first_follow.getFirst();
    map<string, set<string>> follow = first_follow.getFollow();

    cout << "\nFIRST Sets:\n";
    for (const auto& [nonTerminal, firstSet] : first) {
        cout << "FIRST(" << nonTerminal << ") = { ";
        for (const string& terminal : firstSet) {
            cout << terminal << " ";
        }
        cout << "}\n";
    }

    cout << "\nFOLLOW Sets:\n";
    for (const auto& [nonTerminal, followSet] : follow) {
        cout << "FOLLOW(" << nonTerminal << ") = { ";
        for (const string& terminal : followSet) {
            cout << terminal << " ";
        }
        cout << "}\n";
    }

    // Step 3: Generate LR(1) Item Set (Action and Goto)
    ItemSetGenerator item_generator(augment_prod, first);
    item_generator.generateItemSets();
    item_generator.displayItemSets(cout);
    vector<set<Item>> item_sets = item_generator.getItemSets();

    map<pair<int, string>, int> transitions = item_generator.getTransitions();
    for(auto &mp: transitions){
        cout << mp.first.first << " " << mp.first.second << " " << mp.second << endl;
    }

    // Step 4: Generate CLR Parsing Table
    ParsingTable parse_table(augment_prod, item_sets, transitions, follow, start);
    parse_table.generateParseTable();

    map<int, map<string, string>> actionTable = parse_table.getActionTable();
    map<int, map<string, int>> gotoTable = parse_table.getGotoTable();

    // Step 5: Parse the grammar using CLR Parsing table and tell if syntactically correct or not
    InputParsing input_parse(actionTable, gotoTable, augment_prod);
    input_parse.simulateParser();
   
    return 0;
}
