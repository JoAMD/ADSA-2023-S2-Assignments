// TODO: print actual tree structure to debug rotations

#include <iostream>
#include <cstring>
#include <vector>
#include <stack>
#include <set>
#include <math.h>
#include <algorithm>
#include <unistd.h>

using namespace std;

bool debug = false;
// const int TABLE_SIZE = 26;


// ostream& operator<<(ostream &out, SearchResult const& searchResult) {
//     out << searchResult.actualLoc << " " << searchResult.potentialLoc;
//     return out;
// }

// class Node {
//     int data;
//     int buildCost;
//     int destroyCost;
//     // vector<Node*> neighbours;
//     Node* next;
//     Node* prev;

//     Node(int data, int buildCost, int destroyCost) {
//         this->data = data;
//         this->buildCost = buildCost;
//         this->destroyCost = destroyCost;
//         // this->neighbours = vector<Node*>();
//     }
// };

// class Graph {
//     vector<Node*> nodes; //in order

//     Graph() {
//         nodes = vector<Node*>();
//     }

//     void addNode(Node* node) {
//         nodes.push_back(node);
//     }
// };


vector<vector<int>>* inputHandling(string input, bool isAlphaInput) {

    int pos = 0;
    int prevPos = 0;
    string oneInput;

    vector<vector<int>> *matrix = new vector<vector<int>>();

    while (true) {
        pos = input.find_first_of(',', pos);
        if (pos <= -1) {
            pos = input.length();
        }
        // cout << "prev pos: " << prevPos << endl; 
        // cout << "pos: " << pos << endl; 
        oneInput = input.substr(prevPos, pos - prevPos);
        // cout << "oneInput: " << oneInput << endl; 
        
        vector<int> arr = vector<int>();

        int num;
        for(char& c : oneInput) {
            if (isAlphaInput) {
                if (islower(c)) {
                    num = c - 'a' + 26;
                }
                else {
                    num = c - 'A';
                }
            }
            else {
                num = c - '0';
            }
            arr.push_back(num);
        }

        // if (input[input.length() - 1] == '\r\n') {
        //     break;
        // }
        matrix->push_back(arr);
        if (pos >= input.length()) {
            break;
        } 
        pos++;
        prevPos = pos;
    }

    return matrix;
}

void printVectorOfVector(vector<vector<int>> vec) {
    for (vector<int> &v : vec) {
        for (int &i : v) {
            cout << i << " ";
        }
        cout << endl;
    }
}

class Edge {
    public: 

    int fromNode;
    int toNode;

    int buildCost = -1;
    int destoryCost = -1;

    Edge(int fromNode, int toNode, int buildCost, int destoryCost) {
        this->fromNode = fromNode;
        this->toNode = toNode;
        this->buildCost = buildCost;
        this->destoryCost = destoryCost;
    }
    friend ostream& operator<<(ostream &os, const Edge &edge)
    {
        os << edge.fromNode << " ";
        os << edge.toNode << " ";
        os << edge.buildCost << " ";
        os << edge.destoryCost;
        os << endl;
        return os;
    }

};

struct less_than_build
{
    inline bool operator() (const Edge& edge1, const Edge& edge2)
    {
        return (edge1.buildCost < edge2.buildCost);
    }
};

struct less_than_destroy
{
    inline bool operator() (const Edge& edge1, const Edge& edge2)
    {
        return (edge1.destoryCost < edge2.destoryCost);
    }
};

bool isVisited(vector<int> *visited, int search) {
    bool found = (find(visited->begin(), visited->end(), search) != visited->end());
    return found;
}

vector<vector<int>*>* components = new vector<vector<int>*>();

void DFS(vector<vector<int>> *adjacency, vector<int> *visited, int curr, vector<int>* componentSingle) {

    visited->push_back(curr);
    componentSingle->push_back(curr);
    // cout << "curr: " << curr << endl;
    
    for (int i = 0; i < adjacency->size(); ++i) {

        if ((*adjacency)[curr][i] == 1 && // i is neighbour
            !isVisited(visited, i)) {
            // cout << "gonna visit " << i << endl; 
            // sleep(1);
            DFS(adjacency, visited, i, componentSingle);
        }
    }

}

