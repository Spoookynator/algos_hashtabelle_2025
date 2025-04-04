#include "save_load.h"

#include <fstream>
#include <sstream>

bool SaveLoad::save(const Hashtable& hashtable, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return false;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        /**
         * TODO:
         * Iterate through the hashtable
         * if the hashtable at this index is occupied,
         * save the hash value and the data in the file
         * if the hashtable at his index is empty,
         * ignore and move on
         */
        auto entry = hashtable.currentTable()[i];
        if (entry != nullptr && entry->occupied) {
            auto id = entry->getId();
            auto data = entry->getData();
            file << "ENTRY\n" << id->name << "," << id->abbreviation << "," << id->wkn << "\n";
            for (int j = 0; j < 30; j++) {
                if (data[j] != nullptr) {
                    file << data[j]->date << "," << data[j]->close << "," << data[j]->volume << ","
                        << data[j]->open << "," << data[j]->high << "," << data[j]->low << "\n";
                }
            }
        }
    }

    file.close();
    return true;
}

bool SaveLoad::load(Hashtable& hashtable, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for reading." << std::endl;
        return false;
    }

    std::string line;
    Id currentId("", "", "");
    bool idSet = false;
    while (std::getline(file, line)) {
        if (line == "ENTRY") {
            // Read the actual ID line
            if (!std::getline(file, line)) {
                std::cerr << "Error: Expected ID line but found end of file." << std::endl;
                break;
            }
            std::string name, abbreviation, wkn;
            std::stringstream ss(line);
            std::getline(ss, name, ',');
            std::getline(ss, abbreviation, ',');
            std::getline(ss, wkn, ',');
            currentId = Id(name, abbreviation, wkn);
            idSet = true;
            if (hashtable.find(name)) {
                std::cerr << "Error: ID already exists in hashtable. Removing and re-adding." << std::endl;
                hashtable.remove(currentId.name);
            }
            hashtable.add(currentId);
            std::cerr << "Added ID: " << currentId.name << ", " << currentId.abbreviation << ", " << currentId.wkn << std::endl;

            Data* data[30] = { nullptr }; // Initialize the array to nullptr
            for (int i = 0; i < 30; i++) {
                if (!std::getline(file, line) || line.empty() || line == "ENTRY") break;
                std::string date;
                double close;
                int volume;
                double open;
                double high;
                double low;
                std::stringstream ss(line);
                std::getline(ss, date, ',');
                ss >> close;
                ss.ignore();
                ss >> volume;
                ss.ignore();
                ss >> open;
                ss.ignore();
                ss >> high;
                ss.ignore();
                ss >> low;
                data[i] = new Data(date, close, volume, open, high, low);
                std::cerr << "Added Data: " << date << ", " << close << ", " << volume << ", " << open << ", " << high << ", " << low << std::endl;
            }
            hashtable.import(currentId.name, data);
            // Free allocated memory
            for (int i = 0; i < 30; i++) {
                delete data[i];
            }
        }
    }

    file.close();
    return true;
}
