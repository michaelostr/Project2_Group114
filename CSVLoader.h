//
// Created by Michael Ostrowski on 3/26/26.
//

#ifndef PROJECT2_GROUP114_CSVLOADER_H
#define PROJECT2_GROUP114_CSVLOADER_H

#include <string>
#include <vector>
#include "HealthRecord.h"

class CSVLoader {
public:
    static std::vector<HealthRecord> loadHealthData(const std::string& filename);
private:
    static std::vector<std::string> splitCSVLine(const std::string& line);
    static std::string trim(const std::string& str);
    static HealthRecord parseRecord(const std::vector<std::string>& fields);

    static int parseInt(const std::string& value);
    static double parseDouble(const std::string& value);
};

#endif //PROJECT2_GROUP114_CSVLOADER_H