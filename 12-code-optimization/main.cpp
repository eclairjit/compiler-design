#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>

struct Instruction {
    int id;
    std::string op;
    std::string lhs;
    std::string rhs1;
    std::string rhs2;
};

std::vector<Instruction> program;
std::unordered_map<int, std::unordered_set<int>> inSets;
std::unordered_map<int, std::unordered_set<int>> outSets;
std::unordered_map<int, std::unordered_set<int>> genSets;
std::unordered_map<int, std::unordered_set<int>> killSets;

void readProgramFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Instruction instr;
        iss >> instr.id >> instr.op >> instr.lhs >> instr.rhs1 >> instr.rhs2;
        program.push_back(instr);
    }
}

void initializeSets() {
    for (const auto& instr : program) {
        inSets[instr.id] = {};
        outSets[instr.id] = {};
        genSets[instr.id] = {instr.id};
        killSets[instr.id] = {};
    }
}

void writeBitVectorsToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    file << "Instruction ID\tIN\tOUT\tGEN\tKILL\n";
    for (const auto& instr : program) {
        file << instr.id << "\t\t";

        for (const auto& elem : inSets[instr.id]) {
            file << elem << " ";
        }
        file << "\t";

        for (const auto& elem : outSets[instr.id]) {
            file << elem << " ";
        }
        file << "\t";

        for (const auto& elem : genSets[instr.id]) {
            file << elem << " ";
        }
        file << "\t";

        for (const auto& elem : killSets[instr.id]) {
            file << elem << " ";
        }
        file << "\n";
    }

    file.close();
}

int main() {
    std::string inputFilename = "program.txt";
    std::string outputFilename = "bit_vectors.txt";

    readProgramFromFile(inputFilename);
    initializeSets();
    writeBitVectorsToFile(outputFilename);

    std::cout << "Bit vectors written to " << outputFilename << std::endl;

    return 0;
}