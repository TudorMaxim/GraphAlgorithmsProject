#include <fstream>
#include <iostream>
#include <cstring>
#include <queue>
#include <stack>
#include <algorithm>
#include <map>
#include "graph.h"

using namespace std;

const int OO = 1 << 25;
unordered_map <int, int> Dist, Pre, Low, Level;
unordered_map <int, bool> Vis, InStack;
stack <int> Stk;
vector < vector<int> > StronglyConnected;
int cnt;


void Menu() {
	cout << "Your options are: \n";
	cout << "0 - Exit the application. \n";
	cout << "1 - Get the number of vertices. \n";
	cout << "2 - Check if an edge exists. \n";
	cout << "3 - Get the in degree of a vertex. \n";
	cout << "4 - Get the out degree of a vertex. \n";
	cout << "5 - Iterate through the set of inbound edges of a vertex. \n";
	cout << "6 - Iterate through the set of outbound edges of a vertex. \n";
	cout << "7 - Update the cost of an edge. \n";
	cout << "8 - Retrieve the cost of an edge. \n";
	cout << "9 - Add an edge. \n";
	cout << "10 - Add a vertex. \n";
	cout << "11 - Remove an edge. \n";
	cout << "12 - Remove a vertex. \n";
	cout << "13 - Print the graph. \n";
	cout << "14 - BFS \n";
	cout << "15 - Strongly Connected Components \n";
	cout << "16 - Floyd-Warshall algorithm for minimum length path \n";
	cout << "17 - Kruskal algorithm for minimum spanning tree \n";
	cout << "18 - Topological Sorting \n";
	cout << "19 - Number of paths between 2 vertices in a DAG \n";
	cout << "20 - Find a Hamiltonian cycle of low cost\n";
}

int ToInt(char* s) {
	int val = 0;
	for (unsigned int i = 0; i < strlen(s); i++) {
		val = val * 10 + (s[i] - '0');
	}
	return val;
}

void SaveGraph(DirectedGraph& G) {
	ofstream fout("lastgraph.txt");
	for (const auto& v : G.GetVertices()) {
		for (const auto& e : G.GetOutEdges(v)) {
			fout << v << " " << e <<  " " << G.RetrieveCost(v, e) << "\n";
		}
	}
	for (const auto& v : G.GetVertices()) {
		if (G.GetOutEdges(v).size() == 0) {
			fout << v << "  \n";
		}
	}
}

void CreatePath(vector<int>& path, unordered_map <int,int> Prev, int source, int target) {
    if (target == source) {
        path.push_back(source);
        return;
    }
    CreatePath(path, Prev, source, Prev[target]);
    path.push_back(target);
}

void Bfs(DirectedGraph& G, int source, int target, int &distance, vector <int>& path) {
    queue <int> Q;
    Dist[source] = 0;
    Vis[source] = true;
    Q.push(source);
    while (!Q.empty()) {
        int parent = Q.front();
        if (parent == target) break;
        Q.pop();
        for (auto child : G.GetOutEdges(parent)) {
            if (Vis[child] == false) {
                Vis[child] = true;
                Dist[child] = Dist[parent] + 1;
                Pre[child] = parent;
                Q.push(child);
            }
        }
    }
    if (Vis[target] == false) {
        distance = -OO;
    }
    else {
        distance = Dist[target];
        CreatePath(path, Pre, source, target);
    }
}

void Dfs_Scc(DirectedGraph& G, int v) { // Tarjan
    Low[v] = Level[v] = ++cnt;
    Stk.push(v);
    Vis[v] = true;
    InStack[v] = true;
    for (const auto& e : G.GetOutEdges(v)) {
        if (Vis[e] == false) {
            Dfs_Scc(G, e);
            Low[v] = min(Low[v], Low[e]);
        }
        else if (InStack[e] == true) {
            Low[v] = min(Low[v], Low[e]);
        }
    }
    if (Low[v] == Level[v]) { /// is a strongly connected component, so pop the stack
            vector <int> component;
            int vert;
            do {
                vert = Stk.top();
                Stk.pop();
                InStack[vert] = false;
                component.push_back(vert);
            } while (!Stk.empty() && vert != v);
            StronglyConnected.push_back(component);
    }
}

