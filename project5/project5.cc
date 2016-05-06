/*
* project5.cc
*
* Main program for CPS222 Project 5 - Reads input and calls functions
* to create the graph, also has a lot of other functions
*
* Adapted from Professor Bjork's Graph Algorithms Handout
* Modified by: Jonathan Manos
* Date: 5/6/2016
*
*/

#include "project5.h"

/*******************************************************************************
* Read a country graph from a file into an adjacency list
* File format: first line: number of towns (N), number of roads (E)
*              next N lines: names of N towns
*              next E lines: roads - each specified by tail town name,
*                            head town name.
* This file format and code can be used for either an undirected graph or a
* digraph - see line marked with ** below.  It can also be used for a network
* if we add a distance to each road line - see lines marked with **** below
******************************************************************************/
CountryGraph::CountryGraph(istream & source)
{
    // Read number of towns and number of roads
    int numberOfRoads;
    source >> _numberOfTowns >> numberOfRoads;
    // Read in names of towns.  Add a Town object for each to the
    // _towns array, and also temporarily store its name in a map to
    // facilitate reading in the road descriptions by town name
    _towns = new Town[_numberOfTowns];
    map < string, int > nameMap;
    for (int i = 0; i < _numberOfTowns; i ++)
    {
        string name;
        source >> name;
        _towns[i]._name = name;
        nameMap[name] = i;
    }
    // Read in roads.  Add a road node for each to the adjacency
    // list for its towns
    for (int i = 0; i < numberOfRoads; i ++)
    {
        string tail, head, bridgeChar;
        bool bridge;
        double distance;

        source >> tail >> head >> bridgeChar >> distance;
        int tailIndex = nameMap[tail];
        int headIndex = nameMap[head];
        if(bridgeChar == "B")
        bridge = true;
        else
        bridge = false;

        _towns [ tailIndex ]._roads.push_back(Road(headIndex, distance, bridge));
        // ** THE FOLLOWING LINE WOULD BE OMITTED FOR A DIGRAPH **
        _towns [ headIndex ]._roads.push_back(Road(tailIndex, distance, bridge));
    }
}

