#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "Storage.h"


class Serializer {
private:
    Storage* storage;

public:
    Serializer(Storage* storageCentral);



    void salvarParaDisco(const std::string& caminhoArquivoDat);


    void carregarDoDisco(const std::string& caminhoArquivoDat);
};

#endif