ofstream fout ("data.out");

void FloydWarshall(const vector <vector <int> >& Graph, int source, int target) {
    vector <vector <int> > Dist = Graph;
    vector <vector <int> > Next(Dist.size(), vector <int>(Dist.size(), -1));
    vector <int> path;

    for (unsigned int i = 0; i < Next.size(); i++) {
        for (unsigned int j = 0; j < Next.size(); j++) {
            Next[i][j] = j;
            if (Dist[i][j] == 0) Dist[i][j] = OO;
        }
    }

    for (unsigned int k = 0; k < Dist.size(); k++) {
        for (unsigned int i = 0; i < Dist.size(); i++) {
            for (unsigned int j = 0; j < Dist.size(); j++) {
                if (i != j && Dist[i][k] + Dist[k][j] < Dist[i][j]) {
                    Dist[i][j] = Dist[i][k] + Dist[k][j];
                    Next[i][j] = Next[i][k];
                }
            }
        }
    }
    if (Dist[source][target] != OO) {
        cout << "The distantce is: " << Dist[source][target] << "\n";

        //Create the path
        int vert = source;
        path.push_back(vert);
        while (vert != target) {
            vert = Next[vert][target];
            path.push_back(vert);
        }
        cout << "The path is: ";
        for (const int v : path) {
            cout << v << " ";
        }
        cout << "\n";
    }
    else {
        cout << "There is no path between " << source << " and " << target << "\n";
    }
}


class Edge {
public:
    int source, target, cost;

    Edge(int s, int t, int c) : source(s), target(t), cost(c) {}
    ~Edge() {}
};

unordered_map <int,int> Parent, Rank;

bool cmp (const Edge& x, const Edge& y) {
    return x.cost < y.cost;
}

int Find (int vertex) {
    while (vertex != Parent[vertex]) {
        vertex = Parent[vertex];
    }
    return vertex;
}

void Union(int x, int y) {
    if (Rank[x] > Rank[y])
        Parent[y] = x;
    else
        Parent[x] = y;

    if (Rank[x] == Rank[y])
        Rank[y]++;
}

void Kruskal(DirectedGraph& G) {
    vector <Edge> edgeList;
    vector <Edge> apm;
    int minCost = 0;
    for (int v : G.GetVertices()) {
        Rank[v] = 0;
        Parent[v] = v;
        for (int e : G.GetOutEdges(v)) {
            edgeList.push_back(Edge(v, e, G.RetrieveCost(v, e)));
        }
    }
    sort(edgeList.begin(), edgeList.end(), cmp);
    for (int i = 0; i < edgeList.size(); i += 2) {
        int rootX = Find(edgeList[i].source);
        int rootY = Find(edgeList[i].target);
        /*cout << edgeList[i].source << " with parent " << rootX << "\n";
        cout << edgeList[i].target << " with parent " << rootY << "\n";
        cout << "------------------------------------------------------\n";*/
        if (rootX != rootY) {
            Union(rootX, rootY);
            minCost += edgeList[i].cost;
            apm.push_back(edgeList[i]);
        }
    }
    cout << "The cost of the minimum spanning tree is: " << minCost << "\n";
    cout << "The minimum spanning tree is:\n";
    for (const auto& it : apm) {
        cout << it.source << " " << it.target << " " << it.cost << "\n";
    }
}

