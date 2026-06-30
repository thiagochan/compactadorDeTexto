#include "compactador.hpp"

using namespace std;

Compactador::Compactador() {}

int Compactador::mostrarMenu() {
    cout << "Digite a opcao escolhida: " << endl;
    cout << "1: Compactar por caracter " << endl;
    cout << "2: Compactar por palavra " << endl;
    cout << "3: Descompactar por caracter " << endl;
    cout << "4: Compactar por caracter " << endl;
    cout << "0: Sair do programa " << endl;
    int op; cin >> op;
    return op;
}

No* Compactador::criarArvoreHuffman(priority_queue<No*, vector<No*>, NoComp> &q, int n) {
    for(int i = 0; i < n-1; i++) {
        string token;
        No *novo = new No(token, 0, NULL, NULL);

        No *x = q.top();
        q.pop();

        No *y = q.top();
        q.pop();

        novo->token = token;
        novo->esq = x;
        novo->dir = y;
        novo->freq = x->freq + y->freq;

        q.push(novo);
    }

    return q.top();
}

void Compactador::associarCaracterComHuffman(No* atual, map<uchar, uchar> &qntBitsCaracter, map<uchar, uchar> &associacao, uchar depth, uchar bitmask) {
    if (atual==NULL) return;

    if (atual->token != "") {
        associacao[atual->token[0]] = bitmask;
        qntBitsCaracter[atual->token[0]] = depth;
        return;
    }
    else {
        associarCaracterComHuffman(atual->esq, qntBitsCaracter, associacao, depth+1, bitmask);
        associarCaracterComHuffman(atual->dir, qntBitsCaracter, associacao, depth+1, bitmask | (1<<depth));
    }
}

void Compactador::compactarPorCaracter(ifstream &FILE) {
    map<char, int> contagem;
    char buffer;
    while(FILE.get(buffer)) {
        contagem[buffer]++;
    }

    // Criar nós para os caracteres do alfabeto
    priority_queue<No*, vector<No*>, NoComp> q;
    for(auto u: contagem) {
        string token(1, u.first);
        int freq = u.second;

        No *no = new No(token, freq, NULL, NULL);
        q.push(no);
    }

    No* huffman = criarArvoreHuffman(q, contagem.size());

     map<uchar, uchar> qntBitsCaracter, associacaoCaracterHuffman;
     associarCaracterComHuffman(huffman, qntBitsCaracter, associacaoCaracterHuffman, 0, 0);
     
}

void Compactador::compactar() {
    int escolha = 7;
    while(escolha) {
        escolha = mostrarMenu();
        if (escolha==0) break;
        if (escolha>4) continue;

        cout << "Escreva o nome do arquivo: ";
        string fileName; cin >> fileName;

        if (escolha==1) {
            ifstream FILE(fileName);

            if (!FILE.is_open()) {
                cerr << "Erro ao abrir o arquivo" << endl;
                return;
            }

            compactarPorCaracter(FILE);
            
            FILE.close();
        }
    }
}