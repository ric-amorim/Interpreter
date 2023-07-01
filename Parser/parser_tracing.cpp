#include <iostream>
#include <string>


int traceLevel = 0;
const std::string traceIdentPlaceholder = "\t";

std::string identLevel() {
    return std::string(traceLevel - 1, '\t');
}

void tracePrint(const std::string& fs) {
    std::cout << identLevel() << fs << std::endl;
}

void incIdent() {
    traceLevel++;
}

void decIdent() {
    traceLevel--;
}

