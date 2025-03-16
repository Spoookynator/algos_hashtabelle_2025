#include "hashtable.h"

Hashtable::Hashtable() {

	// init table
	for (int i = 0; i < 30; i++)
	{
		table[i] = new Entry();
	}
}

int Hashtable::calculateHash(std::string abbreviation) {
	// hash function
	int hashIndex = 0;
	for (int i = 0; i < abbreviation.length(); i++) {
		hashIndex += abbreviation[i];
	}
	hashIndex = hashIndex % TABLE_SIZE; //2003
	return hashIndex;
}

StockEntry::StockEntry(Id id, Data* data[30]) : Entry(false, true), stockId(id)
{
	for (int i = 0; i < 30; i++)
	{
		if (data[i]) {
			this->stockData[i] = new Data(*data[i]);
		}
	}
	this->occupied = false;
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

const Id* const StockEntry::getId() const
{
	return &stockId;
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

const Id* const Entry::getId() const
{
	return nullptr;
}

bool Entry::isOccupied() const
{
	return occupied;
}

bool Entry::isEmpty() const
{
	return empty;
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

Id::Id(std::string name, std::string abbreviation) : name(name), abbreviation(abbreviation)
{
}
