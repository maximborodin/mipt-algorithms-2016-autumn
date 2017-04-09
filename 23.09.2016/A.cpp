#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include <queue>

const int INF = 1000 * 1000 * 1000;

class Algorithm
{
public:
    class Edge
    {
    public:
        int from, to, capacity;
        int flow;
        Edge(){}
        Edge (int from_, int to_, int capacity_, int flow_) :
                from(from_), to(to_),capacity(capacity_), flow(flow_)
        {}
    };
    Algorithm();
    bool getInput (std::istream& in);
    void addEdge (int from, int to, int capacity);
    void clear();
    int dinic();
private:
    bool bfs();
    int dfs (int v, int flow);

    int vertexCount;
    int source, target, edgeCount;
    std::vector<std::vector<int> > graph;
    std::vector<Edge> edges;
    std::vector<int> distances;
    std::vector<int> pointers;
};




int main ()
{
    Algorithm algorithm;
    while (algorithm.getInput(std::cin)){
        std::cout << algorithm.dinic() << std::endl;
        algorithm.clear();
    }
    return 0;
}

Algorithm::Algorithm(){}


bool Algorithm::getInput (std::istream& in)
{
    in >> vertexCount;
    if (vertexCount == 0){
        return false;
    }
    graph.resize(vertexCount);
    distances.resize(vertexCount);
    pointers.resize(vertexCount);
    in >> source >> target >> edgeCount;
    source--;
    target--;
    int from, to, capacity;
    for (int i = 0;i < edgeCount;i++){
        in >> from >> to >> capacity;
        from--;
        to--;
        addEdge (from, to, capacity);
        addEdge (to, from, capacity);
    }
    return true;
}

void Algorithm::addEdge (int from, int to, int capacity)//right
{
    Edge e1(from, to, capacity, 0);
    Edge e2(to, from, 0, 0);
    graph[from].push_back(edges.size());
    edges.push_back(e1);
    graph[to].push_back(edges.size());
    edges.push_back(e2);
}

bool Algorithm::bfs()//right
{
    std::queue<int> queue;
    queue.push(source);
    distances.assign(vertexCount, -1);
    distances[source] = 0;
    while ((!queue.empty()) && (distances[target] == -1)){
        int v = queue.front();
        queue.pop();
        for (int i = 0;i < graph[v].size();i++){
            int id = graph[v][i];
            int to = edges[id].to;
            if (distances[to] == -1 && (edges[id].flow < edges[id].capacity)){
                queue.push(to);
                distances[to] = distances[v] + 1;
            }
        }
    }
    return (distances[target] != -1);
}

int Algorithm::dfs (int v, int flow) // right
{
    if (!flow){
        return 0;
    }
    if (v == target){
        return flow;
    }
    for (;pointers[v] < graph[v].size();++pointers[v]){
        int id = graph[v][pointers[v]];
        int to = edges[id].to;
        if (distances[to] != distances[v] + 1){
            continue;
        }
        int pushed = dfs(to, std::min
                (flow, (int)edges[id].capacity - edges[id].flow));
        if (pushed){
            edges[id].flow += pushed;
            edges[id ^ 1].flow -= pushed;
            return pushed;
        }
    }
    return 0;
}

int Algorithm::dinic () //right
{
    int flow = 0;
    for(;;){
        if (!bfs()){
            break;
        }
        pointers.assign(vertexCount, 0);
        while (int pushed = dfs(source, INF)){
            flow += pushed;
        }
    }
    return flow;
}

void Algorithm::clear()
{
    pointers.clear();
    for (int i = 0;i < graph.size();i++){
        graph[i].clear();
    }
    graph.clear();
    edges.clear();
    distances.clear();
    vertexCount = 0;
    edgeCount = 0;
    source = 0;
    target = 0;
}