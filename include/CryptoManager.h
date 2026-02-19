#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <stdexcept>

class CryptoManager {
public:
    static void generateRandomIV(unsigned char* iv) {
        if (!RAND_bytes(iv, 16)) {
            throw std::runtime_error("Error generando IV aleatorio");
        }
    }

    static std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext, const uint8_t* key, const uint8_t* iv) {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        int len;
        int ciphertext_len;
        std::vector<uint8_t> ciphertext(plaintext.size() + 16); // Espacio extra para padding

        EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
        EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size());
        ciphertext_len = len;
        EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len);
        ciphertext_len += len;

        ciphertext.resize(ciphertext_len);
        EVP_CIPHER_CTX_free(ctx);
        return ciphertext;
    }

    static std::vector<uint8_t> decrypt(const uint8_t* ciphertext, int ciphertext_len, const uint8_t* key, const uint8_t* iv) {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        int len;
        int plaintext_len;
        std::vector<uint8_t> plaintext(ciphertext_len);

        EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
        EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext, ciphertext_len);
        plaintext_len = len;
        EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
        plaintext_len += len;

        plaintext.resize(plaintext_len);
        EVP_CIPHER_CTX_free(ctx);
        return plaintext;
    }
};