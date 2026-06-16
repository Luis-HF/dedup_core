#include "Serializer.h"
#include "Storage.h"
#include <fstream>
#include <vector>
#include <string>
#include <functional>

Serializer::Serializer(Storage* storageCentral){
    this->storage = storageCentral;
}

void Serializer::salvarParaDisco(const std::string& caminhoArquivoDat) {
    //abertura do arquivo
    std::ofstream arquivoOut(caminhoArquivoDat, std::ios::binary);

    if (!arquivoOut.is_open()) {
        return; 
    }

    //leitura do primeiro size_t que indica quantas strings foram gravadas anteriormente
    size_t qtdStrings = storage->repositorioGlobal.size();
    arquivoOut.write(reinterpret_cast<const char*>(&qtdStrings), sizeof(qtdStrings)); //escreve a quantidade de strings do repositorioGlobal no arquivo

    for (const std::string& texto : storage->repositorioGlobal) { //para cada string lida (baseado no inteiro que lemos antes)
        size_t tamanhoString = texto.size(); //obtemos o tamanho da string
        arquivoOut.write(reinterpret_cast<const char*>(&tamanhoString), sizeof(tamanhoString)); //escreve o tamanho da string no arquivo
        arquivoOut.write(texto.data(), tamanhoString); //escreve a string em si, escrevendo exatamente os bytes equivalentes
    }

    //partindo agora para o armazenamento das receitas
    size_t qtdReceitas = storage->receitas.size(); //quantidade de receitas obtidas
    arquivoOut.write(reinterpret_cast<const char*>(&qtdReceitas), sizeof(qtdReceitas)); //leitura desse inteiro no arquivo

    for (const auto& par : storage->receitas) { //para cada par chave-valor da tabela hash de receitas
        const std::string& nomeArquivo = par.first; //guardamos a chave
        const std::vector<uint32_t>& listaIds = par.second; //guardamos o valor

        size_t tamanhoNome = nomeArquivo.size(); //obtemos o tamanho da chave, que é um string
        arquivoOut.write(reinterpret_cast<const char*>(&tamanhoNome), sizeof(tamanhoNome)); //escrevemos o tamanho da chave no arquivo
        arquivoOut.write(nomeArquivo.data(), tamanhoNome); //escrevemos a chave em si, escrevendo exatamente os bytes equivalentes

        //em seguida, escrevemos o valor, que é um vetor de uint32_t
        size_t qtdIds = listaIds.size();
        arquivoOut.write(reinterpret_cast<const char*>(&qtdIds), sizeof(qtdIds));
        
        if (qtdIds > 0) {
            arquivoOut.write(reinterpret_cast<const char*>(listaIds.data()), qtdIds * sizeof(uint32_t)); //o tipo de dado (uint32_t) tem tamanho fixo e por isso permite escrita direta
        }
    }

    arquivoOut.close();
}

void Serializer::carregarDoDisco(const std::string& caminhoArquivoDat) {
    //abrir arquivo para leitura
    std::ifstream arquivoIn(caminhoArquivoDat, std::ios::binary);

    if (!arquivoIn.is_open()) {
        return;
    }

    //limpar Storage e iniciar classe que gera hash
    storage->repositorioGlobal.clear();
    storage->dicionarioGlobal.clear();
    storage->receitas.clear();

    std::hash<std::string> geradorHash;

    //começa leitura
    //primeiro o repositorioGlobal
    //começamos lendo o primeiro inteiro de 64 bits ou size_t que indica quantas strings existem no repositorioGlobal
    size_t qtdStrings = 0; 
    arquivoIn.read(reinterpret_cast<char*>(&qtdStrings), sizeof(qtdStrings));

    //para cada string, lemos o tamanho da string (size_t) e depois a string em si
    //primeiro garantimos que o loop vai rodar somente o número de vezes equivalente ao número de strings
    for (size_t i = 0; i < qtdStrings; ++i) {
        //lemos o size_t que indica o tamanho da string
        size_t tamanhoString = 0;
        arquivoIn.read(reinterpret_cast<char*>(&tamanhoString), sizeof(tamanhoString));

        std::string textoBloco(tamanhoString, '\0'); //c++ exige alocar memória antes de escrever, então criamos uma string do tamanho que acabamos de ler, preenchida com caracteres nulos
        arquivoIn.read(&textoBloco[0], tamanhoString); //leitura do arquivo e direcionamento para a string que acabamos de criar

        storage->repositorioGlobal.push_back(textoBloco); //inserimos a string lida no repositorioGlobal

        size_t hashDaLinha = geradorHash(textoBloco); //geramos o hash da string atual
        uint32_t idCorrespondente = static_cast<uint32_t>(i); //o id/valor do dicionarioGlobal é o valor de i atual convertido para uint32_t
        storage->dicionarioGlobal[hashDaLinha] = idCorrespondente; //inserção desses dois dados no dicionarioGlobal
    }

    //depois do repositorioGlobal, lemos as receitas
    //começamos lendo o inteiro de 64 bits ou size_t que indica quantas receitas existem no arquivo
    size_t qtdReceitas = 0;
    arquivoIn.read(reinterpret_cast<char*>(&qtdReceitas), sizeof(qtdReceitas));

    for (size_t i = 0; i < qtdReceitas; ++i) { //para cada receita

        size_t tamanhoNome = 0;//a chave é uma string, então primeiro lemos o size_t que indica o tamanho dessa string
        arquivoIn.read(reinterpret_cast<char*>(&tamanhoNome), sizeof(tamanhoNome)); //agora lemos a string em si, alocando memória para ela antes de ler

        std::string nomeArquivo(tamanhoNome, '\0'); //inicializamos a string com o tamanho lido, preenchida com caracteres nulos
        arquivoIn.read(&nomeArquivo[0], tamanhoNome); //leitura do nome da receita e direcionamento para a string que acabamos de criar

        size_t qtdIds = 0; //o valor dessa tabela hash é um vetor de uint32_t, então primeiro lemos o size_t que indica quantos ids existem nesse vetor
        arquivoIn.read(reinterpret_cast<char*>(&qtdIds), sizeof(qtdIds)); //agora a quantidade de Ids que essa chave da 

        std::vector<uint32_t> listaIds(qtdIds); //inicializamos o vetor que servirá como valor da tabela hash de receitas
        if (qtdIds > 0) {
            arquivoIn.read(reinterpret_cast<char*>(listaIds.data()), qtdIds * sizeof(uint32_t)); //já que é um vetor de uint32_t, é possível ler diretamente para a memória, despejando um grande bloco de uma vez
        }

        storage->receitas[nomeArquivo] = listaIds; //inserção do par chave-valor para a tabela hash de receitas
    }

    arquivoIn.close();
}