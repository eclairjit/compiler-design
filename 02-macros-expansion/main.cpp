// #include <iostream>
// #include <fstream>
// #include <string>
// #include <sstream>
// #include <unordered_map>
// #include <regex>
// #include <vector>

#include <bits/stdc++.h>
using namespace std;

struct Macro {
    vector<string> parameters;
    string body;
};

void preprocessFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    if (!inFile) {
        cerr << "Error opening input file: " << inputFile << endl;
        return;
    }

    unordered_map<string, Macro> macros;
    stringstream buffer;
    string line;
    bool skipLine = false;
    vector<bool> skipStack;

    // Read and parse the input file
    while (getline(inFile, line)) {
        regex funcMacroRegex(R"(^\s*#\s*define\s+(\w+)\s*\(([^)]*)\)\s*(.*)\s*$)");
        regex constMacroRegex(R"(^\s*#\s*define\s+(\w+)\s+(.*)\s*$)");
        smatch match;
        if (regex_match(line, match, funcMacroRegex)) {
            if (match.size() == 4) {
                string macroName = match[1].str();
                string params = match[2].str();
                string macroValue = match[3].str();

                Macro macro;
                istringstream paramStream(params);
                string param;
                while (getline(paramStream, param, ',')) {
                    macro.parameters.push_back(param);
                }
                macro.body = macroValue;
                macros[macroName] = macro;
            }
        } else if (regex_match(line, match, constMacroRegex)) {
            if (match.size() == 3) {
                string macroName = match[1].str();
                string macroValue = match[2].str();

                Macro macro;
                macro.body = macroValue;
                macros[macroName] = macro;
            }
        } else if (line.find("#ifdef") != string::npos) {
            string macroName = line.substr(line.find(" ") + 1);
            skipStack.push_back(skipLine);
            skipLine = macros.find(macroName) == macros.end();
        } else if (line.find("#ifndef") != string::npos) {
            string macroName = line.substr(line.find(" ") + 1);
            skipStack.push_back(skipLine);
            skipLine = macros.find(macroName) != macros.end();
        } else if (line.find("#else") != string::npos) {
            skipLine = !skipLine;
        } else if (line.find("#endif") != string::npos) {
            skipLine = skipStack.back();
            skipStack.pop_back();
        } else if (!skipLine) {
            buffer << line << "\n";
        }
    }

    string code = buffer.str();

    // Replace macros in the code
    for (const auto& macro : macros) {
        if (macro.second.parameters.empty()) {
            // Constant macro
            regex macroUsageRegex("\\b" + macro.first + "\\b");
            code = regex_replace(code, macroUsageRegex, macro.second.body);
        } else {
            // Functional macro
            regex macroUsageRegex("\\b" + macro.first + "\\s*\\(([^)]*)\\)");
            smatch match;
            while (regex_search(code, match, macroUsageRegex)) {
                string args = match[1].str();
                istringstream argStream(args);
                string arg;
                vector<string> argValues;
                while (getline(argStream, arg, ',')) {
                    argValues.push_back(arg);
                }

                string expandedMacro = macro.second.body;
                for (size_t i = 0; i < macro.second.parameters.size(); ++i) {
                    regex paramRegex("\\b" + macro.second.parameters[i] + "\\b");
                    expandedMacro = regex_replace(expandedMacro, paramRegex, argValues[i]);
                }

                code = match.prefix().str() + expandedMacro + match.suffix().str();
            }
        }
    }

    // Write the processed code to the output file
    ofstream outFile(outputFile);
    if (!outFile) {
        cerr << "Error opening output file: " << outputFile << endl;
        return;
    }
    outFile << code;
}

int main() {
    string inputFile = "./test/input.cpp";
    string outputFile = "./test/output.cpp";

    preprocessFile(inputFile, outputFile);

    cout << "Macro expansion completed. Check 'output.cpp' for the result." << endl;

    return 0;
}