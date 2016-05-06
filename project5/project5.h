/*
 * project5.h
 *
 * Header class for CPS222 project5
 *
 * Adapted from Professor Bjork's Graph Algorithms Handout
 * Modified by: Jonathan Manos
 * Date: 5/6/2016
 *
 */

 #include <string>
 #include <iostream>
 #include <fstream>
 #include <iomanip>
 #include <map>
 #include <list>
 #include <stack>
 #include <queue>
 using namespace std;

/*******************************************************************************
 * The following class represents a country graph by using an adjacency list
 ******************************************************************************/
class CountryGraph
{
    public:
        // Constructs a graph from the text input
        CountryGraph(istream & source);

        // BFS traversal from the index of the specified town
        // Outputs all town names along with their connected roads
        void bfs(int start, ostream & output) const;

        // Outputs the shortest path from the index of a specific town
        // to all other towns
        void findShortestPaths(int start, ostream & output);

        // Outputs the articulation point towns if it finds any
        void findArticulationPoints(ostream & output);

        // Recursive auxilary function for finding articulation points
        void findArticulationPointsAUX(int current, bool visited[], bool articPoint[],
                                        int num[], int low[], int parent[],
                                        int counter);

        // Outputs the towns and roads involved in the minimum cost spanning tree
        void findMinimumCostTree(ostream & output);

        // Finds the connected component of a road network if all the bridges collapse
        void findConnectedAreas(ostream & output);

        // Destructor
        ~CountryGraph()

        { delete [] _towns; }
    private:
        /* This inner class represents one road of the country */
        class Road
        {
            public:
                Road(int head, double distance, bool bridge)
                : _head(head), _distance(distance), _bridge(bridge)
                { }
                int _head;
                double _distance;
                bool _bridge;
        };
        //helpful structure to make sort look at the road distances
        struct CompareDistance {
            bool operator()(Road * a, Road * b) {return a->_distance < b->_distance;}
        };

        /* This inner class represents one town of the country graph */
        class Town
        {
            public:
                string _name;
                typedef list < Road > RoadList;
                RoadList _roads;
        };
        int _numberOfTowns;
        Town * _towns;
};
