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
        void iniciar();
        void associarCaracterComHuffman(No* atual, map<uchar, uchar> &qntBitsCaracter, map<uchar, int> &associacao, uchar depth, int bitmask);
        void escreverArquivoCompactado(ifstream &original, map<uchar, uchar> &qntBitsCaracter, map<uchar, int> &associacao, map<char,int> &contagem);
        void descompactarPorCaracter(ifstream &arquivo);
        void compactarPorCaracter(ifstream &FILE);
    private:
        int mostrarMenu();
        No* criarArvoreHuffman(priority_queue<No*, vector<No*>, NoComp> &q, int n, int nextId);
        
};

#endif