#include <cstring>
#include <iostream>
#include <vector>


int n, m, k;
int someNum;
int l = 0;
int r = 0;
int size = 0;

std::vector<int> used;
std::vector<int> pa;
std::vector<int> pb;
std::vector<int> a;
std::vector<int> b;
std::vector<std::vector<int> > c;
std::vector<std::vector<int> > rc;


int dfs( int a )
{
    used[a] = someNum;
    for (int i = 0; i < rc[a].size(); ++i) {
        int b = rc[a][i];
        if (pa[b] == -1 || (used[pa[b]] != someNum && dfs(pa[b]))) {
            pa[b] = a;
            pb[a] = b;
            return 1;
        }
    }
    return 0;
}

int findChange(int b)
{
    used[b] = someNum;
    for(int i = 0; i < c[b].size(); ++i) {
        int a = c[b][i];
        if (l < a && a < r) {
            if (pb[a] == -1 || (used[pb[a]] != someNum && findChange(pb[a]))) {
                pa[b] = a;
                pb[a] = b;
                return 1;
            }
        }
    }
    return 0;
}

void add() {
    ++someNum;
    while (r < n && size < m) {
        if (dfs(r++) == 1) {
            ++size;
            ++someNum;
        }
    }
}

int main()
{
    std::cin >> n >> m >> k;

    used.resize(n);
    pb.resize(n);
    rc.resize(n);

    pa.resize(m);
    c.resize(m);

    a.resize(k);
    b.resize(k);
    for (int i = 0;i < k;++i){
        std::cin >> a[i] >> b[i];
        --a[i];
        --b[i];
        c[b[i]].push_back(a[i]);
        rc[a[i]].push_back(b[i]);
    }

    int ans = 0;
    std::fill(pa.begin(), pa.end(), -1);
    std::fill(pb.begin(), pb.end(), -1);
    add();
    for (; size == m && l < n; ++l) {
        ans += n - r + 1;
        if (pb[l] != -1) {
            pa[pb[l]] = -1;
            ++someNum;
            if (!findChange(pb[l])) {
                --size;
                add();
            }
        }
    }
    std::cout << ans << std::endl;
    return 0;
}