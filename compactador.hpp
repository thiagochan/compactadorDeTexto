#ifndef COMPACTADOR_H
#define COMPACTADOR_H
#include <iostream>
#include <chrono>
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
        
        void gerarCodigos(No* raiz, string codigo, map<string,string>& codigos);
        void escreverBits(ofstream &OUT, string &codigo, unsigned char &byte, int &bitsNoByte);
        void finalizarBits(ofstream &OUT, unsigned char &byte, int &bitsNoByte);
        void escreverCabecalho(ofstream &OUT, map<string, int> &contagem);
        void lerCabecalho(ifstream &FILE, map<string, int> &contagem);
        void compactarPorPalavra(ifstream &FILE);
        void descompactarPorPalavra(ifstream &FILE);
        private:
        int mostrarMenu();
        No* criarArvoreHuffman(priority_queue<No*, vector<No*>, NoComp> &q, int n, int nextId);
        
};

#endif