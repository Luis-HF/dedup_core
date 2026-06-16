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

}