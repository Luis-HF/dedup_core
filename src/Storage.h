#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

class Storage {
private:
    std::unordered_map<size_t, uint32_t> dicionarioGlobal;
    std::vector<std::string> repositorioGlobal;
    std::unordered_map<std::string, std::vector<uint32_t>> dicionarioGlobal;

public:
    Storage();

    uint32_t adicionarBlocoSeInedito(size_t hash, const std::string& texto);
    std::string obterTextoDoBloco(uint32_t id);
    void salvarReceita(const std::string& nomeArquivo, const std::vector<uint32_t>& listaIds);
    std::vector<uint32_t> obterReceita(const std::string& nomeArquivo);
};

#endif