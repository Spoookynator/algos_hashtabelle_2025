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

/// <summary>
/// 
/// </summary>
/// <param name="name"></param>
/// <param name="abbreviation"></param>
/// <param name="wkn"></param>
struct Id {
	Id(std::string, std::string, std::string);
	Id(const Id&);
	std::string name;
	std::string abbreviation;
	std::string wkn;
};

struct Entry
{
	Entry(bool = true, bool = false, std::string = "");
	virtual ~Entry() = default;
	bool occupied; // tracks if the stock was occupied before, for safe deletion
	bool empty;

	std::string pastName;

	// double const so the pointer cant be changed and the value also cant be changed
	// this returns nullptr when there is no data stored
	virtual const Data* const* getData() const;
	virtual const Id* getId() const;

	virtual bool setData(Data**);
};

struct StockEntry : public Entry {

	StockEntry(Id, Data*[30]);
	StockEntry(Id);
	StockEntry(const StockEntry&);

	~StockEntry();

	Id stockId;
	Data* stockData[30] = {};

	const Data* const * getData() const override;
	const Id* getId() const override;

	bool setData(Data**);
};

class Hashtable {
public:
	Hashtable();
	~Hashtable();

	bool add(const StockEntry&);

	bool remove(std::string);

	Entry* find(std::string);

	Entry** getTable();

	Entry* const* currentTable() const;

	int calculateHash(const std::string&);

	bool import(std::string);

	bool loadTableEntries(StockEntry& entry, int index);

	bool loadTableEntries(Entry pastEntry, int index);
	
private:

	Entry* table[TABLE_SIZE];
	int findHash(std::string identifier);
	int quadraticHash(int, int);
};
