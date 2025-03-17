#include <iostream>
#include <string>
#include "tests.h"
#include "menu.h"
#include "hashtable.h"

int main() {
	allTests();
	Menu menu;
	
	/*
	* TODO: Add commands to menu
	* menu.addInput({ "ADD" }, hashtable->add(entry), "Fuegt eine Aktie hinzu");
	* menu.addInput({ "SEARCH" }, hashtable->find(stockName), "Eine Aktie wird in der Hashtabelle gesucht und ausgegeben.");
	* menu.addInput({ "DEL" }), hashtable->remove(stockName), "Aktie wird gelöscht");
	*/

	//print menu
	std::cout << "\n----------------------------------\n";
	std::cout << "Aktienkurs-Verwaltungs-System - Uebung 1 ALGODAT\n";
	std::cout << "Menuepunkte:\n";
	std::cout << "ADD, DEL, IMPORT, SEARCH, PLOT, SAVE, LOAD, QUIT\n";
	std::cout << "HELP: Zeigt alle Befehle in Detail an\n";
	std::cout << "----------------------------------\n";
	menu.startLoop();
}