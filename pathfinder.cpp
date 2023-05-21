#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_set>
#include <limits>


#include "pathfinder.h"

// === City ===

City::City(std::string name) {
    this->name = name;
    this->distance = std::numeric_limits<int>::max(); // initialise with max int value to simulate infinity
    this->sptParent = nullptr;
}

std::string City::getName() const {
    return this->name;
}

std::vector<Edge> const& City::getEdges() const {
    return this->edges;
}

void City::addEdge(City* destination, int cost) {
    edges.push_back(Edge(destination, cost));
}

// === Edge ===

Edge::Edge(City* destination, int cost) {
    this->destination = destination;
    this->cost = cost;
}

int Edge::getCost() const {
    return this->cost;
}

// === file handling ===

std::vector<City> readCities(std::string filename) {
    std::vector<City> cities;
    std::ifstream file(filename);

    if(file.is_open())
    {
        std::string line;
        std::vector<std::vector<int>> distances;

        // create cities and save their distances to other cities
        while (std::getline(file, line))
        {
            // skip empty lines
            if(line.empty())
                continue;

            // create cities
            std::istringstream linestream(line);
            std::string name;

            linestream >> name;
            cities.push_back(City(name));

            // save distances
            std::vector<int> distancesFromCity;
            int distance;

            while(linestream >> distance)
                distancesFromCity.push_back(distance);

            distances.push_back(distancesFromCity);
        }

        // create edges based on the saved distances
        for(int i=0; i<cities.size(); ++i)
        {
            for(int j=0; j<distances[i].size(); ++j)
            {
                // 0 means no edge between the cities
                if(distances[i][j] != 0)
                    cities[i].addEdge(&cities[j], distances[i][j]);
            }
        }
    }

    file.close();
    return(cities);
}

// === Main ===

int main() {
    std::vector<City> cities = readCities("cities.txt");

    // display cities and ask for input
    for(int i=0; i<cities.size(); ++i)
    {
        std::cout << i+1 << ": " << cities[i].getName() << std::endl;
    }

    unsigned int startIndex, destIndex;
    std::cout << std::endl << "Select starting point: ";
    std::cin >> startIndex;
    std::cout << "Select destination: ";
    std::cin >> destIndex;

    City* startingPoint = &cities[startIndex-1];
    City* destination   = &cities[destIndex-1];

    // dijkstra setup
    startingPoint->distance = 0;
    std::unordered_set<City*> done;

    // dijkstra loop
    while(done.size() < cities.size())
    {
        // pick closest not-done city
        City* current;
        int min = std::numeric_limits<int>::max();
        for(int i=0; i<cities.size(); ++i)
        {
            if(cities[i].distance <= min && done.count(&cities[i]) == 0)
            {
                current = &cities[i];
                min = current->distance;
            }
        }

        // mark as done
        done.insert(current);

        // update distances and shortest path tree
        std::vector<Edge> const& edges = current->getEdges();
        for(int i=0; i<edges.size(); ++i)
        {
            int newDistance = current->distance + edges[i].getCost();
            City* neighbor = edges[i].destination;
            if(newDistance < neighbor->distance)
            {
                neighbor->distance = newDistance;
                neighbor->sptParent = current;
            }
        }
    }

    // get shortest path from tree
    City* current = destination;
    std::vector<City*> reversePath;

    while(current != nullptr)
    {
        reversePath.push_back(current);
        current = current->sptParent;
    }

    // print shortest path
    std::cout << std::endl << "Calculated shortest path from " << startingPoint->getName()
        << " to " << destination->getName() << "." << std::endl
        << "Total distance: " << destination->distance << " km" << std::endl << std::endl
        << startingPoint->getName();

    for(int i=reversePath.size()-2; i>=0; --i)
    {
        std::cout << " ---> " << reversePath[i]->getName();
    }

    std::cout << std::endl << std::endl;

    return 0;
}
