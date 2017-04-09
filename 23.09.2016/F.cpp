#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>


class Algorithm
{
public:
    Algorithm();
    int getLeftSize();
    int getRightSize();
    void getInput(std::istream& in);
    void clear();
    std::pair<std::vector<int>, std::vector<int> > findControlSet();
private:
    bool matchVisit(int v);
    void controlVisit(int v);

    std::vector<char> right;
    std::vector<char> used;
    std::vector<int> match;
    std::vector<std::vector<int> > graph;
    std::size_t leftSize, rightSize;
};


int main ()
{
    Algorithm algorithm;
    std::size_t testCount;
    std::cin >> testCount;
    for (int i = 0;i < testCount;i++){
        algorithm.getInput(std::cin);
        std::pair<std::vector<int>, std::vector<int> > tmp;
        tmp = algorithm.findControlSet();
        int leftSize = algorithm.getLeftSize();
        int rightSize = algorithm.getRightSize();
        std::vector<char> left;
        left.resize(leftSize);
        std::vector<char> right;
        right.resize(rightSize);
        int firstAns = left.size() - tmp.first.size();
        int secondAns = right.size() - tmp.second.size();
        for (int j = 0;j < left.size();j++){
            left[j] = 1;
        }
        for (int j = 0;j < tmp.first.size();j++){
            left[tmp.first[j]] = 0;
        }
        for (int j = 0;j < right.size();j++){
            right[j] = 1;
        }
        for (int j = 0;j < tmp.second.size();j++){
            right[tmp.second[j]] = 0;
        }
        std::cout << firstAns + secondAns << std::endl;
        std::cout << firstAns << " " << secondAns << std::endl;
        for (int j = 0;j < left.size();j++){
            if (left[j] == 1) {
                std::cout << j + 1 << " ";
            }
        }
        std::cout << std::endl;
        for (int j = 0;j < right.size();j++){
            if (right[j] == 1) {
                std::cout << j + 1 << " ";
            }
        }
        std::cout << std::endl;
        std::cout << std::endl;
        algorithm.clear();
    }
    return 0;
}

Algorithm::Algorithm() {}

int Algorithm::getLeftSize()
{
    return leftSize;
}

int Algorithm::getRightSize()
{
    return rightSize;
}

void Algorithm::getInput(std::istream& in)
{
    in >> leftSize >> rightSize;
    graph.resize(leftSize);
    int girl;
    std::vector<char> tmp;
    tmp.resize(rightSize);
    for (int i = 0;i < leftSize;i++){
        in >> girl;
        while (girl != 0){
            tmp[girl - 1] = 1;
            in >> girl;
        }
        for (int j = 0;j < rightSize;j++){
            if (tmp[j] == 0){
                graph[i].push_back(j);
            }
            tmp[j] = 0;
        }
    }
}

void Algorithm::clear()
{
    leftSize = 0;
    rightSize = 0;
    right.clear();
    used.clear();
    match.clear();
    for (int i = 0;i < graph.size();i++){
        graph[i].clear();
    }
    graph.clear();
}

std::pair<std::vector<int>, std::vector<int> >
Algorithm::findControlSet ()
{
    match.assign(rightSize, -1);
    used.assign(leftSize, 0);

    for (int i = 0;i < leftSize;i++){
        used.assign(leftSize, 0);
        matchVisit(i);
    }
    std::vector<char> free;
    free.assign(leftSize, 1);
    for (int i = 0;i < rightSize;i++){
        if (match[i] != -1){
            free[match[i]] = false;
        }
    }
    right.assign(rightSize, 0);
    used.assign(leftSize, 0);
    for (int i = 0;i < leftSize;i++) {
        if (free[i]){
            controlVisit(i);
        }
    }
    std::pair<std::vector<int>, std::vector<int> > res;
    for (int i = 0;i < rightSize;i++){
        if (match[i] != -1){
            if (right[i]){
                res.second.push_back(i);
            }
            else{
                res.first.push_back(match[i]);
            }
        }
    }
    return res;
}



bool Algorithm::matchVisit(int v)
{
    used[v] = 1;
    for (int i = 0;i < graph[v].size();i++){
        if (match[graph[v][i]] == -1 ||
            (!used[match[graph[v][i]]] && matchVisit(match[graph[v][i]]))){
            match[graph[v][i]] = v;
            return true;
        }
    }
    return false;
}

void Algorithm::controlVisit(int v)
{
    used[v] = 1;
    for (int i = 0;i < graph[v].size();i++){
        if (!right[graph[v][i]] && !used[match[graph[v][i]]]){
            controlVisit(match[graph[v][i]]);
            right[graph[v][i]] = true;
        }
    }
}