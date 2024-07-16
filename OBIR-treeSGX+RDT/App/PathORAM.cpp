//
// Created by hhhead on 24-3-21.
//
#include "Enclave_u.h"
#include "PathORAM.h"
void encrypt(int eid, string* retval, string plain){
    aes_encrypt(1, retval, plain);
}
void decrypt(int eid, string* retval, string cipher){
    aes_decrypt(1,retval, cipher);
}
void random_block(int eid, string* retval, int length){
    generate_random_block(1,retval, length);

}