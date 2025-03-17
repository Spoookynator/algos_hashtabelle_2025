#pragma once
#include <vector>
#include <string>

typedef bool(*funcPtr)(std::string);

struct Input
{
	/// <summary>
	/// New input element
	/// </summary>
	/// <param name="Keywords">(adds keywords for command recognition)</param>
	/// <param name="Bound Function"></param>
	/// <param name="Description">(adds description for help command)</param>
	Input(std::vector<std::string>, funcPtr, std::string = "");
	Input();

	std::vector<std::string> keywords;
	funcPtr function;
	std::string description;
};

class Menu
{
public:
	Menu();

	/// <summary>
	/// Bind inputs to the menus
	/// </summary>
	/// <param name="Keywords"></param>
	/// <param name="Bound Function"></param>
	/// <param name="Description"></param>
	void addInput(std::vector<std::string>, funcPtr, std::string);

	// starts the main loop, all operations will happen here
	void startLoop();

private:
	// gets input from user, returns 0 for false, 1 for true and -1 for exit program
	int getInput();

	// prints description of all inputs (+ the default quit option)
	void help();

	/// <summary>
	/// Executes input with parameters given, needs Input struct to work
	/// </summary>
	/// <param name="Input">(Input struct with keywords and func poiner)</param>
	/// <returns></returns>
	bool executeInput(std::string[2]);


	short int inputLen = 0;
	Input inputs[10] = {};
};

namespace menu {
bool add(std::string);
bool del(std::string);
bool import(std::string);
bool search(std::string);
bool plot(std::string);
bool save(std::string);
bool load(std::string);
bool quit(std::string);
bool help(std::string);
}