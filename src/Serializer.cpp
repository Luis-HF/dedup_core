#include "Serializer.h"
#include <fstream>

Serializer::Serializer(Storage* storageCentral){
    this->storage = storageCentral;
}

void Serializer::salvarParaDisco(const std::string& caminhoArquivoDat) {
    std::ofstream arquivoOut(caminhoArquivoDat, std::ios::binary);

    if (!arquivoOut.is_open()) {
        return; 
    }

    size_t qtdStrings = storage->repositorioGlobal.size();
    arquivoOut.write(reinterpret_cast<const char*>(&qtdStrings), sizeof(qtdStrings));

    for (const std::string& texto : storage->repositorioGlobal) {
        size_t tamanhoString = texto.size();
        
        arquivoOut.write(reinterpret_cast<const char*>(&tamanhoString), sizeof(tamanhoString));
        arquivoOut.write(texto.data(), tamanhoString);
    }

    
    size_t qtdReceitas = storage->receitas.size();
    arquivoOut.write(reinterpret_cast<const char*>(&qtdReceitas), sizeof(qtdReceitas));

    for (const auto& par : storage->receitas) {
        const std::string& nomeArquivo = par.first;
        const std::vector<uint32_t>& listaIds = par.second;

        size_t tamanhoNome = nomeArquivo.size();
        arquivoOut.write(reinterpret_cast<const char*>(&tamanhoNome), sizeof(tamanhoNome));
        arquivoOut.write(nomeArquivo.data(), tamanhoNome);

        size_t qtdIds = listaIds.size();
        arquivoOut.write(reinterpret_cast<const char*>(&qtdIds), sizeof(qtdIds));
        
        if (qtdIds > 0) {
            arquivoOut.write(reinterpret_cast<const char*>(listaIds.data()), qtdIds * sizeof(uint32_t));
        }
    }

    arquivoOut.close();
}


void Serializer::carregarDoDisco(const std::string& caminhoArquivoDat){}