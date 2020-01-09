
#include <algorithm>
#include <iomanip>
#pragma once

void Lex_Ausgabe_Stadt(unsigned int, map<int, City*>);
vector<vector<float>> Lex_Adjazenzmatrix(vector<string>, map<int, City*>);
vector<string> Lex_Drop_Down(map<int, City*>);
vector<vector<float>> Lex_Positions(map<int, City*>);

#define xxx 10000


// ihr Nasen

/*
Funktion zur Erstellung der nxn-Adjazenzmatrix für die Algorithmen

-> bekommt map im Format: map<std::string, City*> übergeben die alle eingelesenen Städte enthält

-- Dynamisch erstellter Vector mit allen Städtenamen
-- Kann durch einkommentieren von: "std::sort(all.begin(), all.end());" auch sortiert zurückgegeben werden

<- gibt Vector mit allen Städtenamen der übergebenen Map zurück
*/


vector<string> Lex_Drop_Down(map<int, City*>a_map)
{
	vector<string> all;


	for (int x = 0; x < a_map.size(); x++) {

		all.push_back(a_map[x]->City_Name);

	}
	//std::sort(all.begin(), all.end());
	return all;
}

vector<vector<float>> Lex_Positions(map<int, City*> a_map)
{
	vector<vector<float>> X_Y_Pos;
	vector<float> pos;


	int size = a_map.size();

	for (int i = 0; i < size; i++)
	{
		pos = a_map[i]->pos;
		X_Y_Pos.push_back(pos);
	}

	//for (int c = 0; c < size; c++)
	//{
	//	
	//	cout << "{" << X_Y_Pos[c][0] << "," << X_Y_Pos[c][1] << "}"<< endl;

	//}



	return X_Y_Pos;
}



/*
Funktion zur Ausgabe einer bestimmten Stadt

-> bekommt einen String (Städtename) und eine map im Format: map<std::string, City*>, die alle eingelesenen Städte enthält übergeben

*/

void Lex_Ausgabe_Stadt(unsigned int ind, map<int, City*>a_map)
{

	std::cout << a_map[ind]->City_Name << ":" << endl;//Name der Stadt
	std::cout << "POS: " << a_map[ind]->pos[0] << "," << a_map[ind]->pos[1] << endl;//Position auf der Karte

	cout << "Neighbours: {";// Liste der Nachbarn
	for (vector<int>::size_type i = 0; i < a_map[ind]->Neighbours.size(); ++i)
	{
		cout << a_map[ind]->Neighbours[i] << " ";
	}
	cout << "}" << endl;
	cout << "Time: {";// Liste der Zeitfaktoren
	for (vector<int>::size_type i = 0; i < a_map[ind]->Time.size(); ++i)
	{
		cout << a_map[ind]->Time[i] << " ";
	}
	cout << "}" << endl;
}



/*
Funktion zur Erstellung der nxn-Adjazenzmatrix für die Algorithmen 

-> bekommt map im Format: map<std::string, City*> übergeben die alle eingelesenen Städte enthält

-- Dynamische Anpassung der Vektoren an die Anzahl der Städte
-- Trägt die Abstände zu den nächsten Nachbarn ein
-- Trägt xxx (10000) ein wenn eine Stadt kein Nachbar ist
-- Trägt 0 auf der Diagonalen ein (Fall Bsp: Berlin - Berlin )

<- gibt Adjazenzmatrix zurück
*/

vector<vector<float>> Lex_Adjazenzmatrix( map<int, City*>a_map)
{
	vector<vector<float>> nxn;
	vector<string>All_City_Names;


	All_City_Names = Lex_Drop_Down(a_map);

	int n = All_City_Names.size(); // Naechste 4 Zeilen: Dynamische Anpassung der an den Algorithmus zu übergebenden Map
	
	nxn.resize(n);


	for (int i = 0; i < n; ++i)
		nxn[i].resize(n);


	for (vector<int>::size_type i = 0; i <n; ++i)
	{

		for (vector<int>::size_type j = 0; j < n; ++j)
		{

			vector<string> NB = a_map[i]->Neighbours;


			std::vector<string>::iterator it = find(NB.begin(), NB.end(), All_City_Names[j]); // schaut ob Stadt (der inneren Schleife) eine Nachbarstadt der Stadt in der auesseren Schleife ist

			if (All_City_Names[i] == All_City_Names[j]) //Traegt 0 ein
			{
				nxn[i][j] = 0;
			}

			else if (it != NB.end()) //Traegt Zeit ein
			{

				int ind = distance(NB.begin(), it);

				int x = (a_map[i]->pos[0] - a_map[j]->pos[0]);
				int y = (a_map[i]->pos[1] - a_map[j]->pos[1]);

				nxn[i][j] = (int)(sqrt(x*x + y*y)*(a_map[i]->Time[ind])/10);
			}

			else // Traegt inf. ein
			{
				nxn[i][j] = xxx;
			}

		}
		//for (vector<int>::size_type d = 0; d < nxn[i].size(); ++d)// Ausgabe eines kompletten Vectors
		//{
		//	cout << setw(4) << nxn[i][d] << ", \t";
		//}
		//cout << endl;
	}
	return nxn;
}
