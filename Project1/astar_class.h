
class astar_algo
{
public:
	int Nnodes, cycles;
	vector<vector<int>> target_routes;
	chrono::duration<double> elapsed_time;

	struct n
	{
		float x_coord;
		float y_coord;
		float h_dist;
		float dist;
		int parent_node;
		int done;
	};
	vector<n> n_info;

	void set_size(int N)
	{
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

	void init(int start_n, vector<vector<float>>& dist_matr, vector<vector<float>>& coords, int end_n)
	{
		int i;
		float end_x, end_y;

		end_x = coords[end_n][0];
		end_y = coords[end_n][1];

		for (i = 0; i < Nnodes; i++)
		{
			n_info[i].x_coord = coords[i][0];
			n_info[i].y_coord = coords[i][1];
			n_info[i].h_dist = sqrtf(powf(abs(end_x - n_info[i].x_coord), 2) +  powf(abs(end_y - n_info[i].y_coord),2));
			n_info[i].done = 0;
			n_info[i].dist = dist_matr[start_n][i];
			n_info[i].parent_node = start_n;
		}
		n_info[start_n].done = 1;
		n_info[start_n].parent_node = -1;
	}

	int node_select()
	{
		int i, minpos; 
		float min;
		min = xxx;
		minpos = -1;
		for (i = 0; i < Nnodes; i++) {			
			if (((n_info[i].dist+n_info[i].h_dist) < min) && (n_info[i].done == 0))
			{
				min = n_info[i].dist + n_info[i].h_dist;
				minpos = i;
			}
		}
		return minpos;
	}

	vector<int> astar(int start_n, int end, vector<vector<float>>& dist_matr, vector<vector<float>>&coords)
	{
		int Nodes = dist_matr.size();
		set_size(Nodes);
		int i, node, k;
		int d;
		init(start_n, dist_matr, coords, end);

		cycles = 0;
		auto start = chrono::high_resolution_clock::now();

		for (i = 0; i < Nnodes - 2; i++) {
			node = node_select();		
			n_info[node].done = 1;
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
		cycles = i+1;
		path_all();

		auto finish = chrono::high_resolution_clock::now();
		elapsed_time = finish - start;

		return target_routes[end];
	}
};
