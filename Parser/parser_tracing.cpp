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

std::string trace(const std::string& msg) {
    incIdent();
    tracePrint("BEGIN " + msg);
    return msg;
}

void untrace(const std::string& msg) {
    tracePrint("END " + msg);
    decIdent();
}

