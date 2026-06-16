#include "CommandLineInterface.h"
#include <iostream>
#include <fstream>


CommandLineInterface::CommandLineInterface(Storage* st, Deduplicator* dedup, Reconstructor* recon, Serializer* seri){
    this->storage = st;
    this->deduplicator = dedup;
    this->reconstructor = recon;
    this->serializer = seri;
}


void CommandLineInterface::exibirAjuda() const {
    std::cout << "Uso Incorreto. Comandos validos:\n"
              << "  ./dedup ingerir <arquivo1.txt> <arquivo2.txt> ...\n"
              << "  ./dedup reconstruir <nome_do_arquivo_original> <caminho_arquivo_saida.txt>\n";
}

void CommandLineInterface::processarArgumentos(int argc, char* argv[]) {
    //valida mínimo de argumentos
    if (argc < 3) {
        exibirAjuda();
        return;
    }

    std::string comando = argv[1]; //guarda o modo
    const std::string ARQUIVO_BIN = "storage.dat"; //nome do arquivo binário para persistência do storage

    // =========================================================================
    // MODO: INGESTÃO
    // =========================================================================
    if (comando == "ingerir") {
        serializer->carregarDoDisco(ARQUIVO_BIN); //chama o serializer

        std::vector<std::string> listaArquivos; //inicializa o vetor de arquivos a serem processados
        size_t bytesOriginaisTotais = 0; //variável para acumular o total de bytes dos arquivos originais

        // Itera sobre todos os arquivos passados no terminal a partir do índice 2, pois 0 é o executável e 1 é o comando/modo
        for (int i = 2; i < argc; ++i) {
            std::string caminhoArquivo = argv[i];
            listaArquivos.push_back(caminhoArquivo); //adiciona esse arquivo na lista

            std::ifstream inTexto(caminhoArquivo, std::ios::binary | std::ios::ate); //abre o arquivo com leitura binária e posiciona o cursor no final
            if (inTexto.is_open()) {
                bytesOriginaisTotais += inTexto.tellg();//com o cursor no final, tellg() retorna o tamanho total do arquivo em bytes e acumula na variável bytesOriginaisTotais
            }
            inTexto.close();//fecha o arquivo de texto
        }

        // Executa a ingestão e captura o número de linhas lidas retornado pelo motor
        size_t totalLinhasLidas = deduplicator->ingerirArquivos(listaArquivos);

        serializer->salvarParaDisco(ARQUIVO_BIN);//salva arquivo em .dat

        std::ifstream inDat(ARQUIVO_BIN, std::ios::binary | std::ios::ate); //abre da mesma forma que antes, mas em vez de .txt agora é .dat
        size_t bytesBinarios = inDat.is_open() ? static_cast<size_t>(inDat.tellg()) : static_cast<size_t>(0); //se abriu, pega o tamanho, se não retorna 0
        inDat.close();//fecha arquivo

        double taxaCompressao = 0.0;
        if (bytesOriginaisTotais > 0) {
            taxaCompressao = (1.0 - (static_cast<double>(bytesBinarios) / bytesOriginaisTotais)) * 100.0; //calculo da compressão
        }

        std::cout << "Total de linhas lidas: " << totalLinhasLidas << "\n"
                  << "Total de linhas unicas armazenadas: " << storage->repositorioGlobal.size() << "\n"
                  << "Taxa de compressao: " << (taxaCompressao < 0 ? 0.0 : taxaCompressao) << "%\n";

    // =========================================================================
    // MODO: RECONSTRUÇÃO
    // =========================================================================
    } else if (comando == "reconstruir") {
        if (argc < 4) {//estritamente 3 parâmetros (executável, modo, nome do arquivo original, caminho do arquivo de saída)
            exibirAjuda();
            return;
        }

        std::string nomeOriginal = argv[2];
        std::string caminhoSaida = argv[3];

        serializer->carregarDoDisco(ARQUIVO_BIN);
        reconstructor->reconstruirArquivo(nomeOriginal, caminhoSaida);

    } else {
        exibirAjuda();
    }
}