#ifndef NO_H
#define NO_h

#include <string>

using namespace std;

class No {
    public:
        string token;
        int freq;
        int id;
        No *esq;
        No *dir;

        No(string token, int freq, int id, No *esq, No *dir);
};

class NoComp {
    public:
        bool operator()(No* a, No* b);
};

#endif