vector <int> topoSort(DirectedGraph& G) {
    queue <int> Q;
    vector <int> sorted;
    unordered_map <int,int> cnt;
    for (int v : G.GetVertices()) {
        cnt[v] = G.GetInDegree(v);
        if (cnt[v] == 0) Q.push(v);
    }

    while (!Q.empty()) {
        int v = Q.front();
        Q.pop();
        sorted.push_back(v);
        for (int e : G.GetOutEdges(v)) {
            cnt[e] = cnt[e] - 1;
            if (cnt[e] == 0) Q.push(e);
        }
    }
    if (sorted.size() < G.GetVerticesNo()) {
        sorted.clear();
    }
    return sorted;
}

unordered_map <int, bool> vis;
int cnt_paths = 0;

void countPaths(DirectedGraph& G, int source, int target) {
    vis[source] = true;
    if (source == target)
        cnt_paths++;
    for (int e : G.GetOutEdges(source)) {
        if (vis[e] == false)
            countPaths(G, e, target);
    }
    vis[source] = false;
}


bool isHamiltonian(DirectedGraph& G) { //Dirac's theorem
    for (int v = 0; v < G.GetVerticesNo(); v++)
        if (G.GetOutDegree(v) < G.GetVerticesNo() / 2) return false;
    return true;
}

bool isSafe(int N, int vert, vector <int> path) {
    if (path.size() == N && path[0] == vert) return true;
    return find(path.begin(), path.end(), vert) == path.end();
}

int costHamilton = 0;
bool found = false;
map < pair <int,int>, bool> usedEdges;

void dfsHamilton(DirectedGraph& G, int vert, vector <int>& path) {
    if (found == true) return;

    Vis[vert] = true;
    path.push_back(vert);
    if (path.size() == G.GetVerticesNo()) {
        for (int child : G.GetOutEdges(vert)) {
            if (child == path[0]) {
                costHamilton += G.RetrieveCost(vert, child);
                usedEdges[make_pair(vert, child)] = true;
                path.push_back(child);
                found = true;
                return ;
            }
        }
        Vis[vert] = false;
        path.pop_back();
        Vis[path.back()] = false;
        return;
    }
    for (int i = 0; i < G.GetOutDegree(vert); i++) {
        int minn = 1 << 30;
        int index = 0;
        int chosen_index = -1;
        for (int child : G.GetOutEdges(vert)) {
            if (Vis[child] == false && isSafe(G.GetVerticesNo(), child, path) && minn > G.RetrieveCost(vert, child) && !usedEdges[make_pair(vert, child)]) {
                minn = G.RetrieveCost(vert, child);
                chosen_index = index;
            }
            index++;
        }
        if (chosen_index != -1) {
            int chosen_child = G.GetOutEdges(vert)[chosen_index];
            costHamilton += minn;
            usedEdges[make_pair(vert, chosen_child)] = true;

            dfsHamilton(G, chosen_child, path);

            if (!found) {
                usedEdges[make_pair(vert, chosen_child)] = false;
                costHamilton -= minn;
                path.pop_back();
            }
        }
    }
    Vis[vert] = false;
}

void hamiltonianCycleOfLowCost(DirectedGraph& G) {

    if (!isHamiltonian(G)) {
        cout << "The graph is not hamiltonian!\n";
        return;
    }
    costHamilton = 0;
    found = false;
    vector <int> path;
    dfsHamilton(G, 0, path);
    cout << "The cost of the cycle is: " << costHamilton << "\n";
    cout << "The hamiltonian cycle is: \n";
    for (auto it : path) {
        cout << it << " ";
    }
    cout << "\n";
}

