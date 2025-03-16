#pragma once
#include <iostream>
#include <vector>
#include <string>
#define TABLE_SIZE 2003 //next largest prime number after 2000, reflecting the intended capacity of 1000 total entries and 50% load factor


struct Data{
	Data(std::string, double, int, double, double, double);
	Data(const Data&); // copy constructor

	std::string date; // format: mm/dd/yyyy
	double close;
	int volume;
	double open;
	double high;
	double low;
};

struct Id {
	Id(std::string, std::string);
	std::string name;
	std::string abbreviation;
};

struct Entry
{
	Entry(bool = true, bool = false);
	virtual ~Entry() = default;
	bool occupied; // tracks if the stock was occupied before, for safe deletion
	bool empty;

	// double const so the pointer cant be changed and the value also cant be changed
	// this returns nullptr when there is no data stored
	virtual const Data* const* getData() const;
	virtual const Id* const getId() const;

	bool isOccupied() const;
	bool isEmpty() const;
};

struct StockEntry : public Entry {

	StockEntry(Id, Data*[30]);
	~StockEntry();
	Id stockId;
	Data* stockData[30] = {};

	const Data* const * getData() const override;
	const Id* const getId() const override;
};

class Hashtable {
public:
	Hashtable();

	int calculateHash(std::string abbreviation);

private:
	Entry* table[TABLE_SIZE];
};
