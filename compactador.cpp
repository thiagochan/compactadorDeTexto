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

void Compactador::gerarCodigos(No* raiz, string codigo, map<string, string>& codigos){
    if(raiz==NULL)return;
    
    // Se for folha atribui codigo armazenado
    if(raiz->esq==NULL && raiz->dir==NULL){
        codigos[raiz->token] = codigo;
        return;
    }

    gerarCodigos(raiz->esq,codigo+"0",codigos);
    gerarCodigos(raiz->dir,codigo+"1",codigos);

    
}

void Compactador::escreverBits(ofstream &OUT, const string &codigo, unsigned char &byte, int &bitsNoByte){
    for(char bit : codigo){
        byte <<= 1;

        if(bit == '1')
            byte |= 1;

        bitsNoByte++;

        if(bitsNoByte == 8){
            OUT.put(byte);
            byte = 0;
            bitsNoByte = 0;
        }
    }
}

void Compactador::finalizarBits(ofstream &OUT,unsigned char &byte, int &bitsNoByte){
    if(bitsNoByte == 0)
        return;

    byte <<= (8 - bitsNoByte);

    OUT.put(byte);
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
            FILE.close();
        }

        if (escolha==2) {
            ifstream FILE(fileName);

            if (!FILE.is_open()) {
                cerr << "Erro ao abrir o arquivo" << endl;
                return;
            }

            map<string, int> contagem;
            string buffer;
            char character;
        
            //conta os separadores como palavra
           while (FILE.get(character)){
                if(character!='\n' && character!='\t' && character!=' '){
                    buffer+=character;
                }else{
                    //contabiliza palavra montada
                    if(!buffer.empty()) {
                        contagem[buffer]++;
                        buffer.clear();
                    }
                    //contabiliza o separador
                    contagem[string(1, character)]++;
                }
           }
            //contabiliza ultima palavra
           if(!buffer.empty()) {
                contagem[buffer]++;
                buffer.clear();
            }


            // Criar nós para cada palavra e separador do texto
            priority_queue<No*, vector<No*>, NoComp> q;
            for(auto u: contagem) {
                string token = u.first;
                int freq = u.second;

                No *no = new No(token, freq, NULL, NULL);
                q.push(no);
            }

            No* huffman = criarArvoreHuffman(q, contagem.size());

            map<string,string>codigos;
            gerarCodigos(huffman, "", codigos);

            FILE.clear();
            FILE.seekg(0);

            // for (auto u : codigos)cout << "\"" << u.first << "\" -> " << u.second << endl;

            ofstream OUT(fileName + ".huff", ios::binary);

            if(!OUT.is_open()){
                cerr << "Erro ao criar arquivo." << endl;
                return;
            }
            unsigned char byte = 0;
            int bitsNoByte = 0;
            buffer.clear();
            while (FILE.get(character)){
                if(character!='\n' && character!='\t' && character!=' '){
                    buffer+=character;
                }else{
                    if(!buffer.empty()) {
                        escreverBits(OUT, codigos[buffer], byte, bitsNoByte);                        
                        buffer.clear();
                    }
                    escreverBits(OUT, codigos[string(1, character)], byte, bitsNoByte);   
                }
            }
            if(!buffer.empty())escreverBits(OUT, codigos[buffer], byte, bitsNoByte);
            finalizarBits(OUT, byte, bitsNoByte);

            OUT.close();

        //     ofstream OUT(fileName + ".huff");
        //     if (!OUT.is_open()) {
        //         cerr << "Erro ao criar o arquivo." << endl;
        //         return;
        //     }
        //     while (FILE.get(character)){
        //         if(character!='\n' && character!='\t' && character!=' '){
        //             buffer+=character;
        //         }else{
        //             //escreve palavra encontrada
        //             if(!buffer.empty()) {
        //                 OUT << codigos[buffer];
        //                 buffer.clear();
        //             }
        //             //escreve separador
        //             OUT << codigos[string(1, character)];
        //         }
        //    }
        //    //escreve ultima palavra
        //    if(!buffer.empty()) {
        //         OUT << codigos[buffer];
        //         buffer.clear();
        //     }
        //     OUT.close();

            FILE.close();
        }
    }
}