#include "menu.h"
#include <iostream>
#include <string>
#include <array>
#include "graphics.h"
#include "save_load.h"

Menu::Menu(Hashtable* hashtable) {
	this->hashtable = hashtable;
}

int Menu::getInput()
{
	std::cout << "----------------------------------\n";
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
				std::cout << "Wrong input or wrong arguments, type HELP to get help!\n";
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

bool Commands::add(std::string args, Hashtable *hashtable) {

	std::array<std::string, 3> tokens;
	size_t start = 0, end;
	char delimiter = ',';
	
	try
	{
		// Find first delimiter
		end = args.find(delimiter, start);
		tokens[0] = args.substr(start, end - start);
		start = end + 1;

		// Find second delimiter
		end = args.find(delimiter, start);
		tokens[1] = args.substr(start, end - start);
		start = end + 1;

		// Remaining part
		tokens[2] = args.substr(start);

		if (tokens[0] == args || tokens[1] == args || tokens[2] == args) return false;

		Id id(tokens[0], tokens[1], tokens[2]);
		StockEntry entry(id);

		bool res = hashtable->add(entry);

		if (!res)
		{
			std::cout << "Element already inside hashtable!\n";
			return false;
		}
		else {
			std::cout << "Added element to hashtable!\n";
			return true;
		}

	}
	catch (...)
	{
		return false;
	}
	
}

bool Commands::del(std::string args, Hashtable* hashtable)
{
	bool res = hashtable->remove(args);
	
	if (res)
	{
		std::cout << "Sucessfully deleted entry!\n";
		return true;
	}
	else {
		return false; 
	}
}

bool Commands::import(std::string args, Hashtable* hashtable) {

	auto res = hashtable->import(args);

	if (res)
	{
		std::cout << "Import successful!\n";
		return true;
	}
	return false;
}

bool Commands::search(std::string args, Hashtable* hashtable) {

	auto entry = hashtable->find(args);

	if (entry == nullptr)  
	{
		std::cout << "'" << args << "' not found!\n";
		return false;
	} 

	displayStock(entry);
	return true;
}

bool Commands::plot(std::string args, Hashtable* hashtable) {
	return plotStock(args, hashtable);
}

bool Commands::save(std::string args, Hashtable* hashtable) {  
   
	if (!SaveLoad::save(*hashtable, args)) {
		std::cout << "Error saving hashtable data!\n";
		return false;
	}
	std::cout << "Save successful!\n";
	return true;
}

bool Commands::load(std::string args, Hashtable* hashtable) {
	// Implement the function
	if (!SaveLoad::load(*hashtable, args)) {
		std::cout << "Error loading hashtable data!\n";
		return false;
	}
	std::cout << "Load successful!\n";
	return true;
}

void Commands::displayStock(Entry* entry) {
	std::cout << "Name: " << entry->getId()->name << "\n";
	std::cout << "Abbreviation: " << entry->getId()->abbreviation << "\n";
	std::cout << "WKN: " << entry->getId()->wkn << "\n";
	std::cout << "----------------------------------\n";
}