#include "graph.h"
using namespace std;

DirectedGraph::DirectedGraph(int N) {
    vector <int> Empty;
	for (int i = 0; i < N; i++) {
		inEdges[i] = Empty;
		outEdges[i] = Empty;
	}
}

DirectedGraph::~DirectedGraph() {
	inEdges.clear();
	outEdges.clear();
}

DirectedGraph& DirectedGraph::operator = (DirectedGraph& other) {
	this->inEdges = other.inEdges;
	this->outEdges = other.outEdges;
	for (const auto& v : other.GetVertices()) {
		for (auto e : other.GetOutEdges(v)) {
			SetCost(v, e, other.RetrieveCost(v, e));
		}
	}
	return *this;
}

int DirectedGraph::GetVerticesNo() {
	return outEdges.size();
}

bool DirectedGraph::ExistEdge(const int& x, const int& y) {
    return find(outEdges[x].begin(), outEdges[x].end(), y) != outEdges[x].end();
}

int DirectedGraph::GetInDegree(const int& x) {
    if (inEdges.find(x) == inEdges.end()) {
        string ex = "The vertex doe not exist in the graph! \n";
        throw(ex);
    }
	return inEdges[x].size();
}

int DirectedGraph::GetOutDegree(const int& x)  {
    if (outEdges.find(x) == outEdges.end()) {
        string ex = "The vertex does not exist in the graph! \n";
        throw(ex);
    }
	return outEdges[x].size();
}

vector<int> DirectedGraph::GetInEdges(const int& x) {
    if (inEdges.find(x) == inEdges.end()) {
        string ex = "The vertex doe not exist in the graph! \n";
        throw(ex);
    }
	return inEdges[x];
}
vector<int> DirectedGraph::GetOutEdges(const int& x) {
    if (outEdges.find(x) == outEdges.end()) {
        string ex = "The vertex does not exist in the graph! \n";
        throw(ex);
    }
	return outEdges[x];
}

vector <int> DirectedGraph::GetVertices() {
    vector <int> ret;
    for (const auto& it : outEdges) {
        ret.push_back(it.first);
    }
    return ret;
}


void DirectedGraph::AddEdge(const int& x, const int& y, const int& c) {
	/*if (ExistEdge(x, y)) {
		string ex = "The edge already exists!\n";
		throw(ex);
	}*/
	outEdges[x].push_back(y);
	inEdges[y].push_back(x);
	SetCost(x, y, c);
}

void DirectedGraph::AddVertex(const int& x) {
	if (outEdges.find(x) != outEdges.end()) {
		string ex = "Cannot add a vertex that already exists!\n";
		throw(ex);
	}
	vector <int> Empty;
	outEdges[x] = Empty;
	inEdges[x] = Empty;
}

void DirectedGraph::RemoveEdge(const int& x, const int &y) {
	if (ExistEdge(x, y) == false) {
		string ex = "Cannot remove an edge that does not exist!\n";
		throw(ex);
	}
	EraseKey(x, y);
	vector<int>::iterator i = find(outEdges[x].begin(), outEdges[x].end(), y);
	outEdges[x].erase(i);

	vector<int> ::iterator j = find(inEdges[y].begin(), inEdges[y].end(), x);
	inEdges[y].erase(j);
}

void DirectedGraph::RemoveVertex(const int& x) {
	if (outEdges.find(x) == outEdges.end()) {
		string ex = "Cannot remove a vertex that does not exist! \n";
		throw(ex);
	}
	for (auto y : outEdges[x]) {
		EraseKey(x, y);
	}
	outEdges[x].clear();
	outEdges.erase(outEdges.find(x));

	for (auto it : outEdges) {
		vector<int> ::iterator i = find(outEdges[it.first].begin(), outEdges[it.first].end(), x);
		if (i != outEdges[it.first].end()) {
			EraseKey(it.first, *i);
			outEdges[it.first].erase(i);
		}
	}

	inEdges[x].clear();
	inEdges.erase(inEdges.find(x));

	for (auto it : inEdges) {
		vector<int> ::iterator i = find(inEdges[it.first].begin(), inEdges[it.first].end(), x);
		if (i != inEdges[it.first].end()) {
			inEdges[it.first].erase(i);
		}
	}
}

