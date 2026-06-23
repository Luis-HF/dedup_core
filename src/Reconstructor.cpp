#include "Reconstructor.h"
#include <fstream>
#include <iostream>

Reconstructor::Reconstructor(Storage* storageCentral){
    this->storage = storageCentral; // Injeção de dependência para permitir o acesso às estruturas de dados centrais
}

void Reconstructor::reconstruirArquivo(const std::string& nomeOriginal, const std::string& nomeSaida) {
    // Consulta o mapa de receitas no Storage para recuperar o vetor de IDs associado ao nome do arquivo original
    std::vector<uint32_t> receita = storage->obterReceita(nomeOriginal);

    // Verifica se o vetor retornado está vazio, o que sinaliza que o arquivo nunca foi ingerido no sistema
    if (receita.empty()) {
        std::cerr << "Erro: Nao ha receita registrada para o arquivo " << nomeOriginal << std::endl;
        return;
    }

    std::ofstream arquivoDestino(nomeSaida); // Abre o fluxo de gravação para criar o arquivo de texto final reconstruído

    if (!arquivoDestino.is_open()) {
        std::cerr << "Erro: Nao foi possivel criar o arquivo de saida " << nomeSaida << std::endl;
        return;
    }

    // Varre sequencialmente o vetor de IDs contido na receita do documento
    for (size_t i = 0; i < receita.size(); ++i) {
        // Recupera o texto real da linha fazendo uma busca por índice direto O(1) no repositório global
        std::string blocoTexto = storage->obterTextoDoBloco(receita[i]);
        arquivoDestino << blocoTexto; // Despeja os caracteres da linha no arquivo físico de saída
        
        // Insere a quebra de linha apenas se não for o último elemento do vetor, mantendo a integridade do arquivo original
        if (i < receita.size() - 1) {
            arquivoDestino << "\n";
        }
    }

    arquivoDestino.close(); // Fecha o arquivo de destino garantindo a gravação total dos buffers no disco
}