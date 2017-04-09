#include <iostream>
#include <vector>
#include <string>
#include <memory>


typedef long long LL;

const size_t ALPHABET = 300;

class TROIKA
{
public:
    TROIKA (long long first, long long second, long long third) :
            first(first),
            second(second),
            third(third)
    {}
    long long first;
    long long second;
    long long third;
};

class SuffixArray
{
public:
    SuffixArray(std::string const& str)
    {
        s = str + "#";
        std::vector<int> count(std::max(s.size(), ALPHABET), 0);
        std::vector<int> clearse(s.size(), 0);
        for (int i = 0; i < s.size(); ++i) {
            ++count[s[i]];
        }
        for (int i = 1; i < ALPHABET; ++i) {
            count[i] += count[i - 1];
        }
        suffixes = std::vector<int>(s.size(), 0);
        for (int i = s.size() - 1; i >= 0; --i) {
            suffixes[--count[s[i]]] = i;
        }
        clearse[suffixes[0]] = 0;
        classesCount = 1;
        for (int i = 1; i < s.size(); ++i) {
            if (s[suffixes[i]] != s[suffixes[i - 1]]) {
                ++classesCount;
            }
            clearse[suffixes[i]] = classesCount - 1;
        }

        std::vector<int> newClasses(s.size(), 0);
        std::vector<int> second(s.size(), 0);
        int tryingLength = 1;
        while (tryingLength < s.size()){
            for (int i = 0; i < s.size(); ++i) {
                second[i] = (suffixes[i] - tryingLength + s.size()) % s.size();
            }
            count.clear();
            count.resize(std::max(s.size(), ALPHABET));
            for (int i = 0; i < s.size(); ++i) {
                ++count[clearse[second[i]]];
            }
            for (int i = 1; i < classesCount; ++i) {
                count[i] += count[i - 1];
            }
            for (int i = s.size() - 1; i >= 0; --i) {
                suffixes[--count[clearse[second[i]]]] = second[i];
            }

            newClasses[suffixes[0]] = 0;
            classesCount = 1;
            for (int i = 1; i < s.size(); ++i) {
                if ((clearse[(suffixes[i] + tryingLength) % s.size()] != clearse[(suffixes[i - 1] + tryingLength) % s.size()]) ||
                        (clearse[suffixes[i]] != clearse[suffixes[i - 1]])) {
                    ++classesCount;
                }
                newClasses[suffixes[i]] = classesCount - 1;
            }
            std::copy(newClasses.begin(), newClasses.end(), clearse.begin());
            tryingLength *= 2;
        }
        calculateLCP();
    }

    void calculateLCP()
    {
        lcp = std::vector<int> (s.size());
        std::vector<int> reversedSuffixes(s.size());
        int n = (int) s.size();

        for (int i = 0; i < n; ++i) {
            reversedSuffixes[suffixes[i]] = i;
        }
        int k = 0;
        for (int i = 0; i < n; ++i) {
            if (k > 0) {
                --k;
            }
            if (reversedSuffixes[i] == n - 1) {
                k = 0;
            }
            else {
                int j = suffixes[reversedSuffixes[i] + 1];
                while (std::max(i, j) + k < n && s[i + k] == s[j + k]) {
                    ++k;
                }
                lcp[reversedSuffixes[i]] = k;
            }
        }
        for (int i = n - 1; i > 0; --i) {
            lcp[i] = lcp[i - 1];
        }
        lcp[0] = 0;
    }
    std::string s;
    std::vector<int> suffixes;
    std::vector<int> lcp;

    int classesCount;
};

class SuffixTree
{
public:
    struct Node;

    Node* root;
    size_t size;
    std::string s;

    struct Edge
    {
        int l, r;
        Node* child;

        Edge(int l, int r, Node* child) :
                l(l),
                r(r),
                child(child)
        {}
    };

    struct Node
    {
        int index;
        int depth;
        Node* parent;
        std::vector<Edge> children;

        Node(int index, int depth) : Node(index, nullptr, depth) { }

        Node(int index, Node* parent, int depth) :
                index(index), depth(depth), parent(parent), children(std::vector<Edge>()) { }

        size_t countLeafs(LL end, LL& refrain, LL& pos, LL& len) {
            size_t cnt = 0;

            if (children.size() == 0) {
                cnt = 1;
                --depth;
            }
            for (Edge& edge: children) {
                if (index != 1 || edge.l != end)
                    cnt += edge.child->countLeafs(end, refrain, pos, len);
            }

            LL cur = (LL) depth * (LL) cnt;
            if (refrain < cur) {
                refrain = cur;
                auto par = parent;
                for (int i = 0; i < par->children.size(); ++i) {
                    if (par->children[i].child == this) {
                        if (children.size() == 0) {
                            pos = par->children[i].r - depth;
                        }
                        else {
                            pos = par->children[i].r - depth + 1;
                        }
                    }
                }
                len = depth;
            }
            if (children.size() == 0) {
                ++depth;
            }
            return cnt;
        }
    };

    Node* pushSuffix(Node* previousNode, int pos, int lcp)
    {
        int n = (int) s.size();
        if (previousNode->depth == 0 || previousNode->depth == lcp) {
            Node* newNode(new Node(++size, previousNode, n - pos));
            previousNode->children.push_back(Edge(pos + lcp, n - 1, newNode));
            return newNode;
        }
        else {
            auto parent = previousNode->parent;
            if (parent->depth < lcp) {
                Node* newNode(new Node(++size, parent, lcp));

                auto edge = parent->children[parent->children.size() - 1];
                parent->children.pop_back();
                parent->children.push_back(Edge(edge.l, edge.l + (lcp - parent->depth - 1), newNode));

                newNode->children.push_back(Edge(edge.l + (lcp - parent->depth), edge.r, edge.child));
                previousNode->parent = newNode;
            }
            return pushSuffix(previousNode->parent, pos, lcp);
        }
    }

    SuffixTree(SuffixArray const& suffixArray) :
            size(1),
            s(suffixArray.s)
    {
        root = new Node (1, 0);
        Node* prev = root;
        for (size_t i = 0; i < suffixArray.lcp.size(); ++i) {
            prev = pushSuffix(prev, suffixArray.suffixes[i], suffixArray.lcp[i]);
        }
    }

    TROIKA getRefrain()
    {
        LL refrain, pos, len;
        refrain = pos = len = 0;
        root->countLeafs((LL) s.size() - 1, refrain, pos, len);
        TROIKA result = TROIKA(refrain, pos, len);
        return result;
    }

};



int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    int n, m;
    std::string str;
    std::cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        int c;
        std::cin >> c;
        str.push_back(static_cast<char>(c + 'a'));
    }
    SuffixArray suffixArray(str);
    SuffixTree suffixTree(suffixArray);

    TROIKA ans = suffixTree.getRefrain();
    std::cout << ans.first << std::endl;
    std::cout << ans.third << std::endl;
    for (int i = 0; i < ans.third; ++i) {
        std::cout << str[ans.second + i] - 'a' << " ";
    }
    std::cout << std::endl;
    return 0;
}