void findSeparatedComponents(vector<vector<int>> *adjacency) {
    vector<int> *visited = new vector<int>();

    for (int i = 0; i < adjacency->size(); ++i) {
        if (!isVisited(visited, i)) {
            if (debug) {
                cout << "component " << i << endl;
            }
            
            vector<int>* componentSingle = new vector<int>();
            // visited->push_back(i); // done already in below DFS call
            DFS(adjacency, visited, i, componentSingle);
            components->push_back(componentSingle);

            if (debug) {
                cout << endl;
            }
        }
    }
}

class Node {
    public:

    int data;
    vector<Node*>* next;
    Edge* edge;

    Node(int data, Edge* edge) {
        this->data = data;
        next = new vector<Node*>();
        this->edge = edge;
    }

    void addNextNode(Node* node) {
        this->next->push_back(node);
    }

};

Edge* findEdge(vector<Edge> *edges, int from, int to) {
    // cout << "finding edge from=" << from << " to=" << to << "\n";

    for (Edge &edge : *edges) {
        // cout << "3\n";
        // cout << edge;

        if (edge.fromNode == from) {
            if (edge.toNode == to) {
                return &edge;
            }
        }
        else if (edge.fromNode == to) {
            if (edge.toNode == from) {
                return &edge;
            }
        }
    }
    return nullptr;
}

bool DFSDestroy(int curr, vector<vector<int>> *adjacency, vector<bool>* visited, stack<int>* stackNext, int prev) {
    (*visited)[curr] = true;
    stackNext->push(curr);
    
    // cout << "1 curr=" << curr << "\n";
    
    for (int i = 0; i < (*adjacency)[curr].size(); ++i) {
        // cout << "2 i=" << i << "\n";
        if (i == prev) { // not back where we came from
            continue;
        }

        if ((*adjacency)[curr][i] == 1) //neighbour
        {
            // cout << "3\n";

            if (!(*visited)[i]) 
            {
                // cout << "4 new fn stack \n";
                if (!DFSDestroy(i, adjacency, visited, stackNext, curr)) {
                    return false;
                }
                else {
                    //pop from stack yea
                    // cout << "poping from stack\n";
                    stackNext->pop();
                }
            }
            else if (!stackNext->empty() && i != stackNext->top()) // not same value as current
            {
                // cout << "5\n";
                if (debug) {
                    cout << "circular ref at=" << i << endl;
                }
                // cout << "stackNext->top()=" << stackNext->top() << endl;
                // cout << "curr=" << curr << endl;
                stackNext->push(i);
                return false;
            }
        }
    }
    // cout << "6\n";

    return true;
}

int destroyRoadsInComponents(vector<Edge> *edgesToDestroy, vector<vector<int>> *adjacency) {
    int destroyCost = 0;
    for (int i = 0; i < components->size(); ++i) 
    {
        // cout << "i=" << i << endl;
        // cout << "(*(*components)[i])[0]=" << (*(*components)[i])[0] << endl;
        // for (int j = 0; j < (*components)[i]->size(); ++j) 
        // {
        // Node* node = new Node((*(*components)[i])[j]);
        // Node* node = new Node((*(*components)[i])[0]);

        vector<bool>* visited = new vector<bool>();
        stack<int>* stackNext = new stack<int>();  

        while (true)
        {
            // sleep(1);
            // cout << "country=\n";
            // printVectorOfVector(*adjacency);
            vector<bool>* visited = new vector<bool>();
            for (int j = 0; j < (*adjacency)[i].size(); ++j) {
                visited->push_back(false);
            }

            stack<int>* stackNext = new stack<int>(); 

            if (DFSDestroy((*(*components)[i])[0], adjacency, visited, stackNext, -1)) {
                break;
            }


            //print stack
            // stack<int> tempStack = stack<int>(*stackNext);
            // cout << "stackNext= ";
            // while(!tempStack.empty())
            // {
            //     cout << tempStack.top() << " ";
            //     tempStack.pop();
            // }
            // cout << endl;

            int prev = -1;
            int init = -1;
            if (!stackNext->empty()) {
                prev = stackNext->top();
                init = prev;
                stackNext->pop();
            }

            vector<Edge>* edges = new vector<Edge>();

            // cout << "dfs path: ";
            while(!stackNext->empty()) {
                int curr = stackNext->top();
                // cout << curr << " ";
                Edge* edge = findEdge(edgesToDestroy, prev, curr);
                edges->push_back(*edge);
                if (edge == nullptr) {
                    cout << "error! cant find edge!\n";
                    exit(0);
                }
                stackNext->pop();
                prev = curr;
                if (curr == init) {
                    break;
                }
            }
            // cout << endl;
            //destroy road and try again

            sort(edges->begin(), edges->end(), less_than_destroy());
            Edge edge = (*edges)[0];
            // cout << "destroying edge= " << edge.fromNode << " to " << edge.toNode << endl;
            destroyCost += edge.destoryCost;
            (*adjacency)[edge.fromNode][edge.toNode] = 0;
            (*adjacency)[edge.toNode][edge.fromNode] = 0;

        }

        delete visited; //idk if im deleting it properly
        delete stackNext;

        // }
    }
    return destroyCost;
}

