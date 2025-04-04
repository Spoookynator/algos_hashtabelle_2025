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

bool SaveLoad::load(Hashtable& hashtable, const std::string& filename) { //THIS CODE IS NOT WORKING AND VIOLATES ACCESS
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for reading." << std::endl;
        return false;
    }

    std::string line;
    Id currentId("", "", "");
	Data* currentData[30] = { nullptr };
	bool idSet = false;
    while (std::getline(file, line)) {
        if (line == "ENTRY") {
			if (!std::getline(file, line)) {
				std::cerr << "Error: Expected ID line but found end of file." << std::endl;
				break;
			}
			int commaCount = 0;
			for (char c : line) {
				if (c == ',') {
					commaCount++;
				}
			}
			std::cerr << "Comma count: " << commaCount << std::endl;
			if (commaCount == 2) { // name, abbreviation, wkn
				std::string name, abbreviation, wkn;
				std::stringstream ss(line);
				std::getline(ss, name, ',');
				std::getline(ss, abbreviation, ',');
				std::getline(ss, wkn, ',');
				currentId = Id(name, abbreviation, wkn);
				idSet = true;
			} else if (commaCount == 5) { // date, close, volume, open, high, low
				for (int i = 0; i < 30; i++) {
					std::string date;
					double close, open, high, low;
					int volume;
					std::stringstream ss2(line);
					std::getline(ss2, date, ',');
					ss2 >> close;
					ss2.ignore();
					ss2 >> volume;
					ss2.ignore();
					ss2 >> open;
					ss2.ignore();
					ss2 >> high;
					ss2.ignore();
					ss2 >> low;
					Data* data = new Data(date, close, volume, open, high, low);
					currentData[i] = data;
				}
			}
        }

		StockEntry entry = StockEntry(currentId, currentData);
		hashtable.loadTable(entry);
    }

    file.close();
    return true;
}
