#include "Storage.h"

Storage::Storage() {}

uint32_t Storage::adicionarBlocoSeInedito(size_t hash, const std::string& texto) {
    auto it = dicionarioGlobal.find(hash);
    
    if (it != dicionarioGlobal.end()) {
        return it->second;
    }
    
    uint32_t novoId = static_cast<uint32_t>(repositorioGlobal.size());
    
    repositorioGlobal.push_back(texto);
    dicionarioGlobal[hash] = novoId;
    
    return novoId;
}

std::string Storage::obterTextoDoBloco(uint32_t id) {
    return repositorioGlobal[id];
}

void Storage::salvarReceita(const std::string& nomeArquivo, const std::vector<uint32_t>& listaIds) {
    receitas[nomeArquivo] = listaIds;

}

std::vector<uint32_t> Storage::obterReceita(const std::string& nomeArquivo) {
    return receitas[nomeArquivo];
}