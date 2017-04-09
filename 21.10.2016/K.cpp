#include <iostream>
#include <vector>
#include <algorithm>


class Vertex
{
public:
    Vertex()
    {
        edges.clear();
        weight = 0;
    }
    std::vector<std::pair<int, int> > edges; //first - to, second - edgeNum;
    int weight;
};

bool cmp (const Vertex& v1, const Vertex& v2)
{
    if (v1.weight >= v2.weight){
        return true;
    }
    return false;
}

int n, m, e;

std::vector<Vertex> firstDole;
std::vector<int> secondDole;
std::vector<std::pair<int, int> > mt;
std::vector<char> used;
std::vector<std::pair<int, int> > firstDolePartners;
std::vector<int> endOfChain;

bool tryKuhn(int v)
{
    if (used[v]){
        return false;
    }
    used[v] = 1;

    for (int i = 0;i < firstDole[v].edges.size();++i){
        int to = firstDole[v].edges[i].first;
        if (mt[to].first == -1 &&
            (endOfChain[v] == -1 || secondDole[endOfChain[v]] < secondDole[to])){
            endOfChain[v] = to;
            firstDolePartners[v] = std::make_pair(to, firstDole[v].edges[i].second);
        }
        else if (mt[to].first != -1 && tryKuhn(mt[to].first)){
            if ((secondDole[endOfChain[v]] < secondDole[endOfChain[mt[to].first]]) ||
                endOfChain[v] == -1){
                endOfChain[v] = endOfChain[mt[to].first];
                firstDolePartners[v] = std::make_pair(to, firstDole[v].edges[i].second);
            }
        }
    }
    if (endOfChain[v] != -1){
        return true;
    }
    return false;
}


int main ()
{
    std::cin >> n >> m >> e;
    mt.assign(m, std::make_pair(-1, -1));   //first - vertex, second - edge
    firstDole.resize(n);
    firstDolePartners.assign(n, std::make_pair(-1, -1));
    used.assign(n, 0);
    endOfChain.assign(n, -1);

    secondDole.resize(m);
    for (int i = 0; i < n; ++i) {
        std::cin >> firstDole[i].weight;
    }
    for (int i = 0; i < m; ++i) {
        std::cin >> secondDole[i];
    }
    for (int i = 0;i < e;++i){
        int a, b;
        std::cin >> a >> b;
        --a;
        --b;
        firstDole[a].edges.push_back(std::make_pair(b, i));
    }
    sort(firstDole.begin(), firstDole.end(), cmp);
    for (int i = 0;i < n;++i){
        if (tryKuhn(i)) {
            int k = i;
            while (mt[firstDolePartners[k].first].first != -1){
                int tmp = mt[firstDolePartners[k].first].first;
                mt[firstDolePartners[k].first] = std::make_pair(k, firstDolePartners[k].second);

                k = tmp;
            }
            mt[firstDolePartners[k].first] = std::make_pair(k, firstDolePartners[k].second);
            used.assign(n, 0);
            endOfChain.assign(n, -1);
            firstDolePartners.assign(n, std::make_pair(-1, -1));
        }
    }
    int maxWeight = 0;
    int matching = 0;
    for (int i = 0;i < m;++i){
        if (mt[i].first != -1){
            maxWeight += secondDole[i] + firstDole[mt[i].first].weight;
            ++matching;
        }
    }
    std::cout << maxWeight << std::endl;
    std::cout << matching << std::endl;
    for (int i = 0;i < m;++i){
        if (mt[i].first != -1){
            std::cout << mt[i].second + 1 << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}