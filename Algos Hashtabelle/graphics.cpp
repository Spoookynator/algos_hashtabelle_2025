#include "hashtable.h"
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

bool plotStock(std::string stock, Hashtable* hashtable) {
	auto element = hashtable->find(stock);

	if (element == nullptr) return false;

	if (element->getData() == nullptr) return false;

    auto data = element->getData();

    // min/max for scaling the graph 
    double minVal = 1e9, maxVal = -1e9; 
    for (int i = 0; i < 30; i++) {
        if (data[i]) {
            minVal = std::min(minVal, data[i]->close);
            maxVal = std::max(maxVal, data[i]->close);
        }
    }

    const int height = 20;  // graph hight

    for (int y = height; y >= 0; y--) {
        double value = minVal + ((maxVal - minVal) * y / height);

        // setw for always allocating 7 space to the num, fixed for fixed pos and setprec for the decimals
        std::cout << std::setw(7) << std::fixed << std::setprecision(2) << value << " | ";

        for (int x = 29; x >=0; x--) {
            if (!data[x]) {
                std::cout << "/";
                continue;
            }

            double close = data[x]->close;

            // checks if the value is close enough and then prints it
            if (std::abs(value - close) < (maxVal - minVal) / height / 2)
                std::cout << "o"; 
            else
                std::cout << ".";
        }
        std::cout << "\n";
    }

    // x axis on bottom for days
    std::cout << "        +";
    for (int i = 0; i < 30; i++)
        std::cout << "-";
    std::cout << "\n        ";
    for (int i = 0; i < 30; i++)
        std::cout << ((i % 5 == 0) ? "|" : " ");
    std::cout << "\n";
}