#include "menu.h"
#include <iostream>
#include <string>

Menu::Menu(Hashtable* hashtable) {
	this->hashtable = hashtable;
}

int Menu::getInput()
{
	std::string input;
	std::getline(std::cin, input); // get whole line, not just one string
	
	std::string split[2];
	int pos = input.find(" ");
	
	split[0] = input.substr(0, pos); // somehow this works even if no space is given, it should be 0, but maybe thats a substr quirk
	split[1] = "";


	// adds second string only if a space was found
	if (pos != std::string::npos)
	{
		split[1] = input.substr(pos + 1);
	}

	// help functionality, this is not an extra input
	if (split[0] == "HELP" || split[0] == "help")
	{
		help();
		return 1;
	}

	// same as help, returns -1 to tell the menu to quit
	if (split[0] == "QUIT" || split[0] == "quit")
	{
		std::cout << "quitting...";
		return -1;
	}

	return executeInput(split);
}

void Menu::addInput(std::vector<std::string> keywords, funcPtr func, std::string description)
{
	if (this->inputLen < 10) // currently max inputs is 10, but that could be changed to a constant
	{
		this->inputs[inputLen++] = Input(keywords, func, description);
	}
	else {
		std::cout << "can't add more inputs\n!";
	}
}

void Menu::startLoop()
{
	int output = 1;
	
	while (output != -1)
	{
		do
		{
			if (output == 0)
			{
				std::cout << "Wrong input, type HELP to get help!\n";
			}
			output = this->getInput();
		} while (output == 0);
	}
}

bool Menu::executeInput(std::string input[2])
{
	for (int i = 0; i < inputLen; i++)
	{
		if (std::find(inputs[i].keywords.begin(), inputs[i].keywords.end(), input[0]) != inputs[i].keywords.end()) {
			return inputs[i].function(input[1], this->hashtable);
		}
	}
	
	return false;
}


void Menu::help()
{
	for (int i = 0; i < inputLen; i++)
	{
		std::cout << this->inputs[i].keywords.at(0) << ": " << this->inputs[i].description << "\n";
	}
		
	std::cout << "HELP: Commands werden erklaert.\n";
	std::cout << "QUIT: Programm wird beendet.\n";
}

Input::Input(std::vector<std::string> keywords, funcPtr func, std::string description) : keywords(keywords), function(func), description(description) {}

Input::Input()
{
	this->function = nullptr;
	this->keywords = {};
	this->description = "";
}

StockEntry Commands::createStock() {
	std::string name, abbreviation, wkn;
	std::cout << "Name: ";
	std::cin >> name;
	std::cout << "Abbreviation: ";
	std::cin >> abbreviation;
	std::cout << "WKN: ";
	std::cin >> wkn;

	Id id(name, abbreviation, wkn);
	StockEntry entry(id);
	return entry;
}

bool Commands::add(std::string, Hashtable *hashtable) {
	// Implement the function
	StockEntry entry = Commands::createStock();
	hashtable->add(entry);
	if (hashtable->find(entry.getId()->name) == nullptr) return false;
	if (hashtable->find(entry.getId()->name)->getId()->name != entry.getId()->name) return false;
	if (hashtable->find(entry.getId()->name)->getId()->abbreviation != entry.getId()->abbreviation) return false;
	if (hashtable->find(entry.getId()->name)->getId()->wkn != entry.getId()->wkn) return false;
	return true;
}

bool Commands::del(std::string, Hashtable* hashtable) {
	// Implement the function
	StockEntry entry = Commands::createStock();
	hashtable->remove(entry.getId()->name);
	if (hashtable->find(entry.getId()->name) != nullptr) return false;
	if (hashtable->remove(entry.getId()->name) != false) return false;
	return true;
}

bool Commands::import(std::string, Hashtable* hashtable) {
	// Implement the function
	return false;
}

bool Commands::search(std::string, Hashtable* hashtable) {
	// Implement the function
	StockEntry entry = Commands::createStock();
	Entry* found = hashtable->find(entry.getId()->name);
	if (hashtable->find(entry.getId()->name) == nullptr) return false;
	if (hashtable->find(entry.getId()->name)->getId()->name != entry.getId()->name) return false;
	if (hashtable->find(entry.getId()->name)->getId()->abbreviation != entry.getId()->abbreviation) return false;
	if (hashtable->find(entry.getId()->name)->getId()->wkn != entry.getId()->wkn) return false;
	displayStock(found);
	return false;
}

bool Commands::plot(std::string, Hashtable* hashtable) {
	// Implement the function
	return false;
}

bool Commands::save(std::string, Hashtable* hashtable) {
	// Implement the function
	return false;
}

bool Commands::load(std::string, Hashtable* hashtable) {
	// Implement the function
	return false;
}

void Commands::displayStock(Entry* entry) {
	

	std::cout << "Name: " << entry->getId()->name << "\n";
	std::cout << "Abbreviation: " << entry->getId()->abbreviation << "\n";
	std::cout << "WKN: " << entry->getId()->wkn << "\n";
	std::cout << "----------------------------------\n";
	std::cout << "Data:\n";
	std::cout << "----------------------------------\n";
	for (int i = 0; i < 30; i++) {
		/*
		* Go through the data and print it
		* Commented out due to no immport function implemented yet to fill the stockdata
		* std::string date = entry->getData()[i]->date;
		* double close = entry->getData()[i]->close;
		* int volume = entry->getData()[i]->volume;
		* double open = entry->getData()[i]->open;
		* double high = entry->getData()[i]->high;
		* double low = entry->getData()[i]->low;
		* std::cout << "Date: " << date << "\n";
		* std::cout << "Close: " << close << "\n";
		* std::cout << "Volume: " << volume << "\n";
		* std::cout << "Open: " << open << "\n";
		* std::cout << "High: " << high << "\n";
		* std::cout << "Low: " << low << "\n";
		* std::cout << "----------------------------------\n";
		*/
		
	}
	
}