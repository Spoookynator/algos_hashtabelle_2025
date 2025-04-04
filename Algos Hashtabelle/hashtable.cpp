#include "hashtable.h"
#include "import.h"

Hashtable::Hashtable() {

	// init table
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		table[i] = new Entry();
	}
}

Hashtable::~Hashtable()
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		delete table[i];
	}
}

int Hashtable::calculateHash(const std::string& hashString) {
	
	int P = 31; // prime num base

	long long hash = 0;

	int power = 1;

	for (char c : hashString) {
		hash = (hash + (c - 'a' + 1) * power) % TABLE_SIZE;
		power = (power * P) % TABLE_SIZE;
	}
	return hash < 0 ? -hash : hash; // hash has to be positive
}

bool Hashtable::import(std::string stockName)
{
	auto data = parseCsv(stockName + ".csv");

	if (data == nullptr) { std::cerr << "Error: parseCsv returned nullptr for " << stockName << ".csv" << std::endl; return false; };

	int hash = findHash(stockName);
	if (hash < 0) hash *= -1; // invert to get real result

	auto stock = find(stockName);

	if (stock == nullptr) { std::cerr << "Error: find returned nullptr for " << stockName << std::endl; return false; }

	auto res = stock->setData(data);

	if (!res) { std::cerr << "Error: setData failed for " << stockName << std::endl; return false; }

	return true;
}

bool Hashtable::loadTable(StockEntry entry)
{
	/**
	 * TODO:
	 * !FIX THIS CODE SO THAT IT NO LONGER GETS ACCESS VIOLATION ERRORS
	 */

	// based on add()
	auto name = entry.getId()->name;

	int hash = findHash(name);

	auto data = entry.stockData;

	auto stock = find(name);


	if (hash < 0) {
		std::cerr << "Error: Element is already inside or no space for " << name << std::endl;
		return false; // if element is already inside or no space
	}

	std::cout << "Added with hash: " << hash << std::endl;

	if (hash >= TABLE_SIZE) {
		std::cerr << "Error: Hash value " << hash << " is out of bounds" << std::endl;
		return false;
	}

	if (this->table[hash] != nullptr) {
		delete this->table[hash]; // free default placeholder
	}
	else {
		std::cerr << "Warning: this->table[" << hash << "] is nullptr" << std::endl;
	}

	// based on import()
	if (data == nullptr) {
		std::cerr << "Error: entry.stockData is nullptr for " << name << std::endl;
		return false;
	}


	if (stock == nullptr) {
		std::cerr << "Error: find returned nullptr for " << name << std::endl;
		return false;
	}
	
	auto res = stock->setData(data);

	if (!res) {
		std::cerr << "Error: setData failed for " << name << std::endl;
		return false;
	}

	this->table[hash] = new StockEntry(entry); // add new entry

	return true;
}

int Hashtable::findHash(std::string identifier)
{
	int hash = calculateHash(identifier);
	std::cerr << "Initial hash for " << identifier << " is " << hash << std::endl;

	int count = 0;
	while (this->table[hash] != nullptr && (!this->table[hash]->empty || this->table[hash]->occupied))
	{
		std::cerr << "Checking position " << hash << " for " << identifier << std::endl;
		if (this->table[hash]->pastName == identifier && this->table[hash]->empty)
		{
			std::cerr << "Found past entry for " << identifier << " at position " << hash << std::endl;
			return hash; // this is the past spot of this entry
		}

		if (this->table[hash]->getId() != nullptr && this->table[hash]->getId()->name == identifier) // we could also change that to wkn or sth
		{
			std::cerr << "Found existing entry for " << identifier << " at position " << hash << std::endl;
			return hash * -1; // already in list
		}

		hash = quadraticHash(hash, count);
		std::cerr << "Quadratic hash for count " << count << " is " << hash << std::endl;

		if (hash < 0 || hash >= TABLE_SIZE) {
			std::cerr << "Error: Hash value " << hash << " is out of bounds" << std::endl;
			return -100000; // invalid hash value
		}

		if (count >= TABLE_SIZE)
		{
			std::cerr << "Error: Hashtable is full while finding hash for " << identifier << std::endl;
			return -100000; // no space in table -> if the hashtable has to be bigger, then this whole function would need a rework
		}

		count++;
	}
	std::cerr << "Final hash for " << identifier << " is " << hash << std::endl;
	return hash;
}

