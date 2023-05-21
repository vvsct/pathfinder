#pragma once
#include <vector>
#include <string>

class Edge;

// === City ===

class City
{
    std::string name;
    std::vector<Edge> edges;
public:
    int distance; // from starting point
    City* sptParent; // previous city in shortest path tree

    City(std::string s);

    std::string getName() const;
    std::vector<Edge> const& getEdges() const;
    void addEdge(City* destination, int cost);
};

// === Edge ===

class Edge
{
    int cost;
public:
    City* destination;

    Edge(City* destination, int cost);

    int getCost() const;
};
