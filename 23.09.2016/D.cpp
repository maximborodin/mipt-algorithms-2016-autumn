#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <queue>
#include <cstring>

const int INF = 1000 * 1000 * 1000;
const int MAX_NODES = 150;

class Algorithm
{
public:
    Algorithm();
    bool getInput (std::istream& in);
    int  edmondsKarp();
    int getTestCount ();
    void changeEdge(int type, int from, int to);
    void clear();
private:
    int bfs ();

    int vertexCount;
    int source, target, edgeCount;
    int testCount;
    int capacities[MAX_NODES][MAX_NODES];
    int flowPassed[MAX_NODES][MAX_NODES];
    std::vector<int> graph[MAX_NODES];
    int parentsList[MAX_NODES];
    int currentPathCapacity[MAX_NODES];
};




int main ()
{
    Algorithm algorithm;
    algorithm.getInput(std::cin);
    std::cout << algorithm.edmondsKarp() << std::endl;
    int type, from, to;
    for (int i = 0;i < algorithm.getTestCount();i++){
        algorithm.clear();
        std::cin >> type >> from >> to;
        from--;
        to--;
        algorithm.changeEdge(type, from, to);
        std::cout << algorithm.edmondsKarp() << std::endl;
    }
    return 0;
}

Algorithm::Algorithm()
{
    source = 0;
}

void Algorithm::changeEdge(int type, int from, int to)
{
    if (type == 1){
        capacities[from][to]++;
    }
    else{
        if (capacities[from][to] != 0){
            capacities[from][to]--;
        }
    }
}

int Algorithm::getTestCount()
{
    return testCount;
}

int Algorithm::edmondsKarp()
{
     int maxFlow = 0;
     while(true){
         int flow = bfs();
         if(flow == 0){
             break;
         }
         maxFlow += flow;
         int currentNode = target;
         while(currentNode != source) {
             int previousNode = parentsList[currentNode];
             flowPassed[previousNode][currentNode] += flow;
             flowPassed[currentNode][previousNode] -= flow;
             currentNode=previousNode;
         }
     }
     return maxFlow;
}


int Algorithm::bfs()
{
     std::memset(parentsList, -1, sizeof(parentsList));
     memset(currentPathCapacity, 0, sizeof(currentPathCapacity));
     std::queue<int> queue;
     queue.push(source);
     parentsList[source]=-2;
     currentPathCapacity[source]=INF;
     while(!queue.empty()){
         int currentNode = queue.front();
         queue.pop();
         for(int i = 0; i<graph[currentNode].size(); i++){
             int to = graph[currentNode][i];
             if(parentsList[to] == -1) {
                 if(capacities[currentNode][to] - flowPassed[currentNode][to] > 0) {
                     parentsList[to] = currentNode;
                     currentPathCapacity[to] = std::min(currentPathCapacity[currentNode],
                                                   capacities[currentNode][to] - flowPassed[currentNode][to]);

                     if(to == target){
                         return currentPathCapacity[target];
                     }
                     queue.push(to);
                 }
             }
         }
     }
     return 0;
}

bool Algorithm::getInput (std::istream& in)
{
    in >> vertexCount;
    if (vertexCount == 0){
        return false;
    }
    target = vertexCount - 1;
    in >> edgeCount;
    int from, to, capacity;
    for (int i = 0;i < edgeCount;i++){
        in >> from >> to >> capacity;
        from--;
        to--;
        capacities[from][to] = capacity;
        graph[from].push_back(to);
        graph[to].push_back(from);
    }
    in >> testCount;
    return true;
}




void Algorithm::clear()
{
    for (int i = 0;i < vertexCount;i++){
        for (int j = 0;j < vertexCount;j++){
            flowPassed[i][j] = 0;
        }
    }
    /*for (int i = 0;i < vertexCount;i++){
        currentPathCapacity[i] = 0;
    }*/
}
