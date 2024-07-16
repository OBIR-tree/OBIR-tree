#include "PathORAM.h"

using namespace CryptoPP;

size_t PathORAM::key_length = (size_t)AES::DEFAULT_KEYLENGTH;
size_t PathORAM::aes_block_size = (size_t)AES::BLOCKSIZE;

AutoSeededRandomPool PathORAM::prng;
CFB_Mode<AES>::Encryption PathORAM::encrypt_handler;
CFB_Mode<AES>::Decryption PathORAM::decrypt_handler;

void PathORAM::aes_encrypt(const std::string& plain, const byte* key,std::string& cipher)
{
    byte* iv = new byte[aes_block_size];
    encrypt_handler.GetNextIV(prng, iv);

    encrypt_handler.SetKeyWithIV(key, key_length, iv, aes_block_size);
    byte* cipher_text = new byte[plain.length()];
    encrypt_handler.ProcessData(cipher_text, (byte*)plain.c_str(), plain.length());
    cipher = std::string((const char*)iv, aes_block_size) + std::string((const char*)cipher_text, plain.length());
    //auto end = std::chrono::high_resolution_clock::now();
    //encryption_time += std::chrono::duration<double>(end - start).count();
}

void PathORAM::aes_decrypt(const std::string& cipher, const byte* key, std::string& plain) {
    //auto start = std::chrono::high_resolution_clock::now();
    decrypt_handler.SetKeyWithIV(key, key_length, (byte*)cipher.c_str(), aes_block_size);
    size_t cipher_length = cipher.length() - aes_block_size;
    if (cipher_length > 160)
    {
        plain = std::string("flag");
        return;
    }
    byte* plain_text = new byte[cipher_length];
    decrypt_handler.ProcessData(plain_text, (byte*)cipher.substr(aes_block_size).c_str(), cipher_length);
    plain = std::string((const char*)plain_text, cipher_length);
    //auto end = std::chrono::high_resolution_clock::now();
    //encryption_time += std::chrono::duration<double>(end - start).count();
}


std::string PathORAM::generate_random_block(const size_t& length) {
    byte *buf = new byte[length];
    prng.GenerateBlock(buf, length);
    return std::string((const char*)buf, length);
}
