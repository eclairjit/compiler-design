#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <sstream>

using namespace std;

struct Production {
    char lhs;
    string rhs;
};

void parseGrammar(const string& grammarInput, vector<Production>& productions) {
    stringstream ss(grammarInput);
    string line;
    while (getline(ss, line, '\n')) {
        char lhs = line[0];
        string rhs = line.substr(3); // Skip "E->"
        stringstream rhsStream(rhs);
        string token;
        while (getline(rhsStream, token, '|')) {
            productions.push_back({lhs, token});
        }
    }
}

bool isNonTerminal(char c) {
    return c >= 'A' && c <= 'Z';
}

void displayStack(const stack<string>& st) {
    stack<string> temp = st;
    vector<string> elements;
    while (!temp.empty()) {
        elements.push_back(temp.top());
        temp.pop();
    }
    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        cout << *it;
    }
}

void shiftReduceParse(const vector<Production>& productions, const string& input) {
    stack<string> parseStack;
    string remainingInput = input;
    bool error = false;

    cout << "Step\tAction\t\tStack\t\tRemaining Input" << endl;
    int step = 0;

    while (!remainingInput.empty() || !parseStack.empty()) {
        cout << step++ << "\t";

        // Shift
        if (!remainingInput.empty()) {
            char nextChar = remainingInput[0];
            parseStack.push(string(1, nextChar));
            remainingInput.erase(0, 1);
            cout << "Shift\t\t";
            displayStack(parseStack);
            cout << "\t\t" << remainingInput << endl;
        }

        // Reduce
        bool reduced = false;
        while (!parseStack.empty()) {
            string stackContent;
            stack<string> tempStack = parseStack;
            vector<string> elements;
            while (!tempStack.empty()) {
                elements.push_back(tempStack.top());
                tempStack.pop();
            }
            for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
                stackContent += *it;
            }

            for (const auto& prod : productions) {
                size_t pos = stackContent.rfind(prod.rhs);
                if (pos != string::npos && pos + prod.rhs.size() == stackContent.size()) {
                    for (size_t i = 0; i < prod.rhs.size(); ++i) {
                        parseStack.pop();
                    }
                    parseStack.push(string(1, prod.lhs));
                    cout << step++ << "\tReduce by " << prod.lhs << "->" << prod.rhs << "\t";
                    displayStack(parseStack);
                    cout << "\t\t" << remainingInput << endl;
                    reduced = true;
                    break;
                }
            }
            if (!reduced) break;
        }

        // Check for syntax error
        if (!reduced && remainingInput.empty() && parseStack.size() == 1 && isNonTerminal(parseStack.top()[0])) {
            break;
        } else if (!reduced && remainingInput.empty()) {
            error = true;
            break;
        }
    }

    if (error) {
        cout << "Syntax Error: Unable to parse the input string." << endl;
    } else {
        cout << "Parsing completed successfully." << endl;
    }
}

int main() {
    string grammarInput;
    string inputString;
    string line;

    cout << "Enter the grammar (use '|' to separate alternatives, and type 'end' to finish):" << endl;
    while (getline(cin, line)) {
        if (line == "end") break;
        grammarInput += line + "\n";
    }

    cout << "Enter the input string:" << endl;
    cin >> inputString;

    vector<Production> productions;
    parseGrammar(grammarInput, productions);

    cout << "\nParsing steps:\n" << endl;
    shiftReduceParse(productions, inputString);

    return 0;
}