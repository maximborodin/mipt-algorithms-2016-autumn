#include <cstdio>
#include <vector>
#include <iostream>
#include <queue>

const int INF = 1000*1000*1000;

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
    void getInput(std::istream& in);
    void addEdge(int from, int to, int capacity);
    int dinic();
    bool bfs();
    int dfs (int v, int flow);
    bool isSuitable (int i, int j);

    int n, m, k, l;
    size_t vertexCount;
    int source, target;
    std::vector<std::vector<int> > graph;
    std::vector<Edge> edges;
    std::vector<std::vector<int> > map;
    std::vector<int> distances;
    std::vector<int> pointers;

    std::vector<std::vector<int> > ids;
    std::vector<std::pair<int, int> > vs;
    std::vector<char> used;
    std::vector<char> chapter;
};


void mainDfs (Algorithm& algorithm, int v)
{
    if (v != algorithm.source){
        algorithm.chapter[v] = 1;
    }
    algorithm.used[v] = 1;
    for (int i = 0;i < algorithm.edges.size();++i){
        if (algorithm.edges[i].from == v) {
            if ((algorithm.edges[i].flow != algorithm.edges[i].capacity) &&
                !algorithm.used[algorithm.edges[i].to]) {
                mainDfs(algorithm, algorithm.edges[i].to);
            }
        }
    }
}

int main()
{
    Algorithm algorithm;
    algorithm.getInput(std::cin);
    int ans = algorithm.dinic();
    if (ans >= INF){
        std::cout << -1 << std::endl;
        return 0;
    }
    std::cout << ans << std::endl;
    mainDfs(algorithm, algorithm.source);
    for (int i = 0;i < algorithm.n;++i){
        for (int j = 0;j < algorithm.m;++j){
            if (algorithm.map[i][j] == 1 && algorithm.chapter[algorithm.ids[i][j]] == 1 &&
                    algorithm.chapter[algorithm.ids[i][j] + 1] == 0){
                std::cout << i + 1 << " " << j + 1 << std::endl;
            }
        }
    }
    return 0;
}

Algorithm::Algorithm()
{
    source = 0;
    target = 1;
}

void Algorithm::getInput(std::istream& in)
{
    in >> n >> m >> k >> l;
    vertexCount = 2 * n * m + 2;
    graph.resize(vertexCount);
    used.resize(vertexCount);
    chapter.resize(vertexCount);
    distances.resize(vertexCount);
    int tmp = 2;
    map.resize(n);
    ids.resize(n);
    vs.resize(vertexCount);
    for (int i = 0;i < n;++i){
        map[i].assign(m, INF);
        ids[i].assign(m, 0);
    }
    int a, b;
    for (int i = 0; i < k; ++i) {
        in >> a >> b;
        map[a - 1][b - 1] = 0; //mountains
    }
    for (int i = 0; i < l; ++i) {
        in >> a >> b;
        map[a - 1][b - 1] = 1; //may build
    }
    for (int i = 0;i < n;++i){
        for (int j = 0;j < m;++j){
            ids[i][j] = tmp;
            vs[tmp] = std::make_pair(i, j);
            ++tmp;
            if (map[i][j] == 1){
                vs[tmp] = std::make_pair(i, j);
                ++tmp;
            }
        }
    }
    in >> a >> b;
    addEdge(source, ids[a - 1][b - 1], INF);
    in >> a >> b;
    addEdge(ids[a - 1][b - 1], target, INF);
    for (int i = 0;i < n;++i){
        for (int j = 0;j < m;++j){
            if (map[i][j] > 0) {
                tmp = ids[i][j];
                if (map[i][j] == 1) {
                    addEdge(tmp, tmp + 1, 1);
                    ++tmp;
                }
                if (isSuitable(i - 1, j)){
                    addEdge(tmp, ids[i - 1][j], INF);
                }
                if (isSuitable(i, j - 1)){
                    addEdge(tmp, ids[i][j - 1], INF);
                }
                if (isSuitable(i + 1, j)){
                    addEdge(tmp, ids[i + 1][j], INF);
                }
                if (isSuitable(i, j + 1)){
                    addEdge(tmp, ids[i][j + 1], INF);
                }
            }
        }
    }
}

void Algorithm::addEdge(int from, int to, int capacity)
{
    Edge e1(from, to, capacity, 0);
    Edge e2(to, from, 0, 0);
    graph[from].push_back(edges.size());
    edges.push_back(e1);
    graph[to].push_back(edges.size());
    edges.push_back(e2);
}


int Algorithm::dinic()
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

bool Algorithm::bfs()
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


int Algorithm::dfs(int v, int flow)
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

bool Algorithm::isSuitable(int i, int j)
{
    if (i >= 0 && i < n && j >= 0 && j < m){
        if (map[i][j] > 0){
            return true;
        }
    }
    return false;
}