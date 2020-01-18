#pragma once
#include <string>
#include <vector>
#include "Lexikalische_Analyse.h"
using namespace std;

class City
{
public:
	City();


	void set_neighbours(vector<string>);		//Methode zum schreiben von Nachbarn aus der Lex. Analyse als Eigenschaften des Objekts
	void set_pos(vector<float>);				//Methode zum schreiben der Position aus der Lex. Analyse als Eigenschaft des Objekts
	void set_Time(vector<float>);				//Methode zum schreiben von Kostenfaktoren aus der Lex. Analyse als Eigenschaften des Objekts
	void set_City_Name(string);					//Methode zum schreiben de Stadtnamesn aus der Lex. Analyse als Eigenschaft des Objekts

	vector<float> pos = { 0,0 };				//Positionsvektor
	string City_Name;							//Statdname
	vector<string> Neighbours;					//Vektor enthält alle Nachbarn der Stadt
	vector<float> Time;							//Vektor enthält die Kostenfaktoren zum entsprechenden Nachbar


};
City::City()
{
	Time = { 0 };								// Initalisierung im Kontruktor
}// Leer

void City::set_neighbours(vector<string> s)		//Schreibt übergeben Nachbarn in den Vektor
{
	Neighbours = s;			
}

void City::set_pos(vector<float> i)				//Schreibt übergebenen Positionsvekotr in den Objekteigenen Vektor
{
	pos = i;
}

void City::set_Time(vector<float> i)			//Schreibt übergebenen Zeitvektor in den Objekteigenen Vektor
{
	Time = i;

}
void City::set_City_Name(string Name)			//Schreibt übergebenen Stadtnamen in die Objektvarible
{
	City_Name = Name;
}



