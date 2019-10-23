// Copyright 2019 Nedelcu Horia (nedelcu.horia.alexandru@gmail.com)

#include <string>
#include "./hash_functions.h"

ULL int_hash(int number) {
    ULL hash = 5381;

    while (number) {
    	hash = hash * 33 ^ (number % 10);
    	number /= 10;
    }

    return hash;
}

ULL string_hash(std::string str) {
    ULL hash = 5381;

    for (unsigned int i = 0; i < str.size(); ++i) {
        hash = hash * 33 ^ str[i];
    }

    return hash;
}
