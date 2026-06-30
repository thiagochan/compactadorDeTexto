#ifndef COMPACTADOR_H
#define COMPACTADOR_H
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <vector>
#include "no.hpp"
#define endl "\n"
#define uchar unsigned char

class Compactador {
    public:
        Compactador();
        void compactar();
        void associarCaracterComHuffman(No* atual, map<uchar, uchar> &qntBitsCaracter, map<uchar, uchar> &associacao, uchar depth, uchar bitmask);
        void compactarPorCaracter(ifstream &FILE);
    private:
        int mostrarMenu();
        No* criarArvoreHuffman(priority_queue<No*, vector<No*>, NoComp> &q, int n);
        
};

#endif