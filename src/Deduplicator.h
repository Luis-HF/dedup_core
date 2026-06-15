#ifndef DEDUPLICATOR_H
#define DEDUPLICATOR_H

#include <string>
#include <vector>
#include "Storage.h"

class Deduplicator{
private:
    Storage* storage;

public:
    Deduplicator(Storage* storageCentral);

    void ingerirArquivos(const std::vector<std::string>& listaNomesArquivos);
};


#endif