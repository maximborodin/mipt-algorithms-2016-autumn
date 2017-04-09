#include <iostream>
#include <cstdio>
#include <vector>

const int INF = 1000*1000*1000;

class Algorithm
{
public:
    Algorithm();
    void getInput();
    void solve();

    int getCost();
    std::vector<int> getAns();
private:
    int size;
    int cost;

    std::vector<int> u;
    std::vector<int> v;
    std::vector<int> parents;
    std::vector<int> way;
    std::vector<std::vector<int> > table;
    std::vector<int> ans;
};

int main()
{
    Algorithm algorithm;
    algorithm.getInput();
    algorithm.solve();
    std::cout << algorithm.getCost() << std::endl;
    std::vector<int> ans = algorithm.getAns();
    for (int i = 1;i < ans.size();i++){
        std::cout << i << " " << ans[i] << std::endl;
    }
}

Algorithm::Algorithm ()
{
    u.clear();
    v.clear();
    parents.clear();
    way.clear();
}

void Algorithm::getInput()
{
    std::cin >> size;
    table.resize(size + 1);
    for (size_t i = 1;i <= size;++i){
        table[i].resize(size + 1);
        for (size_t j = 1;j <= size;++j){
            std::cin >> table[i][j];
        }
    }
    u.resize(size + 1);
    v.resize(size + 1);
    parents.resize(size + 1);
    way.resize(size + 1);
    ans.resize(size + 1);
}

void Algorithm::solve()
{
    for (int i = 1;i <= size;++i){
        parents[0] = i;
        int freeCol = 0;
        std::vector<int> minv(size + 1, INF);
        std::vector<char> used (size + 1, false);
        do{
            used[freeCol] = true;
            int freeRow = parents[freeCol];
            int delta = INF;
            int freeCol1;
            for (int j = 1;j <= size;++j){
                if (!used[j]){
                    int current = table[freeRow][j] - u[freeRow] - v[j];
                    if (current < minv[j]){
                        minv[j] = current;
                        way[j] = freeCol;
                    }
                    if (minv[j] < delta){
                        delta = minv[j];
                        freeCol1 = j;
                    }
                }
            }
            for (int j = 0;j <= size;++j){
                if (used[j]){
                    u[parents[j]] += delta;
                    v[j] -= delta;
                }
                else{
                    minv[j] -= delta;
                }
            }
            freeCol = freeCol1;
        } while (parents[freeCol] != 0);
        do{
            int freeCol1 = way[freeCol];
            parents[freeCol] = parents[freeCol1];
            freeCol = freeCol1;
        } while (freeCol);
    }
    for (int j = 1;j <= size;++j){
        ans[parents[j]] = j;
    }
}

int Algorithm::getCost()
{
    return (-v[0]);
}

std::vector<int> Algorithm::getAns ()
{
    return ans;
}