#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> productions;
map<char, set<string>> firstSets;
map<char, set<string>> followSets;
map<pair<char, string>, string> parsingTable;

const string inputFilePath = "./test/input.cpp";
ifstream inputFile;

void parseGrammar(const string &grammarInput) {

    stringstream ss(grammarInput);
    string line;

    while(getline(ss, line, '\n')) {

        char lhs = line[0];
        string rhs = line.substr(3);
        stringstream rhsStream(rhs);

        string token;

        while(getline(rhsStream, token, '|')) {
            productions[lhs].push_back(token);
        }

    }

}

set<string> calculateFirst(const string &rhs) {
    set<string> first;

    for (size_t i = 0; i < rhs.size(); ++i) {

        set<string> subFirst = calculateFirst("" + rhs[i]);

        first.insert(subFirst.begin(), subFirst.end());

        if (subFirst.find("e") == subFirst.end()) {
            break;
        }

    }

    return first;
}

void calculateFollow(char symbol) {

    if(followSets.find(symbol) != followSets.end()) {
        return;
    }

    set<string> follow;

    if(symbol == productions.begin() -> first) {
        follow.insert("$");
    }

    for(const auto &prod : productions) {

        for(const string &rhs : prod.second) {

        }

    }

}

int main() {

    inputFile.open(inputFilePath);

    if(!inputFile.is_open()) {
        cerr << "Error in opening the input file." << endl;
        exit(1);
    }

    string grammarInput;
    string line;

    while(getline(inputFile, line)) {
        if(line == "end") {
            break;
        }

        grammarInput += line + "\n";
    }

    parseGrammar(grammarInput);

    for(const auto &prod : productions) {
        calculateFirst(prod.first);
    }

    for(const auto &prod : productions) {
        calculateFollow(prod.first);
    }

    return 0;
}