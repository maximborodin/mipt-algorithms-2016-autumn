#include <iostream>
#include <algorithm>
#include <set>

int n, k;
int girlsCount;
int boysCount;
std::vector< std::vector<int> > fullGraph;
std::vector < std::vector<int> > g;
std::vector<int> mt;
std::vector<char> used;
std::vector<int> usefulGirls;
std::vector<char> usefulBoys;

const int VIRGIN = 0;
const int NORMAL = 1;
const int CLIMAX = 2;

bool try_kuhn (int v) {
    if (used[v])  return false;
    used[v] = true;
    for (size_t i = 0; i < g[v].size(); ++i) {
        int to = g[v][i];
        if (mt[to] == -1 || try_kuhn (mt[to])) {
            mt[to] = v;
            return true;
        }
    }
    return false;
}

int main() {
    std::cin >> n >> k;
    fullGraph.resize(n);
    for (int i = 0;i < n;++i){
        int edgesCount;
        std::cin >> edgesCount;
        for (int j = 0;j < edgesCount;++j){
            int a;
            std::cin >> a;
            a--;
            fullGraph[i].push_back(a);
        }
    }

    int queries;
    std::cin >> queries;

    for (int i = 0;i < queries;++i){
        g.clear();
        g.resize(n);
        usefulGirls.assign(k, VIRGIN);
        usefulBoys.assign(n, false);

        girlsCount = 0;
        std::cin >> boysCount;
        for (int j = 0;j < boysCount;j++){
            int a;
            std::cin >> a;
            a--;
            usefulBoys[a] = true;
            for (int t = 0;t < fullGraph[a].size();++t){
                int curGirl = fullGraph[a][t];
                if (usefulGirls[curGirl] == VIRGIN){
                    girlsCount++;
                    usefulGirls[curGirl] = NORMAL;
                }
            }
        }
        for (int j = 0;j < n;++j){
            if (!usefulBoys[j]) {
                for (int t = 0; t < fullGraph[j].size(); ++t) {
                    if (usefulGirls[fullGraph[j][t]] == NORMAL) {
                        g[j].push_back(fullGraph[j][t]);
                    }
                }
            }
            else{
                for (int t = 0;t < fullGraph[j].size();++t){
                    usefulGirls[fullGraph[j][t]] = CLIMAX;
                }
            }
        }

        int matching = 0;
        mt.assign (k, -1);
        for (int v = 0; v < n; ++v) {
            used.assign (n, false);
            if (try_kuhn (v)){
                matching++;
            }
        }
        if (matching == girlsCount){
            std::cout << "Yes" << std::endl;
        }
        else{
            std::cout << "No" << std::endl;
        }
    }

}