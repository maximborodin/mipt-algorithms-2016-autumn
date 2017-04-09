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
    void addEdge (int from, int to);
    void clear();
    int dinic();
    int getAnswer();
    int getColor(int i, int j);
    int getNum (int i, int j);
    void findNeighbours (int i, int j);
private:
    bool bfs();
    int dfs (int v, int flow);

    int vertexCount;
    int n, m, a, b;
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
    std::cout << algorithm.getAnswer() << std::endl;
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
    in >> n >> m >> a >> b;
    map.resize(n);
    graph.resize(m * n + 2);
    distances.resize(m * n + 2);
    pointers.resize(m * n + 2);
    for (int i = 0;i < n;i++){
        map[i].resize(m);
        for (int j = 0;j < m;j++){
            in >> map[i][j];
            if (map[i][j] == '*') {
                map[i][j] = 1;
                vertexCount++;
            }
            /*else if (map[i][j] != '*' && map[i][j] != '.'){
                map[-2][-3] = 0;
            }*/
            else{
                map[i][j] = 0;
            }
        }
    }
    for (int i = 0;i < n;i++){
        for (int j = 0;j < m;j++){
            if (map[i][j] == 1) {
                findNeighbours(i, j);
            }
        }
    }
}

void Algorithm::addEdge (int from, int to)//right
{
    Edge e1(from, to, 1, 0);
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

int Algorithm::getAnswer()
{
    if (2 * b <= a){
        return (vertexCount - 2) * b;
    }
    else{
        int maxFlow = dinic();
        return (maxFlow * a) + (vertexCount - 2 - (2 * maxFlow)) * b;
    }
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
        addEdge (source, num);
        if (i > 0){
            if (map[i - 1][j] == 1){
                addEdge (num, getNum(i - 1, j));
            }
        }
        if (i < n - 1){
            if (map[i + 1][j] == 1) {
                addEdge(num, getNum(i + 1, j));
            }
        }
        if (j > 0){
            if (map[i][j - 1] == 1){
                addEdge (num, getNum(i, j - 1));
            }
        }
        if (j < m - 1){
            if (map[i][j + 1]){
                addEdge(num, getNum(i, j + 1));
            }
        }
    }
    else{
        addEdge(getNum(i, j), target);
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
    a = 0;
    b = 0;
}