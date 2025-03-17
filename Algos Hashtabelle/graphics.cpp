#include <string>
#include <iostream>
#include "hashtable.h"


// wip, should first create the import/export before I can implement and test this
bool plotStock(std::string stock, Hashtable* hashtable) {
	auto element = hashtable->find(stock);

	if (element == nullptr) return false;

	if (element->getData() == nullptr) return false;

	for (int i = 0; i < 30; i++)
	{
		std::cout << "high" << element->getData()[i]->high;
		std::cout << " low" << element->getData()[i]->low << std::endl;
	}
}