#include "compactador.hpp"

using namespace std;

Compactador::Compactador() {}

int Compactador::mostrarMenu() {
    cout << "Digite a opcao escolhida: " << endl;
    cout << "1: Compactar por caracter " << endl;
    cout << "2: Compactar por palavra " << endl;
    cout << "3: Descompactar por caracter " << endl;
    cout << "4: Descompactar por palavra " << endl;
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

void Compactador::escreverArquivoCompactado(ifstream &original, map<uchar, uchar> &qntBitsCaracter, map<uchar, uchar> &associacao, map<char,int> &contagem) {
    ofstream compactado("compactado.bin", ios::binary);
    original.clear();
    original.seekg(0);

    // quantidade de key, values tem o map contendo as frequencias dos caracteres
    int n = contagem.size();
    compactado.write(reinterpret_cast<const char*>(&n), sizeof(n));

    // insere os registros do map
    for (auto u : contagem) {
        char caracter = u.first; int freq = u.second;

        compactado.write(reinterpret_cast<const char*>(&caracter), sizeof(caracter));
        compactado.write(reinterpret_cast<const char*>(&freq), sizeof(freq));
    }

    // percorre arquivo
    char readBuffer, writeBuffer = 0;
    int writeBufferIdx = 0;
    while(original.get(readBuffer)) {
        uchar bitsCaracter = qntBitsCaracter[readBuffer];
        uchar codigoHuffman = associacao[readBuffer];

        for(int i = 0; i < bitsCaracter; i++) {
            // verifica se está cheio o write buffer e limpa se sim
            if (writeBufferIdx==8) {
                compactado.write(reinterpret_cast<const char*>(&writeBuffer), sizeof(writeBuffer));
                writeBuffer = 0;
                writeBufferIdx = 0;
            }

            if (codigoHuffman & (1<<i)) {
                writeBuffer |= (1<<writeBufferIdx);
            }

            writeBufferIdx++;
        }
    }

    // se restar algo no buffer, escreve o restante
    if (writeBufferIdx>0) compactado.write(reinterpret_cast<const char*>(&writeBuffer), sizeof(writeBuffer));
    compactado.close();
}

void Compactador::compactarPorCaracter(ifstream &FILE) {
    map<char, int> contagem;
    char buffer;
    while(FILE.get(buffer)) {
        contagem[buffer]++;
    }
    /*
    00000110011001100110011001100
    */

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

    escreverArquivoCompactado(FILE, qntBitsCaracter, associacaoCaracterHuffman, contagem);
}

void Compactador::descompactarPorCaracter(ifstream &arquivo) {
    int n, freq;
    char c;

    // le quantidade de entradas <caracter, freq>
    arquivo.read(reinterpret_cast<char*>(&n), sizeof(n));

    map<char, int> contagem;

    // lê o map
    for(int i = 0; i < n; i++) {
        arquivo.read(reinterpret_cast<char*>(&c), sizeof(c));
        arquivo.read(reinterpret_cast<char*>(&freq), sizeof(freq));

        contagem[c] = freq;
    }

    // cria a priority queue
    int total = 0;
    priority_queue<No*, vector<No*>, NoComp> q;
    for(auto u: contagem) {
        string token(1, u.first);
        total += u.second;
        No* novo = new No(token, u.second, NULL, NULL);

        q.push(novo);
    }

    // recria arvore
    No* huffman = criarArvoreHuffman(q, contagem.size());

    // percorre a arvore a partir dos bits da compressão
    ofstream saida("descomprimido.txt");

    int caracteres=0;
    No* atual = huffman;
    char buffer;
    while(caracteres<total) {
        arquivo.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));

        for(int i = 0; i < 8; i++) {
            if (atual->token != "") {
                caracteres++;
                saida << atual->token;
                atual = huffman;
                if (caracteres==total) break;
            }

            if (buffer & (1<<i)){
                atual = atual->dir;
            } 
            else atual = atual->esq;
        }
    }
}

void Compactador::iniciar() {
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
        else if (escolha==3) {
            ifstream FILE(fileName, ios::binary);

            descompactarPorCaracter(FILE);

            FILE.close();
        }
    }
}