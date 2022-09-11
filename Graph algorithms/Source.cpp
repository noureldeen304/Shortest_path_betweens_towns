#include<iostream>
#include<list>
#include<vector>
#include<map>
using namespace std;
struct adjacent_node
{
	string des;
	int cost;
	string parent;
};
struct Edge
{
	string src;
	string des;
	int cost;
};
class Priorityqueue
{
private:
	vector<adjacent_node> heap;
	int right(int index_of_parent)
	{
		int p = (index_of_parent * 2) + 2;
		if (p >= heap.size())
			return -1;
		else
			return p;
	}
	int left(int index_of_parent)
	{
		int p = (index_of_parent * 2) + 1;
		if (p >= heap.size())
			return-1;
		else return p;
	}
	int parent(int index_of_child)
	{
		return (index_of_child == 0) ? -1 : (index_of_child - 1) / 2;
	}
	void min_cost_up(int pos)
	{
		if (pos == 0 || heap[parent(pos)].cost < heap[pos].cost)
			return;
		swap(heap[parent(pos)], heap[pos]);
		min_cost_up(parent(pos)); // parent(pos)--------> the new pos of our node after swapping
	}
	void max_cost_down(int pos)
	{
		int smallestchildpos = left(pos);
		if (smallestchildpos == -1) // no left child therefore no right child
			return;


		int rightchildpos = right(pos);
		if (rightchildpos != -1)
			if (heap.size() >= 3)
			{
				if (heap[rightchildpos].cost <= heap[smallestchildpos].cost)
					smallestchildpos = rightchildpos;
			}

		if (heap.size() >= 2)
			if (heap[pos].cost > heap[smallestchildpos].cost)
			{
				swap(heap[pos], heap[smallestchildpos]);
				max_cost_down(smallestchildpos);
			}
	}
public:
	void pop()
	{
		if (heap.size() > 0)
		{
			heap.front() = heap.back();
			heap.pop_back();
			max_cost_down(0);
		}
	}
	adjacent_node ExractMinCost()
	{
		if (heap.size() != 0)
			return heap.front();
	}
	void push(adjacent_node newadjacent)
	{
		heap.push_back(newadjacent);
		min_cost_up(heap.size() - 1);
	}
	bool isempty()
	{
		return heap.size() == 0;
	}
};
class Graph
{
private:
	int num_nodes;
	int num_edges;
	map<int, string> nodes;
	map<int, Edge> edges;
	map<string, list<adjacent_node> > vertices;
	Priorityqueue pq;
	map<string, int> dis;
	map<string, bool> visited;
public:
	Graph(string nodes[], int num_nodes, Edge edges[], int num_edges)
	{
		this->num_nodes = num_nodes;
		this->num_edges = num_edges;
		for (int i = 0; i < num_nodes; i++)
		{
			this->nodes[i] = nodes[i];
		}
		for (int i = 0; i < num_edges; i++)
		{
			vertices[edges[i].src].push_back({ edges[i].des, edges[i].cost, edges[i].src });

			vertices[edges[i].des].push_back({ edges[i].src, edges[i].cost, edges[i].des });

			this->edges[i] = edges[i];
		}
	}
	void Update_Graph()
	{
		string new_node, neighbor;
		int num, cost;
		Edge e;
		cout << "Enter the name of the you want to add: ";
		cin >> new_node;
		this->nodes[this->num_nodes] = new_node;
		this->num_nodes++;
		cout << "nodes[" << this->num_nodes << "]: " << this->nodes[this->num_nodes] << "\n";
		e.src = new_node;
		cout << "Enter the number of node it connects with: ";
		cin >> num;
		for (int i = 0; i < num; i++)
		{
			cout << "Neighbor number " << i + 1 << "\n";
			cout << "name: ";
			cin >> neighbor;
			e.des = neighbor;
			cout << "cost: ";
			cin >> cost;
			cout << "\n";
			e.cost = cost;
			this->vertices[new_node].push_back({ neighbor, cost, new_node });
			this->vertices[neighbor].push_back({ new_node, cost, neighbor });
			this->num_edges++;
			this->edges[this->num_edges] = e;
		}
	}
	void display()
	{
		for (int i = 0; i < num_nodes; i++)
		{
			cout << nodes[i] << ": ";
			for (auto j = vertices[nodes[i]].begin(); j != vertices[nodes[i]].end(); j++)
			{
				cout << "(" << j->des << "," << j->cost << "," << j->parent << ") ";
			}
			cout << "\n";
		}
		cout << "\n\n";
	}
	void Dijkstra(string source, string destination)
	{
		map<string, vector<string> > path;
		for (int i = 0; i < num_nodes; i++)
		{
			visited[nodes[i]] = false;
			dis[nodes[i]] = INT_MAX;
		}
		cout << "Dijkstra:\n";
		adjacent_node cur;

		dis[source] = 0;
		path[source].push_back(source);
		visited[source] = true;

		for (auto i : vertices[source])
		{
			pq.push(i);
		}

		while (!pq.isempty())
		{
			cur = pq.ExractMinCost();
			pq.pop();
			if (dis[cur.parent] + cur.cost < dis[cur.des])
			{
				dis[cur.des] = dis[cur.parent] + cur.cost;

				while (!path[cur.des].empty())
					path[cur.des].pop_back();

				for (auto i : path[cur.parent])
				{
					path[cur.des].push_back(i);
				}

				path[cur.des].push_back(cur.des);
			}
			//////////////////////////////////////////
			visited[cur.des] = true;
			for (auto i : vertices[cur.des])
			{
				if (!visited[i.des])
					pq.push(i);
			}
		}



		cout << "The shortest path from " << source << " to " << destination << ": " << dis[destination] << "\n";
		for (auto j = path[destination].begin(); j != path[destination].end(); j++)
		{
			cout << *j;
			if (j != path[destination].end() - 1)
				cout << " -> ";
		}
		cout << "\n\n";
	}
	void Bellmam_Ford(string source, string destination)
	{
		cout << "Bellmam Ford:\n";
		map<string, vector<string> > path;
		for (int i = 0; i < num_nodes; i++)
		{
			dis[nodes[i]] = INT_MAX;
		}
		dis[source] = 0;
		path[source].push_back(source);
		for (int max_edges = 0; max_edges < num_nodes - 1; max_edges++)
		{
			for (int i = 0; i < num_nodes; i++)
			{
				for (auto j = vertices[nodes[i]].begin(); j != vertices[nodes[i]].end(); j++)
				{
					if (dis[j->parent] != INT_MAX && dis[j->parent] + j->cost < dis[j->des])
					{
						dis[j->des] = dis[j->parent] + j->cost;
						while (!path[j->des].empty())
						{
							path[j->des].pop_back();
						}
						for (auto i : path[j->parent])
						{
							path[j->des].push_back(i);
						}
						path[j->des].push_back(j->des);
					}
				}
			}
		}
		cout << "The shortest path from " << source << " to " << destination << ": " << dis[destination] << "\n";
		for (auto j = path[destination].begin(); j != path[destination].end(); j++)
		{
			cout << *j;
			if (j != path[destination].end() - 1)
				cout << " -> ";
		}
		cout << "\n\n";
	}
};
int main()
{
	int num_edge;
	int num_node;
	string name;
	Edge* edges;
	string* nodes;

	cout << "Insert your graph data:\n***********************\n";
	cout << "Enter the number of towns: \n";
	cin >> num_node;
	nodes = new string[num_node];
	for (int i = 0; i < num_node; i++)
	{
		cout << "Enter the name of town number " << i + 1 << ": ";
		cin >> nodes[i];
	}
	cout << "\nEnter the number of edges: ";
	cin >> num_edge;
	edges = new Edge[num_edge];
	cout << "\n****************\n";
	for (int i = 0; i < num_edge; i++)
	{
		cout << "Enter the edge number " << i + 1 << ": \n";
		cout << "Source: ";
		cin >> edges[i].src;

		cout << "Destination: ";
		cin >> edges[i].des;

		cout << "Distance: ";
		cin >> edges[i].cost;
		cout << "\n****************\n";
	}
	Graph* graph = new Graph(nodes, num_node, edges, num_edge);

	string s, d;
	int ch, ch2;
	//bool flag = true; 
	while (true) {
		cout << "Choose an operation:\n";
		cout << "[1] Create new graph:\n";
		cout << "[2] Display the graph.\n";
		cout << "[3] Find the shortest path between two nodes.\n";
		cout << "[4] Add new towns.\n";
		cout << "[5] Exit.\n";

		cin >> ch;

		if (ch == 5)
			break;
		switch (ch)
		{
		case 1:
			cout << "Insert your graph data:\n***********************\n";
			cout << "Enter the number of towns: \n";
			cin >> num_node;
			nodes = new string[num_node];
			for (int i = 0; i < num_node; i++)
			{
				cout << "Enter the name of town number " << i + 1 << ": ";
				cin >> nodes[i];
			}
			cout << "\nEnter the number of edges: ";
			cin >> num_edge;
			edges = new Edge[num_edge];
			cout << "\n****************\n";
			for (int i = 0; i < num_edge; i++)
			{
				cout << "Enter the edge number " << i + 1 << ": \n";
				cout << "Source: ";
				cin >> edges[i].src;

				cout << "Destination: ";
				cin >> edges[i].des;

				cout << "Distance: ";
				cin >> edges[i].cost;
				cout << "\n****************\n";
			}
			graph = new Graph(nodes, num_node, edges, num_edge);
			break;
		case 2:
			graph->display();
			break;
		case 3:
			cout << "Enter the source name: "; cin >> s;
			cout << "Enter the destination name: "; cin >> d;
			cout << "Choose the algorithm you to use\n";
			cout << "[1] Dijkstra\n";
			cout << "[2] Bellmam_Ford\n";
			cin >> ch2;
			if (ch2 == 1)
				graph->Dijkstra(s, d);
			else
				graph->Bellmam_Ford(s, d);
			break;
		case 4:
			graph->Update_Graph();
		default:
			break;
		}
	}

	cout << "\n\n";
	system("pause");
	return 0;
}
