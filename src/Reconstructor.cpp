#include "Reconstructor.h"
#include <fstream>
#include <iostream>

Reconstructor::Reconstructor(Storage* storageCentral){
    this->storage = storageCentral;
}

void Reconstructor::reconstruirArquivo(const std::string& nomeOriginal, const std::string& nomeSaida) {
    std::vector<uint32_t> receita = storage->obterReceita(nomeOriginal);

    if (receita.empty()) {
        std::cerr << "Erro: Nao ha receita registrada para o arquivo " << nomeOriginal << std::endl;
        return;
    }

    std::ofstream arquivoDestino(nomeSaida);

    if (!arquivoDestino.is_open()) {
        std::cerr << "Erro: Nao foi possivel criar o arquivo de saida " << nomeSaida << std::endl;
        return;
    }

    for (uint32_t id : receita) {
        std::string blocoTexto = storage->obterTextoDoBloco(id);
        arquivoDestino << blocoTexto << "\n";
    }

    arquivoDestino.close();
}