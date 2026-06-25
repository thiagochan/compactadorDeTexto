#include "no.hpp"

No::No(string token, int freq, No* esq, No* dir) {
    this->token = token;
    this->freq = freq;
    this->esq = esq;
    this->dir = dir;
}

bool NoComp::operator()(No* a, No* b) {
    return a->freq > b->freq;
}