bool checkCircularRef(int i, vector<vector<int>> *adjacency) {

    vector<bool>* visited = new vector<bool>();
    // cout << "(*adjacency)[i].size(): " << (*adjacency)[i].size() << endl;
    for (int j = 0; j < (*adjacency)[i].size(); ++j) {
        // cout << "j " << j;
        visited->push_back(false);
    }

    stack<int>* stackNext = new stack<int>(); 

    return !DFSDestroy((*(*components)[i])[0], adjacency, visited, stackNext, -1);
}

//building roads
int buildRoadsAmongComponents(vector<Edge> *edgesToBuild, vector<vector<int>> *adjacency) 
{
    int buildCost = 0;

    set<pair<int, int>>* visited = new set<pair<int, int>>();

    vector<Edge> edgesMain = vector<Edge>();

    for (int i = 0; i < components->size(); i++) 
    {
        for (int j = 0; j < components->size(); j++) {
            if (i == j) {
                continue;
            }

            pair<int, int> currVisited = pair<int, int>();
            currVisited.first = i;
            currVisited.second = j;
            if (visited->find(currVisited) != visited->end()) {
                continue;
            }
            currVisited = pair<int, int>();
            currVisited.first = j;
            currVisited.second = i;
            if (visited->find(currVisited) != visited->end()) {
                continue;
            }
            visited->insert(currVisited);

            vector<int>* firstComp = (*components)[i];
            vector<int>* secondComp = (*components)[j];

            vector<Edge> edges = vector<Edge>();

            for (int f : *firstComp) 
            {
                for (int s : *secondComp) {
                    // cout << "1\n";
                    Edge* edge = findEdge(edgesToBuild, f, s);
                    // cout << f << " " << s;
                    // cout << "edge=" << edge;
                    edges.push_back(*edge);
                }
            }

            sort(edges.begin(), edges.end(), less_than_build()); //TODO: can use normal int min compare here instead of edge list compare
            Edge edgeMinInCompPair = edges[0]; //this is the minimum build cost between components firstComp and secondComp
            if (debug) {
                cout << "Min edge between comp " << i << " and " << j << " is " << edgeMinInCompPair;
            }
            edgesMain.push_back(edgeMinInCompPair);

        }
    }

    if(edgesMain.size() <= 0) {
        if (debug) {
            cout << "No roads to build!\n";
        }
        return 0;
    }

    if (debug) {
        cout << "building roads\n";
    }

    sort(edgesMain.begin(), edgesMain.end(), less_than_build());

    int count = 0;
    for (int i = 0; i < components->size(); i++) {
        Edge edge = edgesMain[i];
        if (debug) {
            cout << "building edge= " << edge.fromNode << " to " << edge.toNode << endl;
        }
        (*adjacency)[edge.fromNode][edge.toNode] = 1;
        (*adjacency)[edge.toNode][edge.fromNode] = 1;
        if (checkCircularRef(i, adjacency)) {
            (*adjacency)[edge.fromNode][edge.toNode] = 0;
            (*adjacency)[edge.toNode][edge.fromNode] = 0;
        }
        else {
            count++;
            buildCost += edge.buildCost;
        }
        if (count >= components->size() - 1) {
            break;
        }
    }

    return buildCost;
}


