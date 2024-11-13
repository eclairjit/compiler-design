#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stack>
using namespace std;

// Action table entry structure
struct Action {
    char type;  // 's' for shift, 'r' for reduce, 'a' for accept
    int state;
    Action(char t = 'e', int s = 0) : type(t), state(s) {}
};

class LRParser {
private:
    // Parsing tables
    map<pair<int, char>, Action> actionTable;
    map<pair<int, char>, int> gotoTable;
    stack<int> stateStack;
    string input;
    int pos;

    void initializeTables() {
        // Action Table
        // State 0
        actionTable[{0, 'i'}] = Action('s', 3);  // shift to state 3
        actionTable[{0, '('}] = Action('s', 2);  // shift to state 2

        // State 1
        actionTable[{1, '+'}] = Action('s', 4);  // shift to state 4
        actionTable[{1, '*'}] = Action('s', 5);  // shift to state 5
        actionTable[{1, '$'}] = Action('a', 0);  // accept

        // State 2
        actionTable[{2, 'i'}] = Action('s', 3);  // shift to state 3
        actionTable[{2, '('}] = Action('s', 2);  // shift to state 2

        // State 3
        actionTable[{3, '+'}] = Action('r', 4);  // reduce by E ? id
        actionTable[{3, '*'}] = Action('r', 4);  // reduce by E ? id
        actionTable[{3, ')'}] = Action('r', 4);  // reduce by E ? id
        actionTable[{3, '$'}] = Action('r', 4);  // reduce by E ? id

        // State 4
        actionTable[{4, 'i'}] = Action('s', 3);  // shift to state 3
        actionTable[{4, '('}] = Action('s', 2);  // shift to state 2

        // State 5
        actionTable[{5, 'i'}] = Action('s', 3);  // shift to state 3
        actionTable[{5, '('}] = Action('s', 2);  // shift to state 2

        // State 6
        actionTable[{6, '+'}] = Action('s', 4);  // shift to state 4
        actionTable[{6, '*'}] = Action('s', 5);  // shift to state 5
        actionTable[{6, ')'}] = Action('s', 9);  // shift to state 9

        // State 7
        actionTable[{7, '+'}] = Action('r', 1);  // reduce by E ? E + E
        actionTable[{7, '*'}] = Action('r', 1);  // reduce by E ? E + E
        actionTable[{7, ')'}] = Action('r', 1);  // reduce by E ? E + E
        actionTable[{7, '$'}] = Action('r', 1);  // reduce by E ? E + E

        // State 8
        actionTable[{8, '+'}] = Action('r', 2);  // reduce by E ? E * E
        actionTable[{8, '*'}] = Action('r', 2);  // reduce by E ? E * E
        actionTable[{8, ')'}] = Action('r', 2);  // reduce by E ? E * E
        actionTable[{8, '$'}] = Action('r', 2);  // reduce by E ? E * E

        // State 9
        actionTable[{9, '+'}] = Action('r', 3);  // reduce by E ? (E)
        actionTable[{9, '*'}] = Action('r', 3);  // reduce by E ? (E)
        actionTable[{9, ')'}] = Action('r', 3);  // reduce by E ? (E)
        actionTable[{9, '$'}] = Action('r', 3);  // reduce by E ? (E)

        // Goto Table
        gotoTable[{0, 'E'}] = 1;
        gotoTable[{2, 'E'}] = 6;
        gotoTable[{4, 'E'}] = 7;
        gotoTable[{5, 'E'}] = 8;
    }

    void displayStack() {
        cout << "Stack: ";
        stack<int> tempStack = stateStack;
        vector<int> states;
        while (!tempStack.empty()) {
            states.push_back(tempStack.top());
            tempStack.pop();
        }
        for (int i = 0; i < states.size(); i++) {
            cout << states[i] << " ";
        }
        cout << "\tInput: " << input.substr(pos) << "$" << endl;
    }

    void errorRoutine1() {
        cout << "Error: Missing operand" << endl;
        cout << "Action: Inserting 'i'" << endl;
        input.insert(pos, "i");
    }

    void errorRoutine2() {
        cout << "Error: Unbalanced right parenthesis" << endl;
        cout << "Action: Removing ')'" << endl;
        input.erase(pos, 1);
    }

    void errorRoutine3() {
        cout << "Error: Missing operator" << endl;
        cout << "Action: Inserting '+'" << endl;
        input.insert(pos, "+");
    }

    void errorRoutine4() {
        cout << "Error: Missing right parenthesis" << endl;
        cout << "Action: Inserting ')'" << endl;
        input.insert(pos, ")");
    }

    bool handleError(int currentState) {
        // Error routine e1
        if ((currentState == 0 || currentState == 2 || currentState == 4 || currentState == 5) &&
            (pos >= input.length() || input[pos] == '+' || input[pos] == '*')) {
            errorRoutine1();
            return true;
        }
        // Error routine e2
        if ((currentState == 0 || currentState == 1 || currentState == 2 || currentState == 4 || currentState == 5) &&
            pos < input.length() && input[pos] == ')') {
            errorRoutine2();
            return true;
        }
        // Error routine e3
        if ((currentState == 1 || currentState == 6) &&
            (pos < input.length() && (input[pos] == 'i' || input[pos] == ')'))) {
            errorRoutine3();
            return true;
        }
        // Error routine e4
        if (currentState == 6 && pos >= input.length()) {
            errorRoutine4();
            return true;
        }
        return false;
    }

public:
    LRParser() {
        initializeTables();
    }

    void parse(string inputStr) {
        input = inputStr;
        pos = 0;
        while (!stateStack.empty()) stateStack.pop();
        stateStack.push(0);

        cout << "\nParsing: " << input << endl;

        while (true) {
            displayStack();
            int currentState = stateStack.top();
            char currentSymbol = (pos < input.length()) ? input[pos] : '$';

            // Check if action exists in table
            auto actionEntry = actionTable.find({currentState, currentSymbol});
           
            if (actionEntry == actionTable.end()) {
                if (!handleError(currentState)) {
                    cout << "Syntax error at position " << pos << endl;
                    return;
                }
                continue;
            }

            Action action = actionEntry->second;

            if (action.type == 's') {
                stateStack.push(action.state);
                pos++;
                cout << "Shift to state " << action.state << endl;
            }
            else if (action.type == 'r') {
                // Determine number of states to pop based on production
                int popCount;
                switch (action.state) {
                    case 1: case 2: popCount = 3; break;  // E ? E + E or E ? E * E
                    case 3: popCount = 3; break;          // E ? ( E )
                    case 4: popCount = 1; break;          // E ? id
                    default: popCount = 0;
                }

                // Pop states
                for (int i = 0; i < popCount; i++) {
                    if (!stateStack.empty()) stateStack.pop();
                }

                // Goto state
                int gotoState = gotoTable[{stateStack.top(), 'E'}];
                stateStack.push(gotoState);
                cout << "Reduce using rule " << action.state << endl;
            }
            else if (action.type == 'a') {
                cout << "Input accepted!" << endl;
                return;
            }
        }
    }
};

int main() {
    LRParser parser;
    string input;

    cout << "LR Parser with Error Recovery" << endl;
    cout << "Enter expressions using 'i' for ID, '+' or '*' for operators, and '(' ')' for parentheses" << endl;
    cout << "Enter 'q' to quit" << endl;

    while (true) {
        cout << "\nEnter expression: ";
        getline(cin, input);

        if (input == "q") break;
        parser.parse(input);
    }

    return 0;
}