#pragma once
#include <chrono>
class dijkstra_algo
{
public:
	int Nnodes, cycles;
	vector<vector<int>> target_routes;
	vector<int> checked_nodes;
	chrono::duration<double> elapsed_time;

	struct n
	{
		float dist;
		int parent_node;
		int done;
	};
	vector<n> n_info;

	void set_size(int N)
	{
		target_routes.clear();
		checked_nodes.clear();
		n_info.clear();
		Nnodes = N;
		n_info.resize(N);
		target_routes.resize(N);
	}


	void pathing(int i)
	{
		int j = i;

		while (n_info[i].parent_node != -1)
		{
			target_routes[j].push_back(i);
			i = n_info[i].parent_node;
		}
		target_routes[j].push_back(i);
	}

	void path_all()
	{
		for (int i = 0; i < Nnodes; i++)
		{
			pathing(i);
		}
	}

	void print_pathing(vector<string>& location)
	{
		for (int i = 0; i < Nnodes; i++)
		{
			int routes_len = target_routes[i].size();
			for (int j = routes_len - 1; j >= 0; j--)
			{
				if (j != routes_len - 1)
				{
					cout << "->";
				}
				cout << location[target_routes[i][j]];
			}
			cout << "(" << n_info[i].dist << ")\n";
		}
	}

	void print_dest_path(int index, vector<string>& location)
	{
		int routes_len = target_routes[index].size();
		for (int j = routes_len - 1; j >= 0; j--)
		{
			if (j != routes_len - 1)
			{
				cout << "->";
			}
			cout << location[target_routes[index][j]];
		}
		cout << "(" << n_info[index].dist << ")\n";
	}

	void init(int start_n, vector<vector<float>>& dist_matr)
	{
		int i;
		checked_nodes.push_back(start_n);
		for (i = 0; i < Nnodes; i++)
		{
			n_info[i].done = 0;
			n_info[i].dist = dist_matr[start_n][i];
			n_info[i].parent_node = start_n;
		}
		n_info[start_n].done = 1;
		n_info[start_n].parent_node = -1;
	}

	int node_select()
	{
		int i, minpos; int min;
		min = xxx;
		minpos = -1;
		for (i = 0; i < Nnodes; i++) {
			if ((n_info[i].dist < min) && (n_info[i].done == 0))
			{
				min = n_info[i].dist; minpos = i;
			}
		}
		return minpos;
	}

	vector<int> dijkstra(int start_n, int end, vector<vector<float>>& dist_matr, bool median)
	{
		int Nodes, i, j, node, k, d;
		Nodes = dist_matr.size();
		elapsed_time.zero();

		if (median == true) {
			j = 100;
		}
		else {
			j = 1;
		}

		for (int n = 0; n < j; n++) {

			set_size(Nodes);
			init(start_n, dist_matr);

			cycles = 0;
			auto start = chrono::high_resolution_clock::now();

			for (i = 0; i < Nnodes - 2; i++) {
				node = node_select();
				n_info[node].done = 1;
				checked_nodes.push_back(node);
				if (node == end) break;
				for (k = 0; k < Nnodes; k++) {
					if (!(n_info[k].done == 1)) {
						d = n_info[node].dist + dist_matr[node][k];
						if (d < n_info[k].dist) {
							n_info[k].dist = d;
							n_info[k].parent_node = node;
						}
					}
				}
			}
			cycles = i + 1;
			path_all();

			auto finish = chrono::high_resolution_clock::now();
			elapsed_time += finish - start;
		}
		elapsed_time = elapsed_time / j;
		return target_routes[end];
	}
};
