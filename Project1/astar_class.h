#pragma once
#include <chrono>
class astar_algo
{
public:
	int Nnodes, cycles;								//Anzahl Knoten
	vector<vector<int>> target_routes;				//Vector mit den k�rzesten Wegen zu den abgearbeiteten Knoten
	vector<int> checked_nodes;						//Vector mit den Index Nummern der abgearbeiteten Knoten, in reihenfolge
	chrono::duration<double> elapsed_time;			//Variable zur Speicherung der Laufzeit des Algorithmus

	struct n										//Struktur mit allen Knoteninformationen
	{
		float x_coord;								//X-Koordinate des Knoten
		float y_coord;								//Y-Koordinate des Knoten
		float h_dist;								//heuristische Distanz zum Zielknoten
		float dist;									//reale Distanz zum Startknoten
		int parent_node;							//Vorheriger Knoten zum Startpunkt
		int done;									//Knoten abgearbeitet 0 / 1
	};
	vector<n> n_info;								//Vector mit datentyp der Informationsstruktur

	void set_size(int N)							//Methode zur Skalierung aller Vectoren
	{
		target_routes.clear();						//Vector.clear() setzt die L�nge des Vectors auf Null (Vector leeren)
		checked_nodes.clear();
		n_info.clear();
		Nnodes = N;									//Festlegen der St�dteanzahl und skalieren der Vectoren auf die Knotenanzahl
		n_info.resize(N);
		target_routes.resize(N);
	}


	void pathing(int i)								//Methode zum f�llen des target_routes Vectors
	{
		int j = i;									//Index des Knoten f�r den der Weg zum Startknoten aufgebaut werden soll

		while (n_info[i].parent_node != -1)			//Schleife wird ausgef�hrt bis der Startknoten erreicht wird
		{
			target_routes[j].push_back(i);			//Wert in parent_node wird an target_routes angeh�ngt 
			i = n_info[i].parent_node;				//n�chster betrachteter Knoten ist der in parent_nodes hinterlegte Index
		}
		target_routes[j].push_back(i);				//Zielknoten anh�ngen
	}

	void path_all()									//Methode zum ausf�hren der pathing() Methode �ber alle Knoten
	{
		for (int i = 0; i < Nnodes; i++)
		{
			pathing(i);
		}
	}

	void init(int start_n, vector<vector<float>>& dist_matr, vector<vector<float>>& coords, int end_n)							//Initialisierung der Informationsstruktur
	{
		int i;
		float end_x, end_y;

		end_x = coords[end_n][0];																								//Koordinaten des Zielknoten �bernehmen
		end_y = coords[end_n][1];																								
		checked_nodes.push_back(start_n);																						//Startknoten Index an checked_nodes vector anh�ngen

		for (i = 0; i < Nnodes; i++)																							//Schleife zur initialisierung
		{
			n_info[i].x_coord = coords[i][0];																					//X, Y Koordinaten
			n_info[i].y_coord = coords[i][1];
			n_info[i].h_dist = sqrtf(powf(abs(end_x - n_info[i].x_coord), 2) +  powf(abs(end_y - n_info[i].y_coord),2));		//heuristische Distanz zum Zielknoten Pythagoras
			n_info[i].done = 0;																									//Done = 0 f�r Knoten noch nicht abgearbeitet
			n_info[i].dist = dist_matr[start_n][i];																				//Streckenbewertung zum Startknoten
			n_info[i].parent_node = start_n;																					//Index des vorhergehenden Knoten, bei init ist hier der Startknoten angegeben
		}
		n_info[start_n].done = 1;																								//Startknoten auf done=1 setzten, Knoten ist abgearbeitet
		n_info[start_n].parent_node = -1;																						//Vorhergehender Knoten f�r Startknoten ist -1 (kein vorhergehender Knoten)
	}

	int node_select()																											//Methode zur Auswahl des n�chsten zu bearbeitenden Knoten
	{
		int i, minpos; 
		float min;
		min = xxx;																												//Referenz-Minimum wird mit unendlich initialisiert
		minpos = -1;
		for (i = 0; i < Nnodes; i++) {																							//Schleife �ber alle Knoten
			if (((n_info[i].dist+n_info[i].h_dist) < min) && (n_info[i].done == 0))												//Wenn die h_dist+dist kleiner als das Referenz-Minimum sind wird dieser Wert als neue Min. Referenz gesetzt
			{
				min = n_info[i].dist + n_info[i].h_dist;
				minpos = i;																										//index speichern
			}
		}
		return minpos;																											//R�ckgabe des Index f�r den n�chsten abzuarbeitenden Index
	}

	vector<int> astar(int start_n, int end, vector<vector<float>>& dist_matr, vector<vector<float>>&coords, bool median)		//Methode A* Algorithmus
	{
		int Nodes, i, j, node, k, d;
		Nodes = dist_matr.size();																								//Knotenanzahl bestimmen
		elapsed_time.zero();																									//Zeitwert mit Null initialisieren

		if (median == true) {																									//Anzahl festlegen wie oft der Algorithmus ausgef�hrt wird
			j = 10000;
		}
		else {
			j = 1;
		}
		auto start = chrono::high_resolution_clock::now();																		//Start Zeitmessung
		for (int n = 0; n < j; n++) {

			set_size(Nodes);																									//Vectorl�ngen der Informationsstruktur skalieren
			init(start_n, dist_matr, coords, end);																				//Informationsstruktur initialisieren
			cycles = 0;																											//Algorithmus interne Durchl�ufe: Z�hler auf 0 setzen

			for (i = 0; i < Nnodes - 2; i++) {																					//Schleife �ber die Knotenanzahl -2
				node = node_select();																							//zu bearbeitenden Knoten ausw�hlen
				n_info[node].done = 1;																							//ausgew�hlten Knoten auf abgearbeitet setzen
				checked_nodes.push_back(node);																					//Index des Knoten an checken_nodes vector anh�ngen
				if (node == end) break;																							//Wenn der angew�hlte Knoten gleich dem Zielknoten ist wird der Algorithmus beendet
				for (k = 0; k < Nnodes; k++) {																					//Schleife �ber alle Knoten
					if (!(n_info[k].done == 1)) {																				//Nur Knoten betrachten die noch nicht abgearbeitet sind
						d = n_info[node].dist + dist_matr[node][k];																//Distanz zu umliegenden Knoten + Distanz zum Startknoten bilden
						if (d < n_info[k].dist) {																				//Wenn diese Distanz kleiner der hinterlegten ist: Distanz �berschreiben und parent_node eintragen
							n_info[k].dist = d;
							n_info[k].parent_node = node;
						}
					}
				}
			}
			cycles = i + 1;																										//Durchlaufz�hler erh�hen
			path_all();																											//Alle Wege in target_routes eintragen
		}
		auto finish = chrono::high_resolution_clock::now();																		//Ende Zeitmessung
		elapsed_time += finish - start;																							//Vergangene Zeit berechnen
		elapsed_time = elapsed_time / j;																						//Zeitmittelwert berechnen
		return target_routes[end];																								//R�ckgabe des Weges Start zu Ziel
	}
};
