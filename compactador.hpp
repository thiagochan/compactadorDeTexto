#ifndef COMPACTADOR_H
#define COMPACTADOR_H
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <vector>
#include "no.hpp"
#define endl "\n"

class Compactador {
    public:
        Compactador();
        void compactar();
        void compactarPorCaracter(ifstream &FILE);
    private:
        int mostrarMenu();
        No* criarArvoreHuffman(priority_queue<No*, vector<No*>, NoComp> &q, int n);
        
};

#endif