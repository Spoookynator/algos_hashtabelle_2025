#include <iostream>
#include <string>
#include "tests.h"
#include "menu.h"
#include "hashtable.h"

int main() {
	//allTests();
	Hashtable* hashtable = new Hashtable();
	Menu menu(hashtable);
	
	menu.addInput({ "ADD", "add", "Add", "a"}, Commands::add, "Fuegt eine Aktie hinzu (name,kuerzel,wkn)");
	menu.addInput({ "DEL", "del", "Del", "d"}, Commands::del, "Aktie wird geloescht");
	menu.addInput({ "IMPORT", "import", "Import", "i"}, Commands::import, "Kurswerte fuer eine Aktie werden aus einer csv Datei importiert (Namen ohne Dateiendung angeben - csv erwarted und Name muss mit dem Aktiennamen uebereinstimmen)");
	menu.addInput({ "SEARCH", "search", "Search" "s"}, Commands::search, "Eine Aktie wird in der Hashtabelle gesucht und ausgegeben.");
	menu.addInput({ "PLOT", "plot", "Plot", "p"}, Commands::plot, "Die Schlusskurse der letzten 30 TAage einer Aktie werden ausgegeben.");
	menu.addInput({ "SAVE", "save", "Save", "sv"}, Commands::save, "Programm speichert die Hashtabelle in eine Datei ab.");
	menu.addInput({ "LOAD", "load", "Load", "l"}, Commands::load, "Programm laedt die Hashtabelle aus einer Datei.");
	
	//print menu
	std::cout << "\n----------------------------------\n";
	std::cout << "Aktienkurs-Verwaltungs-System - Uebung 1 ALGODAT\n";
	std::cout << "Menuepunkte:\n";
	std::cout << "ADD, DEL, IMPORT, SEARCH, PLOT, SAVE, LOAD, QUIT\n";
	std::cout << "HELP: Zeigt alle Befehle in Detail an\n";

	menu.startLoop();
}