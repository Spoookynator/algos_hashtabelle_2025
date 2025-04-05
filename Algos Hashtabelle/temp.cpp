#include "save_load.h"

#include <fstream>
#include <sstream>

bool SaveLoad::save(const Hashtable& hashtable, const std::string& filename) { //THIS CODE WORKS AS INTENDED
	std::ofstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
		return false;
	}

	for (int i = 0; i < TABLE_SIZE; i++) {
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

	// Clear the hashtable
	for (int i = 0; i < TABLE_SIZE; i++) {
		auto entry = hashtable.currentTable()[i];
		if (entry != nullptr && entry->occupied) {
			hashtable.remove(entry->getId()->name); // remove old entry
		}
	}

	std::string line;
	Id currentId("", "", "");
	Data* currentData[30] = { nullptr };
	bool idIsSet = false;
	bool dataIsSet = false;
	int dataIndex = 0;

	while (std::getline(file, line)) {
		if (line == "ENTRY") {
			if (idIsSet && dataIsSet) {
				// Create a new StockEntry with the currentId and currentData
				StockEntry* entry = new StockEntry(currentId, currentData);
				hashtable.loadTable(*entry); // Add to hashtable
				idIsSet = false; // Reset for next entry
				dataIsSet = false;
				dataIndex = 0;
			}
		}
		else {
			//count the number of commas in the line
			int commaCount = std::count(line.begin(), line.end(), ',');
			if (commaCount == 2) {
				std::stringstream idStrings(line);
				std::string name, abbreviation, wkn;

				std::getline(idStrings, name, ',');
				std::getline(idStrings, abbreviation, ',');
				std::getline(idStrings, wkn, ',');

				currentId.name = name;
				currentId.abbreviation = abbreviation;
				currentId.wkn = wkn;
				idIsSet = true;
			}
			if (commaCount == 5) {
				std::stringstream dataStrings(line);
				std::string date, closeStr, openStr, highStr, lowStr, volumeStr;
				std::getline(dataStrings, date, ',');
				std::getline(dataStrings, closeStr, ',');
				std::getline(dataStrings, volumeStr, ',');
				std::getline(dataStrings, openStr, ',');
				std::getline(dataStrings, highStr, ',');
				std::getline(dataStrings, lowStr, ',');

				double close = stod(closeStr);
				double open = stod(openStr);
				double high = stod(highStr);
				double low = stod(lowStr);
				int volume = stoi(volumeStr);
				std::cerr << "DATA:\n" << "date - " << date << ", close - " << close << ", open - " << open << ", high - " << high << ", low - " << low << ", volume - " << volume << std::endl;
				if (dataIsSet == false) dataIsSet = true;
				currentData[dataIndex] = new Data(date, close, volume, open, high, low);
				dataIndex++;
			}
		}
	}

	file.close();
	return true;
}
