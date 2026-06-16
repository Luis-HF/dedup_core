#include "Storage.h"
#include "Deduplicator.h"
#include "Reconstructor.h"
#include "Serializer.h"
#include "CommandLineInterface.h"

int main(int argc, char* argv[]) {
    Storage storageCentral;//instancia o Storage

    Deduplicator motorIngestao(&storageCentral);//instancia as outras classes injetando a dependência do Storage
    Reconstructor motorReconstrucao(&storageCentral);
    Serializer motorPersistencia(&storageCentral);

    CommandLineInterface cli(&storageCentral, &motorIngestao, &motorReconstrucao, &motorPersistencia);//injeta dependências no CLI

    cli.processarArgumentos(argc, argv); //processa os argumentos passados no terminal, o CLI é responsável por chamar os métodos corretos do motor de acordo com o modo selecionado

    return 0; 
}