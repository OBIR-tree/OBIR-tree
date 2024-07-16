#ifndef PATHORAM_H
#define PATHORAM_H

#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>
#include <random>
#include <string>

class PathORAM {
public:
    static void aes_encrypt(const std::string& plain, const byte* key, std::string& cipher);
    static void aes_decrypt(const std::string& plain, const byte* key, std::string& cipher);
    static std::string generate_random_block(const size_t& length);


    static size_t key_length;
    static size_t aes_block_size;
    static CryptoPP::AutoSeededRandomPool prng;
    static CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption encrypt_handler;
    static CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption decrypt_handler;


    PathORAM() = default;
};


#endif //PATHORAM_H
