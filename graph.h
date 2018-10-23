#include <vector>
#include <algorithm>
#include "costs.h"

typedef std::unordered_map <int, std::vector <int> > AdjList;

class DirectedGraph: public Costs<int> {
private:
	AdjList inEdges;
	AdjList outEdges;

public:
	//Constructor
	DirectedGraph(int N = 0);

	//Destructor
	~DirectedGraph();

	//Copy operator
	DirectedGraph& operator = (DirectedGraph& other);

	//Function that return the number of vertices of the graph
    int GetVerticesNo();

	/*
		Function that checks if an edge exists
		pre:
			x, y - integers, the endpoints of an edge
		post:
			return true if the edge exists, otherwise false
	*/
    bool ExistEdge(const int& x, const int& y);

	/*
		Function that return the in degree of a vertex
		pre:
			x - integer, the vertex for which we want the in degree
		post:
			returns the in degree
	*/
    int GetInDegree(const int& x);

	/*
	Function that return the out degree of a vertex
	pre:
		x - integer, the vertex for which we want the out degree
	post:
		returns the out degree
	*/
    int GetOutDegree(const int& x);

	/*
	Function that return a list of the vertices that point to a vertex.
	pre:
		x - interger, the vertex for which we want the in edges
	post:
		returns a list of the vertices that point to x
	*/
    std::vector<int> GetInEdges(const int& x);

	/*
	Function that return a list of the vertices that are pointed by a vertex.
	pre:
		x - interger, the vertex for which we want the out edges
	post:
		returns a list of the vertices that are pointed by x
	*/
    std::vector<int> GetOutEdges(const int& x);

    /*
    Function that returns a list with all the vertices from the graph
    pre:
        *this = the graph
    post:
        return a vector with all the vertices
    */
    std::vector <int> GetVertices();

	/*
	Function that adds an edge in the graph
	pre:
		x - integer, the source vertex
		y - integer, the target vertex
		c - integer, the cost of the edge
	post:
		G' - the graph G after the edge (x, y, c) was added
		throw exeption if any occurs
	*/
	void AddEdge(const int& x, const int& y, const int& c = 0);

	/*
	Function that adds a vertex in the graph
	pre:
		x - integer, the vertex
	post:
		G' - the graph G after the vertex x was added
		throw exeption if any occurs
	*/
	void AddVertex(const int& x);

	/*
	Function that removes an edge from the graph
	pre:
		x - integer, the source vertex
		y - integer, the target vertex
	post:
		G' - the graph G after the edge (x, y) was removed
		throw exeption if any occurs
	*/
	void RemoveEdge(const int& x, const int& y);

	/*
	Function that removes a vertex from the graph
	pre:
		x - integer, the vertex
	post:
		G' - the graph G after the vertex x was removed
		throw exeption if any occurs
	*/
	void RemoveVertex(const int& x);
};



