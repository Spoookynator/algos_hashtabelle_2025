#include "hashtable.h"

hashtable::hashtable() {
	// empty initialization
	for (int i = 0; i < TABLE_SIZE; i++) {
		table[i].stockId.name = "";
		table[i].stockId.abbreviation = "";
		std::cout << "table[" << i << "].stockId.name: " << table[i].stockId.name << std::endl;
	}
}

int hashtable::calculateHash(string abbreviation) {
	// hash function
	int hashIndex = 0;
	for (int i = 0; i < abbreviation.length(); i++) {
		hashIndex += abbreviation[i];
	}
	hashIndex = hashIndex % TABLE_SIZE; //2003
	return hashIndex;
}

