#include "tests.h"
#include "hashtable.h"

bool addTests(Hashtable* hashtable) {
	std::string name = "Testname test";
	std::string abbreviation = "tstn";
	std::string wkn = "123123";

	Id id(name, abbreviation, wkn);
	StockEntry entry(id);

	hashtable->add(entry);

	//tests
	if (hashtable->calculateHash(entry.getId()->name) != 1313) return false;
	if (hashtable->find(name) == nullptr) return false;
	if (hashtable->find(name)->getId()->name != name) return false;
	if (hashtable->find(name)->getId()->abbreviation != abbreviation) return false;
	if (hashtable->find(name)->getId()->wkn != wkn) return false;

	return true;
}

bool findTests(Hashtable* hashtable) {
	std::string name = "Find Test";
	std::string abbreviation = "ftst";
	std::string wkn = "123123";

	Id id(name, abbreviation, wkn);
	StockEntry entry(id);

	hashtable->add(entry);

	// tests
	if (hashtable->find(name) == nullptr) return false;
	if (hashtable->find(name)->getId()->abbreviation != abbreviation) return false;

	return true;
}

bool removeTests(Hashtable* hashtable) {


	std::string name = "Delete This";
	std::string abbreviation = "delts";
	std::string wkn = "123123";

	Id id(name, abbreviation, wkn);
	StockEntry entry(id);


	hashtable->add(entry);

	hashtable->remove(name);

	if (hashtable->find(name) != nullptr) return false;
	if (hashtable->remove(name) != false) return false;

	return true;
}

void allTests() {
	Hashtable* hashtable = new Hashtable();
	std::cout << "Add test: " << addTests(hashtable) << "\n";
	std::cout << "Find test: " << findTests(hashtable) << "\n";
	std::cout << "Remove test: " << removeTests(hashtable) << "\n";

	delete hashtable;

}