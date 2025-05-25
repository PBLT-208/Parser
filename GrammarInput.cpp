#include "GrammarInput.h"

void GrammarInput::readGrammar() {
    // take input grammar from file
    string filename = "grammarInput.txt";  
    ifstream infile(filename);

    if (!infile.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'\n";
        return;
    }

    cout << "Reading grammar from file: " << filename << "\n";
    string line;
    bool firstProduction = true;

    // Read grammar line by line
    while (getline(infile, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        string lhs, arrow;

        if (!(iss >> lhs >> arrow)) {
            cerr << "Invalid format in line: " << line << "\n";
            continue;
        }

        if (arrow != "->") {
            cerr << "Invalid production arrow in line: " << line << "\n";
            continue;
        }

        // first non-terminal of first production is start symbol of grammar
        if (firstProduction) {
            startSymbol = lhs;
            firstProduction = false;
        }

        string productionPart;
        getline(iss, productionPart); // read everythin on RHS of ->
        // remove trailing spaces
        productionPart.erase(0, productionPart.find_first_not_of(" \t"));

        istringstream prodIss(productionPart);
        string production;

        while (getline(prodIss, production, '|')) { // if production seperated by OR '|'
            production.erase(0, production.find_first_not_of(" \t"));// remove trailing spaces
            production.erase(production.find_last_not_of(" \t") + 1);

            if (production.empty()) continue;

            istringstream rhsStream(production);
            string symbol;
            vector<string> symbols;

            // push all symbols in RHS, each symbol is a string
            while (rhsStream >> symbol) {
                symbols.push_back(symbol);
            }

            // store production rule only if it is not empty
            if (!symbols.empty()) {
                productions[lhs].push_back(symbols);
            }
        }
    }

    infile.close();
}

// If grammar is correct, then insert in output stream
void GrammarInput::displayGrammar(ostream& os) const {
    os << "\nGrammar:\n";
    for (const auto& entry : productions) {
        os << entry.first << " -> ";
        for (size_t i = 0; i < entry.second.size(); ++i) {
            for (const string& sym : entry.second[i]) {
                os << sym << " ";
            }
            if (i != entry.second.size() - 1) os << "| ";
        }
        os << "\n";
    }
}

map<string, vector<vector<string>>> GrammarInput::getProductions() const {
    return productions;
}

string GrammarInput::getStartSymbol() const {
    return startSymbol;
}
