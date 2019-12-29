#pragma once
#include <string>
#include <vector>
#include "Lexikalische_Analyse.h"
using namespace std;

class City
{
public:
	City();


	void set_neighbours(vector<string>);
	void set_pos(vector<float>);
	//void set_Time(vector<float>);
	void set_City_Name(string);

	vector<float> pos = { 0,0 };
	string City_Name;
	vector<string> Neighbours;
	//vector<float> Time;


};
City::City()
{
	
}// Leer

void City::set_neighbours(vector<string> s)
{
	Neighbours = s;
}

void City::set_pos(vector<float> i)
{
	pos = i;
}

/*void City::set_Time(vector<float> i)
{
	Time = i;

}*/
void City::set_City_Name(string Name)
{
	City_Name = Name;
}



