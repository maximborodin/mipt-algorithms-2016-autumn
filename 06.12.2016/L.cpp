#include <iostream>
#include <map>
#include <vector>


class SuffixAutomaton
{
public:

    SuffixAutomaton();
    void extend(char c);
    bool isWordInAutomaton(std::string& word);
private:

    class State
    {
        friend class SuffixAutomaton;
    public:
        State(int length, int suffixLink)
        {
            (*this).length = length;
            (*this).suffixLink = suffixLink;
        }
    private:
        int length, suffixLink;
        std::map<char, int> children;
    };

    std::vector<State> states;
    size_t size;
    size_t lastStateNumber;
};


SuffixAutomaton::SuffixAutomaton()
{
    states.push_back(State(0, -1));
    size = 1;
    lastStateNumber = 0;
}

void SuffixAutomaton::extend (char c)
{
    size_t newVertexNumber = size++;
    states.push_back(State(0, 0));
    states[newVertexNumber].length = states[lastStateNumber].length + 1;
    int pointer;
        for (pointer = lastStateNumber;pointer != -1 && !states[pointer].children.count(c); pointer = states[pointer].suffixLink){
            states[pointer].children[c] = newVertexNumber;
        }
        if (pointer == -1){
            states[newVertexNumber].suffixLink = 0;
    }
    else{
        int child = states[pointer].children[c];
        if (states[pointer].length + 1 == states[child].length){
            states[newVertexNumber].suffixLink = child;
        }
        else{
            int cloneNumber = size++;
            states.push_back(State(0, 0));
            states[cloneNumber].length = states[pointer].length + 1;
            states[cloneNumber].children = states[child].children;
            states[cloneNumber].suffixLink = states[child].suffixLink;
            for (; pointer != -1 && states[pointer].children[c] == child;pointer = states[pointer].suffixLink){
                states[pointer].children[c] = cloneNumber;
            }
            states[child].suffixLink = states[newVertexNumber].suffixLink = cloneNumber;
        }
    }
    lastStateNumber = newVertexNumber;
}

bool SuffixAutomaton::isWordInAutomaton (std::string& word)
{
    int vertexNumber = 0;
    for (size_t i = 0;i < word.size();++i){
        if (states[vertexNumber].children.count(word[i])){
            vertexNumber = states[vertexNumber].children[word[i]];
        }
        else{
            return false;
        }
    }
    return true;
}

int main ()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    SuffixAutomaton suffixAutomaton;
    char command;
    std::string word;
    while (std::cin >> command){
        std::cin >> word;
        for (size_t i = 0;i < word.size();++i){
            word[i] = tolower(word[i]);
        }
        if (command == '?'){
            if (suffixAutomaton.isWordInAutomaton(word)){
                printf ("YES\n");
            }
            else{
                printf ("NO\n");
            }
        }
        else{
            for (size_t i = 0;i < word.size();++i){
                suffixAutomaton.extend(word[i]);
            }
        }
    }
    return 0;
}