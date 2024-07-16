//
// Created by hhhead on 24-3-21.
//
#include "Enclave_u.h"
#ifndef SGXOIRT3_SEARCH_复件_PATHORAM_H
#define SGXOIRT3_SEARCH_复件_PATHORAM_H


class PathORAM {
    public:
   static void encrypt(int eid, string* retval, string plain);
   static void decrypt(int eid, string* retval, string cipher);
   static void random_block(int eid, string* retval, int length);

    PathORAM() = default;
};


#endif //SGXOIRT3_SEARCH_复件_PATHORAM_H
