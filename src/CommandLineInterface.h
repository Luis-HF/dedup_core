#ifndef COMMANDLINEINTERFACE_H
#define COMMANDLINEINTERFACE_H
#include <string>
#include "Storage.h"
#include "Deduplicator.h"
#include "Reconstructor.h"
#include "Serializer.h"


class CommandLineInterface {
private:
    Storage* storage;
    Deduplicator* deduplicator;
    Reconstructor* reconstructor;
    Serializer* serializer;

    void exibirAjuda() const;
    void exibirEstatisticas(size_t bytesOriginais, const std::string& arquivoDat) const;

public:

    CommandLineInterface(Storage* st, Deduplicator* dedup, Reconstructor* recon, Serializer* seri);

    void processarArgumentos(int argc, char* argv[]);

};
#endif