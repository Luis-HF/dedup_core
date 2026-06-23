#include "Storage.h"

Storage::Storage() {}

uint32_t Storage::adicionarBlocoSeInedito(size_t hash, const std::string& texto) {
    // Busca a assinatura numérica (hash) no dicionário global para verificar duplicidade em tempo O(1)
    auto it = dicionarioGlobal.find(hash);
    
    // Se o iterador não chegou ao fim do mapa, significa que o hash já existe; retorna o ID associado
    if (it != dicionarioGlobal.end()) {
        return it->second;
    }
    
    // Caso seja inédito, o novo ID será deterministicamente o tamanho atual do vetor (índice sequencial)
    uint32_t novoId = static_cast<uint32_t>(repositorioGlobal.size());
    
    // Insere o texto real da linha no fim do vetor do repositório global
    repositorioGlobal.push_back(texto);
    
    // Registra a nova relação de indexação entre o hash (chave) e o ID criado (valor) no dicionário global
    dicionarioGlobal[hash] = novoId;
    
    return novoId;
}

std::string Storage::obterTextoDoBloco(uint32_t id) {
    // Acesso direto via índice de vetor em tempo constante O(1) para recuperar a string correspondente ao ID
    return repositorioGlobal[id];
}

void Storage::salvarReceita(const std::string& nomeArquivo, const std::vector<uint32_t>& listaIds) {
    // Mapeia o nome do arquivo original (chave) ao seu vetor de IDs sequenciais (valor) na tabela hash de receitas
    receitas[nomeArquivo] = listaIds;
}

std::vector<uint32_t> Storage::obterReceita(const std::string& nomeArquivo) {
    // Retorna o vetor contendo a sequência de IDs vinculada ao nome do arquivo consultado no mapa de receitas
    return receitas[nomeArquivo];
}