int main(int argc, char* argv[]) {
    // testing
    // int num = '0' - '0';
    // cout << num << "\n";
    // cout << typeid(num).name() << "\n";
    // return 0;

    if (argc > 1) {
        debug = true;
    }

    string input;
    getline(cin, input);

    string countryStr;
    string buildCostStr;
    string destroyCostStr;

    vector<string> arrStrings;
    int pos = 0;
    int prevPos = 0;
    string oneInput;

    while ((pos = input.find_first_of(' ', pos)) && pos != -1) {
        pos = input.find_first_of(' ', pos);
        // cout << "prev pos: " << prevPos << endl; 
        // cout << "pos: " << pos << endl; 
        oneInput = input.substr(prevPos, pos - prevPos);
        // cout << "oneInput: " << oneInput << endl; 

        // if (input[input.length() - 1] == '\r\n') {
        //     break;
        // }
        arrStrings.push_back(oneInput);
        pos++;
        prevPos = pos;
    }

    arrStrings.push_back(input.substr(prevPos, input.length() - prevPos));

    vector<vector<int>> *country = inputHandling(arrStrings[0], false);
    vector<vector<int>> *buildCost = inputHandling(arrStrings[1], true);
    vector<vector<int>> *destroyCost = inputHandling(arrStrings[2], true);

    if (debug) {
        cout << "country: \n";
        printVectorOfVector(*country);
        cout << "buildCost: \n";
        printVectorOfVector(*buildCost);
        cout << "destroyCost: \n";
        printVectorOfVector(*destroyCost);
    }

    // int ctr = 1;
    // for (vector<int> &v : *country) {
    //     int idx = ctr - 1;
    //     Node* node = new Node(ctr, buildCost[idx])
    //     ctr++;
    // }

    vector<Edge> *edgesToBuild = new vector<Edge>(); //TODO: If needed, to make searching faster, we can store edges as hashmaps in hashmaps. But probably not needed
    vector<Edge> *edgesToDestroy = new vector<Edge>();

    //ascending order
    //buildCost
    for (int i = 0; i < country->size(); ++i) {
        for (int j = i; j < (*country)[i].size(); ++j) {
            int edgeExists = (*country)[i][j];
            // cout << edgeExists << " ";
            if (edgeExists) {
                //destroy cost
                Edge edge = Edge(i, j, -1, (*destroyCost)[i][j]);
                edgesToDestroy->push_back(edge); //todo: need to handle i == j always destroy case?
            }
            else {
                if (i == j) {
                    continue;
                }
                //build cost
                Edge edge = Edge(i, j, (*buildCost)[i][j], -1);
                edgesToBuild->push_back(edge);
            }
        }
        // cout << endl;
    }

    // cout << "edgesToBuild: " << endl;
    // for (Edge &edge : *edgesToBuild) {
    //     cout << edge;
    // }
    // cout << endl;
    
    // cout << "edgesToDestroy: " << endl;
    // for (Edge &edge : *edgesToDestroy) {
    //     cout << edge;
    // }
    // cout << endl;

    // sort(edgesToBuild->begin(), edgesToBuild->end(), less_than_build());
    // sort(edgesToDestroy->begin(), edgesToDestroy->end(), less_than_destroy());

    if (debug) {
        cout << "edgesToBuild: " << endl;
        for (Edge &edge : *edgesToBuild) {
            cout << edge;
        }
        cout << endl;
        
        cout << "edgesToDestroy: " << endl;
        for (Edge &edge : *edgesToDestroy) {
            cout << edge;
        }
        cout << endl;
    }

    findSeparatedComponents(country);

    int destroyCostN = destroyRoadsInComponents(edgesToDestroy, country);
    if (debug) {
        cout << "destroyCostN = " << destroyCostN << endl;
    }

    int buildCostN = buildRoadsAmongComponents(edgesToBuild, country);
    if (debug) {
        cout << "buildCostN = " << buildCostN << endl;

        cout << "Total Cost = " << buildCostN + destroyCostN << endl;
    }
    
    cout << buildCostN + destroyCostN << endl;

    //next steps
    // 1) (Skip) Put the 3 vectors into Node and Graph class, in that structure
    // 2) Ascending order of build cost and destroy cost separately (all nodes not actually needed, will come to that later)
    // 3) test.txt case, find out what prof actually wants.. 
    // Afaik, destroy least costly roads in each component, such that circular refs are not there anymore, and all nodes connected in that component. Similarly, for other components.
    // Then, connect up components with least build cost between nodes of the different components....... 

    // cout << "mods vector:\n";
    // for (string s : mods)
    // {
    //     cout << s << "|";
    // }
    // cout << endl;

    

    // cout << "searching " << hashTbl.search("aaa") << endl;
    // cout << "searching " << hashTbl.search("bbb") << endl;
    // cout << "searching " << hashTbl.search("ccc") << endl;
    // cout << "searching " << hashTbl.search("cccc") << endl;
    

    // cout << endl;
    // cout << input << endl;
    // cout << oneInput << endl;
    // cout << pos << endl;
    // cout << prevPos << endl;
    // cout << traversalTypeStr << endl;

    return 0;
}