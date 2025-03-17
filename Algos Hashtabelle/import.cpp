#include "import.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>
#include "hashtable.h"

std::tm parseDate(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%m/%d/%Y");
    return tm;
}

std::string formatDate(const std::tm& tm) {
    std::ostringstream ss;
    ss << std::put_time(&tm, "%m/%d/%Y");
    return ss.str();
}

std::string getPreviousDate(const std::string& dateStr) {
    std::tm date = parseDate(dateStr);
    date.tm_mday--;
    mktime(&date); // Normalize the date
    return formatDate(date);
}

double parsePrice(const std::string& priceStr) {
    return std::stod(priceStr.substr(1)); // Skip the '$' symbol
}

Data** parseCsv(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return nullptr;
    }

    std::map<std::string, Data*> dataMap;
    bool firstLine = true;

    while (std::getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            continue; // Skip header
        }

        std::stringstream ss(line);
        std::string date, closeStr, openStr, highStr, lowStr, volumeStr;

        // Read comma-separated values
        if (!std::getline(ss, date, ',') ||
            !std::getline(ss, closeStr, ',') ||
            !std::getline(ss, volumeStr, ',') ||
            !std::getline(ss, openStr, ',') ||
            !std::getline(ss, highStr, ',') ||
            !std::getline(ss, lowStr, ',')) {
            std::cerr << "Invalid CSV format: " << line << "\n";
            continue;
        }
        double close, open, high, low;
        int volume;
        char delimiter;

        close = parsePrice(closeStr);
        open = parsePrice(openStr);
        high = parsePrice(highStr);
        low = parsePrice(lowStr);

        volume = std::stoi(volumeStr);

        dataMap[date] = new Data(date, close, volume, open, high, low);
    }

    if (dataMap.empty()) {
        std::cerr << "CSV file is empty or invalid!" << std::endl;
        return nullptr;
    }

    std::string latestDate = dataMap.rbegin()->first;

    Data** stockData = new Data*[30]{};

    for (int i = 29; i >= 0; --i) {
        if (dataMap.count(latestDate)) {
            stockData[i] = dataMap[latestDate];
        }
        else {
            stockData[i] = nullptr;
        }
        latestDate = getPreviousDate(latestDate);
    }

    return stockData;
}