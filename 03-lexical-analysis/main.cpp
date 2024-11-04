// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <unordered_map>
// #include <vector>
// #include <cctype>
// #include <algorithm>

#include <bits/stdc++.h>
using namespace std;

class Tokenizer {
public:
    int countTokens = 0;
    int line = 1;
    unordered_map<string, pair<string, string>> symbolTable;

    void scan(const string& inputFile, const string& outputFile);
    bool isKeyword(const string& token);
    bool isIdentifier(const string& token);
    bool isConstant(const string& token);
    bool isOperator(const string& token);
    bool isString(const string& token);
    bool isSpecialSymbol(const string& token);

private:
    const vector<string> keywords = {"int", "float", "char", "if", "else", "while", "return", "string", "bool", "for", "using", "std", "namespace"};
    const vector<string> operators = {"+", "-", "*", "/", "=", "==", "!="};
    const vector<char> specialSymbols = {';', '(', ')', '{', '}', ','};
};

void Tokenizer::scan(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    if (!inFile.is_open() || !outFile.is_open()) {
        cerr << "Error opening file." << endl;
        return;
    }

    string token;
    char ch;
    while (inFile.get(ch)) {
        if (isspace(ch)) {
            if (!token.empty()) {
                if (isKeyword(token)) {
                    symbolTable[token] = {"Keyword", ""};
                } else if (isIdentifier(token)) {
                    symbolTable[token] = {"Identifier", ""};
                } else if (isConstant(token)) {
                    symbolTable[token] = {"Constant", ""};
                } else if (isOperator(token)) {
                    symbolTable[token] = {"Operator", ""};
                } else if (isString(token)) {
                    symbolTable[token] = {"String", ""};
                }
                token.clear();
            }
            if (ch == '\n') {
                line++;
            }
        } else if (isSpecialSymbol(string(1, ch))) {
            if (!token.empty()) {
                if (isKeyword(token)) {
                    symbolTable[token] = {"Keyword", ""};
                } else if (isIdentifier(token)) {
                    symbolTable[token] = {"Identifier", ""};
                } else if (isConstant(token)) {
                    symbolTable[token] = {"Constant", ""};
                } else if (isOperator(token)) {
                    symbolTable[token] = {"Operator", ""};
                } else if (isString(token)) {
                    symbolTable[token] = {"String", ""};
                }
                token.clear();
            }
            symbolTable[string(1, ch)] = {"SpecialSymbol", ""};
        } else {
            token += ch;
        }
    }

    // Write the symbol table to the output file
    for (const auto& entry : symbolTable) {
        outFile << entry.first << " : " << entry.second.first << endl;
    }

    inFile.close();
    outFile.close();
}

bool Tokenizer::isKeyword(const string& token) {
    return find(keywords.begin(), keywords.end(), token) != keywords.end();
}

bool Tokenizer::isIdentifier(const string& token) {
    if (token.empty() || !isalpha(token[0])) return false;
    return all_of(token.begin() + 1, token.end(), [](char c) { return isalnum(c) || c == '_'; });
}

bool Tokenizer::isConstant(const string& token) {
    return all_of(token.begin(), token.end(), ::isdigit);
}

bool Tokenizer::isOperator(const string& token) {
    return find(operators.begin(), operators.end(), token) != operators.end();
}

bool Tokenizer::isString(const string& token) {
    return token.size() >= 2 && token.front() == '"' && token.back() == '"';
}

bool Tokenizer::isSpecialSymbol(const string& token) {
    return token.size() == 1 && find(specialSymbols.begin(), specialSymbols.end(), token[0]) != specialSymbols.end();
}

int main() {
    Tokenizer tokenizer;
    string inputFile = "./test/input.cpp";
    string outputFile = "./test/output.txt";

    tokenizer.scan(inputFile, outputFile);

    cout << "Tokenization completed. Check 'output.cpp' for the result." << endl;

    return 0;
}