void CountryGraph::bfs(int start, ostream & output) const
{
    output << "\nThe input data in breadth-first order:\n" << endl;

    // Keep track of whether a Town has been scheduled to be visited, lest
    // we get into a loop
    bool scheduled [ _numberOfTowns ];
    for (int i = 0; i < _numberOfTowns; i ++)
    scheduled[i] = false;
    queue < int > toVisit;
    toVisit.push(start);
    scheduled[start] = true;
    while (! toVisit.empty())
    {
        // Visit front Town on the queue
        int currentTown  = toVisit.front(); toVisit.pop();
        output << _towns [ currentTown ]._name << endl;
        // Enqueue its unscheduled neighbors
        for (Town::RoadList::iterator neighbor =
            _towns[currentTown]._roads.begin();
            neighbor != _towns[currentTown]._roads.end();
            neighbor ++)
            {
                int head = neighbor -> _head;

                output << "\t" << _towns[head]._name << " ";
                output << neighbor -> _distance << " miles ";
                if(neighbor -> _bridge)
                output << "via bridge";
                output << endl;

                if (! scheduled[head])
                {
                    toVisit.push(head);
                    scheduled[head] = true;
                }
            }
        }
    }

    // Outputs the shortest path to some towns
    void CountryGraph::findShortestPaths(int start, ostream & output)
    {
        cout << "\nThe shortest routes from " << _towns[start]._name << " are:\n" << endl;

        double shortestPaths[_numberOfTowns];

        for (int i = 0; i < _numberOfTowns; i ++)
        shortestPaths[i] = 123;

        // If a town needs to be visited add them to schedule
        bool scheduled [ _numberOfTowns ];
        for (int i = 0; i < _numberOfTowns; i ++)
        scheduled[i] = false;
        queue < int > toVisit;
        toVisit.push(start);
        scheduled[start] = true;
        while (!toVisit.empty())
        {
            // Visit front Town on the queue
            int currentTown  = toVisit.front(); toVisit.pop();
            // Enqueue its unscheduled neighbors
            for (Town::RoadList::iterator neighbor =
                _towns[currentTown]._roads.begin();
                neighbor != _towns[currentTown]._roads.end();
                neighbor ++)
                {
                    int head = neighbor -> _head;
                    shortestPaths[head] = neighbor -> _distance;

                }

                for (int i = 0; i < _numberOfTowns; i++)
                {
                    if(i != start)
                    {
                        output << "\t" << _towns[i]._name << " ";
                        output << shortestPaths[i] << " miles ";
                        output << endl;
                    }
                }

                if(_numberOfTowns == 1)
                    output << "\t" << _towns[0]._name << " is the only town in the whole country!"<<endl;
            }
        }

        // Outputs the towns and roads involved in the minimum cost tree
        void CountryGraph::findMinimumCostTree(ostream & output)
        {

            cout << "\nThe road upgrading goal can be achieved at minimal cost by upgrading:\n" << endl;

            //The list of roads
            list<Road *> roadList;

            //Puts all the roads in the roadList
            for (int i = 0; i < _numberOfTowns;  i++)
            {
                for(Town::RoadList::iterator iter = _towns[i]._roads.begin();
                iter != _towns[i]._roads.end();
                iter ++)
                {
                    if(iter -> _head > i)
                    roadList.push_back(&* iter);
                }
            }

            //sorts the roads by the distances
            roadList.sort(CompareDistance());

            //component array to show which towns are connected
            //0 for not connected, 1 for connected
            int component[_numberOfTowns];
            for (int i = 0; i < _numberOfTowns; i++)
            component[i] = 0;

            //empty road, a starting town and ending town, as well as size of output
            //set to 0
            Road * road;
            int startTown;
            int visitTown;
            int size = 0;

            //While we haven't found enough roads
            while(size < _numberOfTowns - 1)
            {
                if(roadList.empty())
                    break;
                road = roadList.front();
                roadList.pop_front();
                startTown = road -> _head;

                //for each town, searches its roads for the town thats
                //on the end of the road we already have
                for (int i = 0; i < _numberOfTowns; i++)
                {
                    for(Town::RoadList::iterator iter = _towns[i]._roads.begin();
                    iter != _towns[i]._roads.end();
                    iter ++)
                    {
                        //if the distance is the same, and its not the same side of the road
                        //then we set the visitTown to this town
                        if(iter -> _distance == road -> _distance && _towns[startTown]._name != _towns[i]._name)
                        {
                            visitTown = i;
                            i = _numberOfTowns;
                            //cout << "startTown: " << _towns[startTown]._name << " and 2ndTown: "<< _towns[visitTown]._name<<endl;
                            //breaks the iterator for loop
                            break;
                        }
                    }
                }

                //As long as the road isn't connecting two towns that have already
                //been connected, we add it to the minimum spanning tree
                if(!(component[startTown] == 1 && component[visitTown] == 1))
                {
                    size++;
                    component[startTown] = 1;
                    component[visitTown] = 1;
                    output << "\t" << _towns[startTown]._name << " to " << _towns[visitTown]._name << endl;
                }
            }
            if(_numberOfTowns == 1)
                output << "\t" << _towns[0]._name << " is the only town in the whole country!"<<endl;
        }

        //looks for any articulation points in the country graph
        //outputting the name of the town if it is an articulation point
        //outputting none if there are no articulation points
        void CountryGraph::findArticulationPoints(ostream & output)
        {

            cout << "\nObliterating any of the following would result in the province becoming disconnected:\n" << endl;

            //bools for the towns visited and articulationPoints found
            bool visited[_numberOfTowns];
            bool articPoint[_numberOfTowns];

            //Set all bools to false
            for(int i =0; i < _numberOfTowns; i++)
            {
                visited[i] = false;
                articPoint[i] = false;
            }

            //ints for the low values, num values, and the parent indexes
            int num[_numberOfTowns];
            int low[_numberOfTowns];
            int parent[_numberOfTowns];
            int counter = 0;

            //Set all parents to -1
            for(int i =0; i < _numberOfTowns; i++)
            {
                parent[i] = -1;
            }

            //calls the auxilary function for finding articulation points on each
            //unvisited town
            for(int i = 0; i < _numberOfTowns; i++)
            if(! visited[i])
            findArticulationPointsAUX(i, visited, articPoint, num, low, parent, counter);

            bool foundArticPoints = false;

            // outputs all the articulation points and sets foundArticPoints to true
            for(int i = 0; i < _numberOfTowns; i++)
            if(articPoint[i])
            {
                output << "\t" << _towns[i]._name << endl;
                foundArticPoints = true;
            }

            //if there were no Articulation points, output none
            if(!foundArticPoints)
            output << "\tNone" << endl;
        }

        // the auxilary function for finding articulation points on the unvisited towns
        void CountryGraph::findArticulationPointsAUX(int currentTown, bool visited[], bool articPoint[], int num[], int low[], int parent[], int counter)
        {
            //set the current town to visited and increases the counter,
            //setting both the num and low value to the new counter
            visited[currentTown] = true;
            counter ++;
            num[currentTown] = counter;
            low[currentTown] = counter;

            int visitedSoFar = 0;
            int head;

            // iterator to go through every road
            for(Town::RoadList::iterator neighbor = _towns[currentTown]._roads.begin();
            neighbor != _towns[currentTown]._roads.end();
            neighbor ++)
            {
                head = neighbor -> _head;

                //if the town hasnt been visited yet, run auxilary function again
                if(!visited[head])
                {
                    //increases how many towns have been visited and sets parent
                    //to the currentTown before recursing again
                    visitedSoFar ++;
                    parent[head] = currentTown;
                    findArticulationPointsAUX(head, visited, articPoint, num, low, parent, counter);

                    //sets new low value to min of currentTown and head's low
                    low[currentTown] = min(low[currentTown], low[head]);

                    //if its the root and places have already been visitedSoFar
                    //besides this town, it is an articulation point
                    if(parent[currentTown] == -1 && visitedSoFar > 1)
                    articPoint[currentTown] = true;

                    //otherwise if its not the root and
                    //the low of the head is greater than or equal to the num of the
                    //currentTown then it is in articulation point
                    else if(parent[currentTown] != -1 && low[head] >= num[currentTown])
                    articPoint[currentTown] = true;
                }
                //otherwise if the town has been visited and it is not the parent
                //then set low to the min of the currentTown's low and the head's num
                else if(head != parent[currentTown])
                low[currentTown] = min(low[currentTown], num[head]);
            }
        }

        // Finds the connected components of the country if all the bridges are destroyed
        // in a storm
        void CountryGraph::findConnectedAreas(ostream & output)
        {

            cout << "\nConnected components in event of a major storm are:" << endl;

            //a bool for each town on whether it was visted or not, initialized to false
            bool * visited = new bool[_numberOfTowns];
            for (int i = 0; i < _numberOfTowns; i++)
            {
                visited[i] = false;
            }

            //currentTown is the index of the town currently being checked
            //stillNeedsVisiting just says whether the loop still needs to visit more towns
            //toVisit queue has the towns that will be visited for the current connected
            //component
            int currentTown;
            bool stillNeedsVisiting = true;
            queue <int> toVisit;

            while(stillNeedsVisiting)
            {
                currentTown = 0;

                //find the index of the next unvisited Town
                while(visited[currentTown])
                {
                    //if we reached the last town, we have nothing left to visit, breaks
                    if(currentTown == _numberOfTowns - 1)
                    {
                        stillNeedsVisiting = false;
                        break;
                    }
                    else
                    currentTown++;
                }

                //If the last town is reached, the search is done
                if(!stillNeedsVisiting)
                break;

                output << "\n    If all bridges fail, the following towns would form an isolated group:\n" << endl;
                output << "\t" << _towns[currentTown]._name << endl;

                //sets the bool for the current town to true, and then pushes it to queue
                visited[currentTown] = true;
                toVisit.push(currentTown);

                //While there is a town in the queue to visit, it checks every road of the
                //finding any town that is connected via a road, doesn't check bridges
                while(!toVisit.empty())
                {
                    //takes current town out of the queue
                    currentTown = toVisit.front();
                    toVisit.pop();

                    //check each of the roads of the currentTown
                    for(Town::RoadList::iterator road = _towns[currentTown]._roads.begin();
                    road != _towns[currentTown]._roads.end();
                    road ++)
                    {
                        int head = road -> _head;

                        //If the road is not a bridge and hasn't been visited
                        //outputs the name of the town at its head
                        if(road -> _bridge == false && !visited[head])
                        {
                            output << "\t" << _towns[head]._name << endl;

                            //If it hasn't been visited, push it into the queue so
                            //it can be searched and set its visited bool to true
                            if(!visited[head])
                            {
                                toVisit.push(head);
                                visited[head] = true;
                            }
                        }
                    }
                }
            }
        }

        int main(int argc, char * argv[])
        {

            //std::cout<<"Please enter country graph data now!"<<std::endl;

            while(true)
            {
                char c;
                std::cin >> c;
                if (cin.eof())
                break;
                else
                cin.unget();
                CountryGraph countryGraph(cin);
                countryGraph.bfs(0, cout);
                countryGraph.findShortestPaths(0, cout);
                countryGraph.findMinimumCostTree(cout);
                countryGraph.findConnectedAreas(cout);
                countryGraph.findArticulationPoints(cout);
                cout<<endl;
            }



        }
