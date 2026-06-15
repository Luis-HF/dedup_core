#ifndef RECONSTRUCTOR_H
#define RECONSTRUCTOR_H

#include <string>
#include "Storage.h"

class Reconstructor {
private:
    Storage* storage;

public:
    Reconstructor(Storage* storageCentral);

    void reconstruirArquivo(const std::string& nomeOriginal, const std::string& nomeSaida);
};

#endif