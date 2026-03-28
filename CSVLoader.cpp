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

// Expects fields in the same column order as the CSV header:
// id, age, gender, body_mass_index, daily_steps, hours_of_sleep,
// water_intake, calories_consumed, smoking_status, alcohol_consumption,
// resting_heart_rate, systolic_blood_pressure, diastolic_blood_pressure,
// cholesterol_level, family_history_of_disease, disease_risk_score
HealthRecord CSVLoader::parseRecord(const vector<string>& fields) {
    if (fields.size() < 16) {
        throw runtime_error("Row has fewer than 16 fields (" +
                            to_string(fields.size()) + " found)");
    }

    return HealthRecord(
        parseInt(fields[0]),    // id
        parseInt(fields[1]),    // age
        fields[2],              // gender (string — no parsing needed)
        parseDouble(fields[3]), // body_mass_index
        parseInt(fields[4]),    // daily_steps
        parseDouble(fields[5]), // hours_of_sleep
        parseDouble(fields[6]), // water_intake
        parseInt(fields[7]),    // calories_consumed
        parseInt(fields[8]),    // smoking_status
        parseInt(fields[9]),    // alcohol_consumption
        parseInt(fields[10]),   // resting_heart_rate
        parseInt(fields[11]),   // systolic_blood_pressure
        parseInt(fields[12]),   // diastolic_blood_pressure
        parseInt(fields[13]),   // cholesterol_level
        parseInt(fields[14]),   // family_history_of_disease
        parseInt(fields[15])    // disease_risk_score
    );
}

vector<HealthRecord> CSVLoader::loadHealthData(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + filename);
    }

    vector<HealthRecord> records;
    string line;

    // Skip the header row
    if (!getline(file, line)) {
        throw runtime_error("File is empty: " + filename);
    }

    int lineNumber = 1;
    while (getline(file, line)) {
        lineNumber++;

        // Skip blank lines
        if (trim(line).empty()) continue;

        try {
            vector<string> fields = splitCSVLine(line);
            records.push_back(parseRecord(fields));
        } catch (const exception& e) {
            // Log the bad row and continue — don't crash the whole load
            cerr << "Warning: skipping line " << lineNumber
                 << " (" << e.what() << ")\n";
        }
    }

    return records;
}