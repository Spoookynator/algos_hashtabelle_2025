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

bool Commands::add(std::string) {
	// Implement the function
	return false;
}

bool Commands::del(std::string) {
	// Implement the function
	return false;
}

bool Commands::import(std::string) {
	// Implement the function
	return false;
}

bool Commands::search(std::string) {
	// Implement the function
	return false;
}

bool Commands::plot(std::string) {
	// Implement the function
	return false;
}

bool Commands::save(std::string) {
	// Implement the function
	return false;
}

bool Commands::load(std::string) {
	// Implement the function
	return false;
}

bool Commands::quit(std::string) {
	// Implement the function
	return false;
}

bool Commands::help(std::string) {
	// Implement the function
	return false;
}