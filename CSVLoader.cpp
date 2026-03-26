//
// Created by Michael Ostrowski on 3/26/26.
//

#include "CSVLoader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <cctype>

using namespace std;

string CSVLoader::trim(const string& str) {
    size_t start = 0;
    while (start < str.size() && isspace(static_cast<unsigned char>(str[start]))) {
        start++;
    }

    size_t end = str.size();
    while (end > start && isspace(static_cast<unsigned char>(str[end - 1]))) {
        end--;
    }

    return str.substr(start, end - start);
}

vector<string> CSVLoader::splitCSVLine(const string& line) {
    vector<string> fields;
    string field;
    stringstream ss(line);

    while (getline(ss, field, ',')) {
        fields.push_back(trim(field));
    }

    return fields;
}

int CSVLoader::parseInt(const string& value) {
    size_t pos = 0;
    int result = stoi(value, &pos);

    if (pos != value.size()) {
        throw runtime_error("Invalid integer value: " + value);
    }
    return result;
}

double CSVLoader::parseDouble(const string& value) {
    size_t pos = 0;
    double result = stod(value, &pos);

    if (pos != value.size()) {
        throw runtime_error("Invalid double value: " + value);
    }
    return result;
}