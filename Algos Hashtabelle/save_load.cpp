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
		if (!entry->empty) {
            auto occupied = entry->occupied;
			auto pastName = entry->pastName;
			auto id = entry->getId();
			auto data = entry->getData();
            file << "ENTRY\n" << i << "," << occupied << "," << pastName << "\n";
            file << id->name << "," << id->abbreviation << "," << id->wkn << "\n";
			for (int j = 0; j < 30; j++) {
				if (data[j] != nullptr) {
					file << data[j]->date << "," << data[j]->close << "," << data[j]->volume << ","
						<< data[j]->open << "," << data[j]->high << "," << data[j]->low << "\n";
				}
				else {
					file << "NULL\n"; // Indicate empty data
                }
			}
		}
        else if (entry->empty && entry->occupied){
            auto occupied = entry->occupied;
            auto pastName = entry->pastName;
            file << "PASTENTRY\n" << i << "," << occupied << "," << pastName;
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
    int currentIndex;
    Entry currentEntry;
    Id currentId("", "", "");
    Data* currentData[30] = { nullptr };
    bool idIsSet = false;
    int dataIndex = 0;

    while (std::getline(file, line)) {
        bool isNull = (line == "NULL");
        if (line == "ENTRY") {
            if (idIsSet) {
                // Create a new StockEntry with the currentId and currentData
                StockEntry* entry = new StockEntry(currentId, currentData);
                entry->empty = false;
				entry->occupied = true; // Mark as occupied
                entry->pastName = "";
                hashtable.loadTableEntries(*entry, currentIndex); // Add to hashtable

                // Clean up currentData
                for (int i = 0; i < 30; i++) {
                    delete currentData[i];
                    currentData[i] = nullptr;
                }
                currentId.name = "";
                currentId.abbreviation = "";
                currentId.wkn = "";
                currentEntry.empty = true;
				currentEntry.occupied = false;
				entry->pastName = "";
                dataIndex = 0;
            }

            if (!std::getline(file, line)) {
                std::cerr << "Expected ID line but found end of file!" << std::endl;
                break;
            }

			std::stringstream entryStrings(line);
			std::string indexStr;
			std::string occupiedStr;
			std::string pastNameStr;
			std::getline(entryStrings, indexStr, ',');
			currentIndex = std::stoi(indexStr);
			std::getline(entryStrings, occupiedStr, ',');
			bool occupied = (occupiedStr == "true") ? true : false;
			std::getline(entryStrings, pastNameStr, ',');
            currentEntry = Entry(false, occupied, pastNameStr);
        }
        else if (line == "PASTENTRY") {
            std::getline(file, line);
            std::string indexStr;
            std::string occupiedStr;
            std::string pastNameStr;
            std::stringstream entryStrings(line);
            std::getline(entryStrings, indexStr, ',');
            int index = stoi(indexStr);
            std::getline(entryStrings, occupiedStr, ',');
            bool occupied = (occupiedStr == "true") ? true : false;
            std::getline(entryStrings, pastNameStr, ',');
            Entry pastEntry = Entry(false, occupied, pastNameStr);
            hashtable.loadTableEntries(pastEntry, index);
        }
		else if (line == "NULL") {
			// Handle NULL data
			currentData[dataIndex] = nullptr;
			dataIndex++;
		}
        else {
            int commaCount = std::count(line.begin(), line.end(), ',');
            if (commaCount == 2) {
                std::stringstream idStrings(line);
                std::string name, abbreviation, wkn;
                std::getline(idStrings, name, ',');
                std::getline(idStrings, abbreviation, ',');
                std::getline(idStrings, wkn, ',');
                currentId = Id(name, abbreviation, wkn);
                idIsSet = true;
                if (hashtable.currentTable()[currentIndex]->occupied) {
                    hashtable.remove(name); // remove old entry
                }
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
        hashtable.loadTableEntries(*entry);

        // Clean up currentData
        for (int i = 0; i < 30; i++) {
            delete currentData[i];
        }
    }

    file.close();
    return true;
}
