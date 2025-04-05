#include "save_load.h"

#include <fstream>
#include <sstream>

bool SaveLoad::save(const Hashtable& hashtable, const std::string& filename) { //THIS CODE WORKS AS INTENDED
    std::ofstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Could not open file " << filename << " for writing!" << std::endl;
		return false;
	}
	
	for (int i = 0; i < TABLE_SIZE; i++) {
		auto entry = hashtable.currentTable()[i];
		if (entry->occupied) {
			auto id = entry->getId();
			auto data = entry->getData();
			file << "ENTRY\n" << id->name << "," << id->abbreviation << "," << id->wkn << "\n";
			for (int j = 0; j < 30; j++) {
				if (data[j] != nullptr) {
					file << data[j]->date << "," << data[j]->close << "," << data[j]->volume << ","
						<< data[j]->open << "," << data[j]->high << "," << data[j]->low << "\n";
				}
				else {
					file << "NULL,NULL,NULL,NULL,NULL,NULL\n"; // Indicate empty data
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
        std::cerr << "Could not open file " << filename << " for reading!" << std::endl;
        return false;
    }

    std::string line;
    Id currentId("", "", "");
    Data* currentData[30] = { nullptr };
    bool idIsSet = false;
    int dataIndex = 0;

    while (std::getline(file, line)) {
        bool isNull = (line == "NULL,NULL,NULL,NULL,NULL,NULL");
        if (line == "ENTRY") {
            if (idIsSet) {
                // Create a new StockEntry with the currentId and currentData
                StockEntry* entry = new StockEntry(currentId, currentData);
                hashtable.loadTable(*entry); // Add to hashtable

                // Clean up currentData
                for (int i = 0; i < 30; i++) {
                    delete currentData[i];
                    currentData[i] = nullptr;
                }
                currentId.name = "";
                currentId.abbreviation = "";
                currentId.wkn = "";
                dataIndex = 0;
            }

            if (!std::getline(file, line)) {
                std::cerr << "Expected ID line but found end of file!" << std::endl;
                break;
            }
            std::stringstream idStrings(line);
            std::string name, abbreviation, wkn;
            std::getline(idStrings, name, ',');
            std::getline(idStrings, abbreviation, ',');
            std::getline(idStrings, wkn, ',');
            currentId = Id(name, abbreviation, wkn);
            idIsSet = true;
            if (hashtable.currentTable()[hashtable.calculateHash(name)]->occupied) {
				hashtable.remove(name); // remove old entry
            }
        }
        else {
            int commaCount = std::count(line.begin(), line.end(), ',');
            if (isNull) {
                currentData[dataIndex] = nullptr; // Indicate empty data
                dataIndex++;
            }
            // Count the number of commas in the line
            else if (commaCount == 5) {
                std::stringstream dataStrings(line);
                std::string date, closeStr, openStr, highStr, lowStr, volumeStr;
                std::getline(dataStrings, date, ',');
                std::getline(dataStrings, closeStr, ',');
                std::getline(dataStrings, volumeStr, ',');
                std::getline(dataStrings, openStr, ',');
                std::getline(dataStrings, highStr, ',');
                std::getline(dataStrings, lowStr, ',');

                double close = std::stod(closeStr);
                double open = std::stod(openStr);
                double high = std::stod(highStr);
                double low = std::stod(lowStr);
                int volume = std::stoi(volumeStr);
                currentData[dataIndex] = new Data(date, close, volume, open, high, low);
                dataIndex++;
			}
            else {
                std::cerr << "Invalid data line format: " << line << std::endl;
            }
        }
    }

    // Load the last entry if idIsSet is true
    if (idIsSet) {
        StockEntry* entry = new StockEntry(currentId, currentData);
        hashtable.loadTable(*entry);

        // Clean up currentData
        for (int i = 0; i < 30; i++) {
            delete currentData[i];
        }
    }

    file.close();
    return true;
}
