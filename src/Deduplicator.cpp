#include "Deduplicator.h"
#include <fstream>
#include <iostream>
#include <functional>

Deduplicator::Deduplicator(Storage* storageCentral) {
    this->storage = storageCentral; // Injeção de dependência do Storage centralizado
}

size_t Deduplicator::ingerirArquivos(const std::vector<std::string>& listaNomesArquivos) {
    std::hash<std::string> geradorHash; // Instancia o functor padrão do C++ para geração de hashes de 64 bits (size_t)
    size_t totalLinhas = 0; // Acumulador para a estatística do total de linhas lidas no terminal

    // Navega pelo vetor contendo a lista de caminhos de arquivos de entrada passados por parâmetro
    for (const std::string& nomeArquivo : listaNomesArquivos) {
        std::ifstream arquivoFisico(nomeArquivo); // Abre o fluxo de leitura do arquivo texto atual
        if (!arquivoFisico.is_open()) {
            std::cerr << "Erro: Nao foi possivel abrir o arquivo " << nomeArquivo << std::endl;
            continue; // Ignora o arquivo atual caso ocorra falha de leitura e passa para o próximo no vetor
        }

        std::vector<uint32_t> receitaDoArquivo; // Cria o vetor temporário que armazenará ordenadamente a receita deste arquivo
        std::string linhaAtual;

        // Executa a leitura linha por linha de forma dinâmica até atingir o fim do arquivo (EOF)
        while (std::getline(arquivoFisico, linhaAtual)) {
            size_t hashDaLinha = geradorHash(linhaAtual); // Gera a assinatura digital baseada nos caracteres da string lida
            
            // Interage com o Storage para obter o ID da linha (reaproveita o ID se já existir ou cria um se for inédito)
            uint32_t idDoBloco = storage->adicionarBlocoSeInedito(hashDaLinha, linhaAtual);
            
            // Insere o ID retornado no fim do vetor de receitas do arquivo atual
            receitaDoArquivo.push_back(idDoBloco);
            totalLinhas++; // Incrementa o contador de linhas processadas
        }

        // Transfere o vetor de IDs estruturado para o repositório definitivo dentro do Storage indexado pelo nome do arquivo
        storage->salvarReceita(nomeArquivo, receitaDoArquivo);

        arquivoFisico.close(); // Fecha o arquivo físico atual para liberar os descritores de arquivos no SO
    }
    return totalLinhas; // Retorna a contagem total para que a interface CLI exiba os dados de compressão
}