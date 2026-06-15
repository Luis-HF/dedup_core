#include "Deduplicator.h"
#include <fstream>
#include <iostream>
#include <functional>

Deduplicator::Deduplicator(Storage* storageCentral) {
    this->storage = storageCentral;
}

void Deduplicator::ingerirArquivos(const std::vector<std::string>& listaNomesArquivos) {
    std::hash<std::string> geradorHash;

    for (const std::string& nomeArquivo : listaNomesArquivos) {
        std::ifstream arquivoFisico(nomeArquivo);
        if (!arquivoFisico.is_open()) {
            std::cerr << "Erro: Nao foi possivel abrir o arquivo " << nomeArquivo << std::endl;
            continue;
        }

        std::vector<uint32_t> receitaDoArquivo;
        std::string linhaAtual;

        while (std::getline(arquivoFisico, linhaAtual)) {
            size_t hashDaLinha = geradorHash(linhaAtual);
            uint32_t idDoBloco = storage->adicionarBlocoSeInedito(hashDaLinha, linhaAtual);
            receitaDoArquivo.push_back(idDoBloco);
        }

        storage->salvarReceita(nomeArquivo, receitaDoArquivo);

        arquivoFisico.close();
    }
}