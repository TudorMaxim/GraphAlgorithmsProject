#include <unordered_map>
#include <string>

template <typename T>
class Costs {
private:
	std::unordered_map <std::string, T> cost;

	inline std::string hash_fnc(const int& x, const int& y) {
        return std::to_string(x) + "->" + std::to_string(y);
    }

public:
	//constructor
	Costs();

	//destructor
	~Costs();
	/*
	Function that sets the cost of an edge
	pre:
		x - integer, the source vertex
		y - integer, the target vertex
		c - generic element, the cost
	post:
		-
	*/
	void SetCost(const int& x, const int& y, const T& c);

	/*
	Function that sets the cost of an edge
		pre :
			x - integer, the source vertex
			y - integer, the target vertex
			c - generic element, the new cost
		post :
			the cost of the edge (x, y) is updated
			throw exception if any occurs
	*/
	void UpdateCost(const int& x, const int& y, const T& c);

	/*
	Function that retrieves the cost of an edge
	pre :
		x - integer, the source vertex
		y - integer, the target vertex
	post :
		the cost of the edge (x, y)
		throw exception if any occurs
	*/
	T RetrieveCost(const int& x, const int& y);

	/*
	Function that erases an edge and its cost
	pre :
		x - integer, the source vertex
		y - integer, the target vertex
	post :
		-
	*/
	void EraseKey(const int&x, const int& y);
};

template<typename T>
Costs<T>::Costs() {}

template<typename T>
Costs<T>::~Costs() {
	cost.clear();
}

template<typename T>
void Costs<T>::SetCost(const int& x, const int& y, const T& c) {
	cost[hash_fnc(x, y)] = c; // set the cost
}

template<typename T>
void Costs<T>::UpdateCost(const int& x, const int& y, const T& c) {
    auto it = cost.find(hash_fnc(x, y));
	if (it == cost.end()) { // exception: Cannot update the cost of an edge that does not exist
		std::string ex = "Cannot update the cost of an edge that does not exist! \n";
		throw(ex);
	}
	cost[hash_fnc(x, y)] = c; // Update the cost
}

template<typename T>
T Costs<T>::RetrieveCost(const int& x, const int& y) {
	auto it = cost.find(hash_fnc(x, y));
	if (it == cost.end()) {// exception: Cannot retrieve the cost of an edge that does not exist
		std::string ex = "Cannot retrieve the cost of an edge that does not exist! \n";
		throw(ex);
	}
	return cost[hash_fnc(x, y)]; // retrieve the cost
}


template <typename T>
void Costs<T>::EraseKey(const int&x, const int& y) {
	auto i = cost.find(hash_fnc(x, y)); //search for the edge
	if (i != cost.end()) { // if the edge exists
		cost.erase(i);// erase the edge (x, y)
	}
}