int Hashtable::quadraticHash(int hash, int count)
{
	return ((hash + count * count) % TABLE_SIZE);
}

bool Hashtable::add(const StockEntry& entry)
{
	auto name = entry.getId()->name;

	int hash = findHash(name);

	if (hash < 0) return false; // if element is already inside or no space

	std::cout << "Added with hash: " << hash << std::endl;
	delete this->table[hash]; // free default placeholder

	this->table[hash] = new StockEntry(entry); // add new entry

	return true;
}


// this just searches for the hashed string int the table, so that should go first
bool Hashtable::remove(std::string identifier)
{
	auto entry = find(identifier);

	if (entry == nullptr) return false;

	int hash = findHash(identifier);
	
	
	std::string pastId = entry->pastName;
	
	delete entry;

	if (hash <= -100000 || hash >= 0) return false; // nothing was found

	hash *= -1; // invert to get real result


	table[hash] = new Entry(true, true, pastId);

	table[hash]->occupied = true;
	return true;
}

Entry* Hashtable::find(std::string identifier)
{
	int hash = findHash(identifier);

	if (hash <= -100000 || hash >= 0) { std::cerr << "Error: findHash returned " << identifier << std::endl; return nullptr; } // nothing was found
	
	hash *= -1; // invert to get real result
	
	if (table[hash]->empty && !table[hash]->occupied) { std::cerr << "Error: Entry at hash " << hash << " is empty and not occupied for " << identifier << std::endl;; return nullptr; } // identifier was not found in hashtable
	return this->table[hash];
}

Entry** Hashtable::getTable()
{
	return this->table;
}

Entry* const* Hashtable::currentTable() const
{
	return this->table;
}

StockEntry::StockEntry(Id id, Data* data[30]) : Entry(false, true), stockId(id)
{
	for (int i = 0; i < 30; i++)
	{
		if (data[i]) {
			this->stockData[i] = new Data(*data[i]);
		}
		else {
			this->stockData[i] = nullptr;
		}
	}
	this->pastName = id.name;
}

StockEntry::StockEntry(Id id) : Entry(false, true), stockId(id)
{
	for (int i = 0; i < 30; i++)
	{
		this->stockData[i] = nullptr;
	}

	this->pastName = id.name;
}

StockEntry::StockEntry(const StockEntry& other) : Entry(other), stockId(other.stockId)
{
	for (int i = 0; i < 30; i++)
	{
		if (other.stockData[i]) {
			this->stockData[i] = new Data(*other.stockData[i]);
		}
		else {
			this->stockData[i] = nullptr;
		}
	}

	this->empty = other.empty;
	this->occupied = other.occupied;
	this->pastName = other.pastName;
}

StockEntry::~StockEntry()
{
	for (int i = 0; i < 30; i++)
	{
		delete this->stockData[i];
	}
}

const Data* const* StockEntry::getData() const
{
	return this->stockData;
}

const Id* StockEntry::getId() const
{
	const Id* id = &stockId;
	return id;
}

bool StockEntry::setData(Data** data)
{
	for (int i = 0; i < 30; i++)
	{
		this->stockData[i] = data[i];
	}

	delete[] data;
	return true;
}


Entry::Entry(bool empty, bool occupied, std::string pastName)
{
	this->empty = empty;
	this->occupied = occupied;
	this->pastName = pastName;
}

const Data* const* Entry::getData() const
{
	return nullptr;
}

const Id* Entry::getId() const
{
	return nullptr;
}

bool Entry::setData(Data**)
{
	return false;
}

Data::Data(std::string date, double close, int volume, double open, double high, double low) : date(date), close(close), volume(volume), open(open), high(high), low(low)
{

}

Data::Data(const Data& other)
{
	this->close = other.close;
	this->date = other.date;
	this->high = other.high;
	this->low = other.low;
	this->open = other.open;
	this->volume = other.volume;
}

Id::Id(std::string name, std::string abbreviation, std::string wkn) : name(name), abbreviation(abbreviation), wkn(wkn)
{
}

Id::Id(const Id& other)
{
	this->abbreviation = other.abbreviation;
	this->name = other.name;
	this->wkn = other.wkn;
}
