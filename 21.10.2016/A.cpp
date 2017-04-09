#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include <queue>

const int INF = 2000 * 1000 * 1000;

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
    void getInput (std::istream& in);
    void addEdge (int from, int to, int capacity);
    void clear();
    int dinic();
    bool getAnswer();
    int getColor(int i, int j);
    int getNum (int i, int j);
    void findNeighbours (int i, int j);
private:
    bool bfs();
    int dfs (int v, int flow);

    int vertexCount;
    int n, m;
    int source, target;
    std::vector<std::vector<char> > map;
    std::vector<std::vector<int> > graph;
    std::vector<Edge> edges;
    std::vector<int> distances;
    std::vector<int> pointers;
};


int main ()
{
    Algorithm algorithm;
    algorithm.getInput(std::cin);
    if (algorithm.getAnswer()){
        std::cout << "Valid" << std::endl;
    }
    else{
        std::cout << "Invalid" << std::endl;
    }
    algorithm.clear();
    return 0;
}

Algorithm::Algorithm()
{
    vertexCount = 2;
    source = 0;
    target = 1;
}


void Algorithm::getInput (std::istream& in)
{
    in >> n >> m;
    map.resize(n);
    vertexCount = m * n + 2;
    graph.resize(vertexCount);
    distances.resize(vertexCount);
    pointers.resize(vertexCount);
    for (int i = 0;i < n;i++){
        map[i].resize(m);
        for (int j = 0;j < m;j++){
            in >> map[i][j];
            if (map[i][j] == '.') {
                map[i][j] = 0;
            }
            else if (map[i][j] == 'H'){
                map[i][j] = 1;
            }
            else if (map[i][j] == 'O'){
                map[i][j] = 2;
            }
            else if (map[i][j] == 'N'){
                map[i][j] = 3;
            }
            else{
                map[i][j] = 4;
            }
        }
    }
    for (int i = 0;i < n;i++){
        for (int j = 0;j < m;j++){
            findNeighbours(i, j);
        }
    }
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
    distances.assign(n * m + 2, -1);
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
                (flow, edges[id].capacity - edges[id].flow));
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
        pointers.assign(n * m + 2, 0);
        while (int pushed = dfs(source, INF)){
            flow += pushed;
        }
    }
    return flow;
}

bool Algorithm::getAnswer()
{
    int vertexSum = 0;
    for (int i = 0;i < n;i++){
        for (int j = 0;j < m;j++){
            vertexSum += map[i][j];
        }
    }
    if (vertexSum % 2 == 1){
        return false;
    }
    if (vertexSum == 0){
        return false;
    }
    vertexSum /= 2;
    int maxFlow = dinic();
    if (maxFlow == vertexSum){
        return true;
    }
    return false;
}

int Algorithm::getColor(int i, int j)
{
    if ((i + j) % 2 == 0){
        return 0;
    }
    return 1;
}


int Algorithm::getNum (int i, int j)
{
    return (i * m + j + 2);
}

void Algorithm::findNeighbours(int i, int j)
{
    if (!getColor(i, j)){
        int num = getNum(i, j);
        addEdge (source, num, map[i][j]);
        if (i > 0){
            addEdge (num, getNum(i - 1, j), 1);
        }
        if (i < n - 1){
            addEdge(num, getNum(i + 1, j), 1);
        }
        if (j > 0){
            addEdge (num, getNum(i, j - 1), 1);
        }
        if (j < m - 1){
            addEdge(num, getNum(i, j + 1), 1);
        }
    }
    else{
        addEdge(getNum(i, j), target, map[i][j]);
    }
}

void Algorithm::clear()
{
    pointers.clear();
    for (int i = 0;i < graph.size();i++){
        graph[i].clear();
    }
    for (int i = 0;i < map.size();i++){
        map[i].clear();
    }
    map.clear();
    graph.clear();
    edges.clear();
    distances.clear();
    vertexCount = 0;
    source = 0;
    target = 0;
    n = 0;
    m = 0;
}