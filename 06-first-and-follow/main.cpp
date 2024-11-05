#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> productions;
map<char, set<string>> firstSets;
map<char, set<string>> followSets;
map<pair<char, string>, string> parsingTable;

void parseGrammar(const string &grammarInput) {
    stringstream ss(grammarInput);
    string line;

    while (getline(ss, line, '\n')) {
        char lhs = line[0];
        string rhs = line.substr(3);
        stringstream rhsStream(rhs);
        string token;

        while (getline(rhsStream, token, '|')) {
            productions[lhs].push_back(token);
        }
    }
}

set<string> calculateFirst(char symbol) {
    if (firstSets.find(symbol) != firstSets.end()) {
        return firstSets[symbol];
    }

    set<string> first;
    if (!isupper(symbol)) {
        first.insert(string(1, symbol));
        return first;
    }

    for (const string &production : productions[symbol]) {
        for (size_t i = 0; i < production.size(); ++i) {
            string subSymbol(1, production[i]);
            set<string> subFirst = calculateFirst(production[i]);
            first.insert(subFirst.begin(), subFirst.end());
            if (subFirst.find("eps") == subFirst.end()) {
                break;
            }
        }
    }

    firstSets[symbol] = first;
    return first;
}

set<string> calculateFirst(const string &rhs) {
    set<string> first;
    for (size_t i = 0; i < rhs.size(); ++i) {
        string subSymbol(1, rhs[i]);
        set<string> subFirst = calculateFirst(rhs[i]);
        first.insert(subFirst.begin(), subFirst.end());
        if (subFirst.find("eps") == subFirst.end()) {
            break;
        }
    }
    return first;
}

void calculateFollow(char symbol) {
    if (followSets.find(symbol) != followSets.end()) {
        return;
    }

    set<string> follow;
    if (symbol == productions.begin()->first) {
        follow.insert("$");
    }

    for (const auto &prod : productions) {
        for (const string &rhs : prod.second) {
            for (size_t i = 0; i < rhs.size(); ++i) {
                if (rhs[i] == symbol) {
                    if (i + 1 < rhs.size()) {
                        set<string> firstNext = calculateFirst(rhs.substr(i + 1));
                        for (const string &f : firstNext) {
                            if (f != "eps") {
                                follow.insert(f);
                            }
                        }
                        if (firstNext.find("eps") != firstNext.end() && prod.first != symbol) {
                            calculateFollow(prod.first);
                            follow.insert(followSets[prod.first].begin(), followSets[prod.first].end());
                        }
                    } else if (prod.first != symbol) {
                        calculateFollow(prod.first);
                        follow.insert(followSets[prod.first].begin(), followSets[prod.first].end());
                    }
                }
            }
        }
    }

    followSets[symbol] = follow;
}

void generateParsingTable() {
    for (const auto &prod : productions) {
        char lhs = prod.first;
        for (const string &rhs : prod.second) {
            set<string> first = calculateFirst(rhs);
            for (const string &terminal : first) {
                if (terminal != "eps") {
                    parsingTable[{lhs, terminal}] = rhs;
                }
            }
            if (first.find("eps") != first.end()) {
                set<string> follow = followSets[lhs];
                for (const string &terminal : follow) {
                    parsingTable[{lhs, terminal}] = rhs;
                }
            }
        }
    }
}

void displayFirstSets() {
    cout << "FIRST sets:" << endl;
    for (const auto &firstSet : firstSets) {
        cout << firstSet.first << " : { ";
        for (const string &terminal : firstSet.second) {
            cout << terminal << " ";
        }
        cout << "}" << endl;
    }
}

void displayFollowSets() {
    cout << "FOLLOW sets:" << endl;
    for (const auto &followSet : followSets) {
        cout << followSet.first << " : { ";
        for (const string &terminal : followSet.second) {
            cout << terminal << " ";
        }
        cout << "}" << endl;
    }
}

void displayParsingTable() {
    cout << "Parsing Table:" << endl;
    for (const auto &entry : parsingTable) {
        cout << entry.first.first << ", " << entry.first.second << " : " << entry.second << endl;
    }
}

int main() {

    string inputFilePath = "./test/input.txt";

    ifstream inputFile(inputFilePath);

    if(!inputFile.is_open()) {
        cerr << "Error opening the input file." << endl;
        return 1;
    }

    string grammarInput;
    string line;

    // cout << "Enter the grammar (use '|' to separate alternatives, and type 'end' to finish):" << endl;
    while (getline(inputFile, line)) {
        if (line == "end") break;
        grammarInput += line + "\n";
    }

    parseGrammar(grammarInput);

    for (const auto &prod : productions) {
        calculateFirst(prod.first);
    }

    for (const auto &prod : productions) {
        calculateFollow(prod.first);
    }

    generateParsingTable();

    displayFirstSets();
    displayFollowSets();
    displayParsingTable();

    return 0;
}