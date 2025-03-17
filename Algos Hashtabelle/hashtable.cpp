#include "hashtable.h"

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
	// hash function
	int hashIndex = 0;
	for (int i = 0; i < hashString.length(); i++) {
		hashIndex += hashString[i];

	}
	hashIndex = hashIndex % TABLE_SIZE; //2003
	return hashIndex;
}

int Hashtable::findHash(std::string identifier)
{
	int hash = calculateHash(identifier); 

	int count = 0;
	while (!this->table[hash]->empty || this->table[hash]->occupied)
	{
		hash = quadraticHash(hash, count);

		if (count >= TABLE_SIZE)
		{
			return -100000; // no space in table -> if the hashtable has to be bigger, then this whole function would need a rework
		}

		if (this->table[hash]->getId() != nullptr && this->table[hash]->getId()->name == identifier) // we could also change that to wkn or sth
		{
			return hash * -1; // already in list
		}

		count++;
	}

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
	
	delete entry;

	if (hash <= -100000 || hash >= 0) return false; // nothing was found

	hash *= -1; // invert to get real result
	table[hash] = new Entry();

	table[hash]->occupied = true;
	return true;
}

Entry* Hashtable::find(std::string identifier)
{
	int hash = findHash(identifier);

	if (hash <= -100000 || hash >= 0) return nullptr; // nothing was found
	
	hash *= -1; // invert to get real result

	if (table[hash]->empty) return nullptr; // identifier was not found in hashtable
	return this->table[hash];
}

StockEntry::StockEntry(Id id, Data* data[30]) : Entry(false, true), stockId(id)
{
	for (int i = 0; i < 30; i++)
	{
		if (data[i]) {
			this->stockData[i] = new Data(*data[i]);
		}
	}
}

StockEntry::StockEntry(Id id) : Entry(false, true), stockId(id)
{
	for (int i = 0; i < 30; i++)
	{
		this->stockData[i] = nullptr;
	}

}

StockEntry::StockEntry(const StockEntry& other) : stockId(other.stockId)
{
	for (int i = 0; i < 30; i++)
	{
		this->stockData[i] = other.stockData[i];
	}

	this->empty = other.empty;
	this->occupied = other.occupied;
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

Entry::Entry(bool empty, bool occupied)
{
	this->empty = empty;
	this->occupied = occupied;
}

const Data* const* Entry::getData() const
{
	return nullptr;
}

const Id* Entry::getId() const
{
	return nullptr;
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