int main() {
	ios_base::sync_with_stdio(false);
	cout << "Type 1 for using the initial graph or 2 to use the last modified graph \n";
	int opt;
	int N, M, x, y, c, option;
	DirectedGraph G;
	cin >> opt;
	if (opt == 1) {
		ifstream fin("sample.txt");
		fin >> N >> M;
		DirectedGraph G1(N);
		for (int i = 0; i < M; i++) {
			fin >> x >> y >> c;
			G1.AddEdge(x, y, c);
			G1.AddEdge(y, x, c);
		}
		G = G1;
	}
	else if (opt == 2) {
		char line[200];
		ifstream fin("lastgraph.txt");
		DirectedGraph G2;
		while (fin.getline(line, 200)) {
			int argc = 0;
			char* argv[3];
			for (char* p = strtok(line, " "); p != NULL; p = strtok(NULL, " ")) {
				argv[argc] = new char[50];
				strcpy(argv[argc++], p);
			}
			if (argc == 3) {
				G.AddEdge(ToInt(argv[0]), ToInt(argv[1]), ToInt(argv[2]));
			}
			else if (argc == 1) {
				G.AddVertex(ToInt(argv[0]));
			}
		}
	}

	Menu();
	while (1) {
		cout << "Type an option: ";
		cin >> option;
		try {
			if (option == 0) {
				break;
			}
			else if (option == 1) {
				cout << "There are " << G.GetVerticesNo() << " in the graph. \n";
			}
			else if (option == 2) {
				cout << "Give the source vertex: ";
				cin >> x;
				cout << "Give the target vertex: ";
				cin >> y;
				if (G.ExistEdge(x, y)) {
					cout << "The edge exists! \n";
				}
				else cout << "The Edge does not exist! \n";
			}
			else if (option == 3) {
				cout << "Give the vertex: ";
				cin >> x;
				cout << "The in degree of this vertex is: " << G.GetInDegree(x) << "\n";
			}
			else if (option == 4) {
				cout << "Give the vertex: ";
				cin >> x;
				cout << "The out degree of this vertex is: " << G.GetOutDegree(x) << "\n";
			}
			else if (option == 5) {
				cout << "Give the vertex: ";
				cin >> x;
				vector<int>  inbounds = G.GetInEdges((x));
				if (inbounds.size() == 0) {
                    cout << "There are no inbound edges for the given vertex. \n";
				}
				else {
                    cout << "The inbound edges of the given vertex are: \n";
                    for (auto it : inbounds) {
                        cout << it << " " << x << " " << G.RetrieveCost(it, x) << "\n";
                    }
				}
			}
			else if (option == 6) {
				cout << "Give the vertex: ";
				cin >> x;
				vector <int> outbounds = G.GetOutEdges(x);
				if (outbounds.size() == 0) {
                    cout << "There are no outbound edges for the given vertex. \n";
				}
				else {
                    cout << "The outbound edges of the given vertex are: \n";
                    for (auto it : G.GetOutEdges(x)) {
                        cout << x << " " << it << " " << G.RetrieveCost(x, it) << "\n";
                    }
				}
			}
			else if (option == 7) {
				cout << "Give the source vertex: ";
				cin >> x;
				cout << "Give the target vertex: ";
				cin >> y;
				cout << "Give the new cost: ";
				cin >> c;
				G.UpdateCost(x, y, c);
				cout << "Successful Update! The new cost of the given edge is: " << c << "\n";
			}
			else if (option == 8) {
				cout << "Give the source vertex: ";
				cin >> x;
				cout << "Give the target vertex: ";
				cin >> y;
				int cost = G.RetrieveCost(x, y);
				cout << "The cost of the given edge is: " << cost << '\n';
			}
			else if (option == 9) {
				cout << "Give the source vertex: ";
				cin >> x;
				cout << "Give the target vertex: ";
				cin >> y;
				cout << "Give the cost: ";
				cin >> c;
				G.AddEdge(x, y, c);
				cout << "Edge added successfully! \n";
			}
			else if (option == 10) {
				cout << "Give the vertex you want to add: ";
				cin >> x;
				G.AddVertex(x);
				cout << "Vertex added successfully! \n";
			}
			else if (option == 11) {
				cout << "Give the source vertex: ";
				cin >> x;
				cout << "Give the target vertex: ";
				cin >> y;
				G.RemoveEdge(x, y);
				cout << "Edge removed successfully! \n";
			}
			else if (option == 12) {
				cout << "Give the vertex you want to remove: ";
				cin >> x;
				G.RemoveVertex(x);
				cout << "Vertex removed successfully! \n";
			}
			else if (option == 13) {
				cout << "The edges from the graph are: \n";
				for (auto v : G.GetVertices()) {
					for (auto e : G.GetOutEdges(v)) {
						cout << v << " " << e << " " << G.RetrieveCost(v, e) << "\n";
					}		}
			}
			else if (option == 14) {
                vector <int> path;
                int distance = -OO;
                cout << "Give the source vertex: ";
                cin >> x;
                cout << "Give the target vertex: ";
                cin >> y;
                Bfs(G, x, y, distance, path);
                if (distance == -OO) {
                    cout << "There is no path between " << x << " and " << y << "!\n";
                    Vis.clear();
                    Dist.clear();
                    Pre.clear();
                    continue;
                }
                cout << "The lowest length path is: ";
                for (const auto& it : path) {
                    cout << it << " ";
                }
                cout << "\nThe distance is: " << distance << '\n';
                Vis.clear();
                Dist.clear();
                Pre.clear();
			}
			else if (option == 15) {
                cnt = 0;
                for (const int& v : G.GetVertices()) {
                    if (Vis[v] == false) {
                        Dfs_Scc(G, v);
                    }
                }
                cout << "The strongly connected components are: \n";
                int i = 1;
                for (vector<int>& comp : StronglyConnected) {
                    sort(comp.begin(), comp.end());
                    cout << "Component " << i << ":\n";
                    for (const int& vertex : comp) {
                        cout << vertex << " ";
                    }
                    i++;
                    cout << "\n";
                    bool message = false;
                    for (const int& v : comp) {
                        for (const int& e : comp) {
                            if (G.ExistEdge(v, e)) {
                                if (message == false) {
                                    message = true;
                                    cout << "The edges of the current strongly connected component are: \n";
                                }
                                cout << v << " -> " <<  e << " with cost " << G.RetrieveCost(v, e) << " \n";
                            }
                        }
                    }
                }
                Vis.clear();
                Low.clear();
                Level.clear();
                InStack.clear();
			}
			else if (option == 16) {
                vector < vector<int> > Graph(N, vector <int> (N, 0));
                cout << "Give the source vertex: ";
                cin >> x;
                cout << "Give the target vertex: ";
                cin >> y;
                for (auto v : G.GetVertices()) {
					for (auto e : G.GetOutEdges(v)) {
						Graph[v][e] = G.RetrieveCost(v, e);
					}
				}
				FloydWarshall(Graph, x, y);
			}
			else if (option == 17) {
                Kruskal(G);
			}
			else if (option == 18) {
                vector <int> sorted = topoSort(G);
                if (sorted.size() == 0) {
                    cout << "The graph is not a DAG!\n";
                }
                else {
                    cout << "A topological sorting is: ";
                    for (int v : sorted) {
                        cout << v << " ";
                    }
                    cout << "\n";
                }
			}
			else if (option == 19) {
                cout << "Give the source: ";
                int source, target;
                cin >> source;
                cout << "Give the target: ";
                cin >> target;
                vector <int> sorted = topoSort(G);
                if (sorted.size() == 0) {
                    cout << "The graph is not a DAG!\n";
                }
                else {
                    for (int v : G.GetVertices())
                        vis[v] = false;
                    countPaths(G, source, target);
                    cout << "The number of paths between " << source << " and " << target << " is: " << cnt_paths << "\n";
                }
			}
			else if (option == 20) {
                hamiltonianCycleOfLowCost(G);
			}
			else {
				cout << "Invalid command! Read again the instructions! \n";
				Menu();
			}
		}
		catch (string ex) {
			cout << ex;
		}
	}
	cout << "Bye Bye! \n";
	SaveGraph(G);
	return 0;
}
