#include <iostream>
#include <vector>
#include <cstring>

const int ALPHABET = 2;
const int NMAX = 30000;

struct Vertex
{
    Vertex()
    {
        parent = -1;
        pch = -1;
        link = -1;
        isUsed = 2;
        isBad = 0;
        isChecked = 0;
        for (int i = 0;i < ALPHABET;++i){
            children[i] = -1;
            go[i] = -1;
        }
    }

    int children[ALPHABET];
    int go[ALPHABET];

    char pch;

    int parent;
    int link;
    int isUsed;

    bool isBad;
    bool isChecked;
};

Vertex t[NMAX + 1];
int bohrSize = 0;

void init()
{
    t[0].parent = t[0].link = -1;
    t[0].isBad = 0;
    t[0].isChecked = 1;
    t[0].isUsed = 2;
    memset (t[0].children, 255, sizeof t[0].children);
    memset (t[0].go, 255, sizeof t[0].go);
    bohrSize = 1;
}

void addString (const std::string& str)
{
    int v = 0;
    for (size_t i = 0; i < str.length(); ++i) {
        char c = str[i] - '0';
        if (t[v].children[c] == -1) {
            memset (t[bohrSize].children, 255, sizeof t[bohrSize].children);
            memset (t[bohrSize].go, 255, sizeof t[bohrSize].go);
            t[v].isUsed = 2;
            t[bohrSize].link = -1;
            t[bohrSize].parent = v;
            t[bohrSize].pch = c;
            t[v].children[c] = bohrSize++;
        }
        v = t[v].children[c];
    }
    t[v].isChecked = 1;
    t[v].isBad = 1;
}

int go (int v, char c);

int getLink (int v)
{
    if (t[v].link == -1) {
        if (v == 0 || t[v].parent == 0) {
            t[v].link = 0;
        } else {
            t[v].link = go(getLink(t[v].parent), t[v].pch);
        }
    }
    return t[v].link;
}

int go (int v, char c) {
    if (t[v].go[c] == -1) {
        if (t[v].children[c] != -1) {
            t[v].go[c] = t[v].children[c];
        } else {
            t[v].go[c] = v == 0 ? 0 : go(getLink(v), c);
        }
    }
    return t[v].go[c];
}

bool getIsBad (int v)
{
    if (!t[v].isChecked){
        t[v].isBad = getIsBad(t[v].link);
        t[v].isChecked = 1;
    }
    return t[v].isBad;
}

int dfs (int v)
{
    if (t[v].isUsed == 2){
        if (t[v].isBad){
            t[v].isUsed = 0;
        }
        else{
            t[v].isUsed = 1;
            t[v].isUsed = (dfs(go (v, 0))) || (dfs(go(v, 1)));
        }
    }
    return t[v].isUsed;
}

int main ()
{
    init();
    int n;
    std::cin >> n;
    for (int i = 0;i < n;++i){
        std::string str;
        std::cin >> str;
        addString(str);
    }

    for (int i = 0;i < bohrSize;++i){
        t[i].link = getLink(i);
    }
    for (int i = 0;i < bohrSize;++i){
        getIsBad(i);
    }
    for (int i = 0;i < bohrSize;++i){
        for (int j = 0;j < ALPHABET;++j){
            t[i].go[j] = go(i, j);
        }
    }

    if (dfs(0) == 1){
        std::cout << "TAK" << std::endl;
    }
    else{
        std::cout << "NIE" << std::endl;
    }
}