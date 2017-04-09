#include <iostream>
#include <vector>
#include <cstring>

const int ALPHABET = 26;
const int NMAX = 200500;

typedef long long LL;

struct Vertex
{
    Vertex()
    {
        parent = -1;
        pch = 0;
        link = -1;
        sum = 0;
        countOfEnds = 0;

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
    LL sum;
    LL countOfEnds;

    bool isBad;
    bool isChecked;
};

Vertex t[NMAX + 1];
int bohrSize = 0;

void init()
{
    t[0].isBad = 1;
    t[0].isChecked = 1;
    memset (t[0].children, 255, sizeof t[0].children);
    memset (t[0].go, 255, sizeof t[0].go);
    bohrSize = 1;
}

void addString (const std::string& str)
{
    int v = 0;
    for (size_t i = 0; i < str.length(); ++i) {
        char c = str[i] - 'a';
        if (t[v].children[c] == -1) {
            memset (t[bohrSize].children, 255, sizeof t[bohrSize].children);
            memset (t[bohrSize].go, 255, sizeof t[bohrSize].go);
            t[bohrSize].parent = v;
            t[bohrSize].pch = c;
            t[v].children[c] = bohrSize++;
        }
        v = t[v].children[c];
    }
    ++t[v].countOfEnds;
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

LL getSum (int v)
{
    if (!t[v].isChecked){
        t[v].isChecked = 1;
        t[v].sum = t[v].countOfEnds + getSum(getLink(v));
    }
    return t[v].sum;
}

LL answer = 0;

void solve ()
{
    for (int i = 0;i < bohrSize;++i){
        if (t[i].isBad){
            answer += getSum(i);
        }
    }
}

int main ()
{
    init();
    int n;
    std::cin >> n;
    for (int i = 1;i < n;++i){
        t[i].isBad = 1;
        ++bohrSize;
    }
    for (int i = 0;i < n;++i){
        int countOfChildren;
        std::cin >> countOfChildren;
        for (int j = 0;j < countOfChildren;++j) {
            int vertexNumber;
            char vertexCharacter;
            std::cin >> vertexNumber >> vertexCharacter;
            t[i].children[vertexCharacter - 'a'] = vertexNumber - 1;
            t[vertexNumber - 1].parent = i;
            t[vertexNumber - 1].pch = vertexCharacter - 'a';
        }
    }
    int m;
    std::cin >> m;
    for (int i = 0;i < m;++i){
        std::string str;
        std::cin >> str;
        addString(str);
    }
    solve();
    std::cout << answer << std::endl;
    return 0;
}