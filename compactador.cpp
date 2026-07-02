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

        for(uchar i = 0; i < bitsCaracter; i++) {
            // verifica se tem espaco no writebuffer, se tem, escreve
            if (writeBufferIdx<8) {
                if (codigoHuffman & (1<<i)) {
                    writeBuffer |= (1<<i);
                }

                writeBufferIdx++;
            }
            else { // se não tiver espaço no writebuffer, escreve no arquivo e reseta ele
                compactado.write(reinterpret_cast<const char*>(&writeBuffer), sizeof(writeBuffer));
                writeBuffer = 0;
                writeBufferIdx = 0;
            }
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

    // lê o map
    ofstream teste("teste1.txt");
    for(int i = 0; i < n; i++) {
        arquivo.read(reinterpret_cast<char*>(&c), sizeof(c));
        arquivo.read(reinterpret_cast<char*>(&freq), sizeof(freq));
        teste << c << " " << freq << endl;
    }

    teste.close();

    // cria a priority queue

    // recria arvore

    // percorre a arvore a partir dos bits da compressão
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