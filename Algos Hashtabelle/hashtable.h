#pragma once
#include <iostream>
#include <vector>
#include <string>
#define TABLE_SIZE 2003 //next largest prime number after 2000, reflecting the intended capacity of 1000 total entries and 50% load factor

using std::string;

class hashtable {
public:
	hashtable();
	int calculateHash(string abbreviation);

private:
	typedef struct {
		string name;
		string abbreviation;
	} id;

	typedef struct {
		string date; // format: mm/dd/yyyy
		double close;
		int volume;
		double open;
		double high;
		double low;
	} data;

	typedef struct {
		id stockId;
		data stockData[30];
	} stockEntry;

	stockEntry table[TABLE_SIZE];
};