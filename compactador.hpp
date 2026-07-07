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
        void gerarCodigos(No* raiz, string codigo, map<string,string>& codigos);
        void escreverBits(ofstream &OUT, const string &codigo, unsigned char &byte, int &bitsNoByte);

void finalizarBits(ofstream &OUT, unsigned char &byte, int &bitsNoByte);
    private:
        int mostrarMenu();
        No* criarArvoreHuffman(priority_queue<No*, vector<No*>, NoComp> &q, int n);
        
};

#endif