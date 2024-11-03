#include <bits/stdc++.h>
using namespace std;

const string inputFilePath = "./test/input.cpp";
const string outputFilePath = "./test/output.cpp";

int main() {

    ifstream inputFile;
    ofstream outputFile;
    
    try {

        inputFile.open(inputFilePath);

        if(!inputFile.is_open()) {
            throw "Error opening the input file!\n";
        }

        outputFile.open(outputFilePath);

        if(!outputFile.is_open()) {
            throw "Error opening the output file\n";
        }

    } catch(const char* err) {
        cerr << err << endl;
    }

    string line;

    bool partOfMultiLineComment = false;

    while(getline(inputFile, line)) {

        string newLine = "";
        bool partOfString = false;

        for(int i=0; line[i] != '\0'; i++) {

            if(partOfMultiLineComment) {
                if(line[i] == '*' && i+1 < line.length() && line[i+1] == '/') {
                    partOfMultiLineComment = false;
                    i++;
                }
            } else if(partOfString) {
                newLine += line[i];

                if(line[i] == '"' && line[i-1] != '\\') {
                    partOfString = false;
                }
            } else if(line[i] == '"') {
                newLine += line[i];
                partOfString = true;
            } else if(!partOfString && line[i] == '/' && i+1 < line.length() && line[i+1] == '/') {
                break;
            } else if(!partOfString && line[i] == '/' && i+1 < line.length() && line[i+1] == '*') {
                partOfMultiLineComment = true;
            } else {
                newLine += line[i];
            }

        }

        if(newLine.length() > 0) {
            outputFile << newLine << endl;
        }

    }

    return 0;
}