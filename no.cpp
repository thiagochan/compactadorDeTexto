#include "no.hpp"

No::No(string token, int freq, int id, No* esq, No* dir) {
    this->token = token;
    this->freq = freq;
    this->id = id;
    this->esq = esq;
    this->dir = dir;
}

bool NoComp::operator()(No* a, No* b) {
    if(a->freq != b->freq) return a->freq > b->freq;

    return a->id > b->id;
}