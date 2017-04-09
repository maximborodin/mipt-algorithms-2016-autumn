#include <iostream>
#include <vector>
#include <algorithm>

const size_t MAXSIZE = 10000;

std::vector<int> prefixFunc (const std::string& str)
{
    int n = str.size();
    std::vector<int> pi(n);
    for (int i = 1;i < n;++i){
        int j = pi[i - 1];
        while (j > 0 && str[i] != str[j]){
            j = pi[j - 1];
        }
        if (str[i] == str[j]){
            ++j;
        }
        pi[i] = j;
    }
    return pi;
}

//std::vector<int> zArr;
int zArr[MAXSIZE * 2 + 5];


void zFunc(size_t move, const std::string& str)
{
    size_t size = str.size() - move;
    int left = 0;
    int right = 0;
    zArr[0] = size;
    for (int i = 1;i < size;++i) {
        zArr[i] = std::max(0, std::min(right - i, zArr[i - left]));
        while (i + zArr[i] < size && str[zArr[i] + move] == str[i + zArr[i] + move]) {
            ++zArr[i];
        }
        if (i + zArr[i] >= right) {
            left = i;
            right = i + zArr[i];
        }
    }
}



int main ()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::string str;
    std::cin >> str;
    str = str + "#" + str;
    //zArr.resize(str.size());
    size_t size = str.size() / 2;
    std::vector<std::vector<int> > dp (size);
    size_t result = size;
    size_t luxuryIndex = 0;
    size_t luxuryLength = 0;
    for (size_t i = 0;i < size;++i){
        dp[i].assign(size - i + 1, 1);
        int index = i - 1;
        zFunc(i, str);
        size_t lim = std::min(i, size - i);
        for (size_t length = 1;length <= lim;++length){
            if (index >= 0){
                index = std::min(i - length, (size_t)index);
                while (index >= 0){
                    if (zArr[size + 1 + index - i] >= length){
                        break;
                    }
                    --index;
                }
                if (index < 0){
                    dp[i][length] = 1;
                }
                else{
                    dp[i][length] = dp[index][length] + 1;
                }
                if (size + length + dp[i][length] - length * dp[i][length] < result){
                    result = size + length + dp[i][length] - length * dp[i][length];
                    luxuryLength = length;
                    luxuryIndex = i;
                }
            }
        }
    }
    for (size_t i = luxuryIndex; i < luxuryIndex + luxuryLength; ++i){
        //std::cout << str[i];
        printf ("%c", str[i]);
    }
    printf("\n");
    if (luxuryLength > 0){
        zFunc(luxuryIndex, str);
        for (size_t i = size + 1 - luxuryIndex; i < 2 * size + 1 - luxuryIndex;++i){
            if (zArr[i] >= luxuryLength){
                printf ("A");
                i += 2 + luxuryLength - 3;
            }
            else{
            	//std::cout << str[i + luxuryIndex - size - 1];
                printf ("%c", str[i + luxuryIndex - size - 1]);
            }
        }
    }
    else{
        for (size_t i = 0;i < size;++i){
       		//std::cout << str[i];
            printf ("%c", str[i]);
        }
    }
    //std::cout << "\n";
    printf ("\n");
    return 0;
}