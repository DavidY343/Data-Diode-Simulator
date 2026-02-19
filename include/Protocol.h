#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>
#include <cstdlib>
#include <string>
#include <iostream>

#pragma pack(push, 1)
struct DataPacket {
    uint32_t file_id;       // ID único de la transmisión
    uint32_t chunk_idx;     // Índice del fragmento actual
    uint32_t total_chunks;  // Total de fragmentos para saber cuándo terminar
    uint8_t iv[16];         // IV aleatorio para AES-256-CBC
    uint32_t payload_size;  // Tamaño de los datos cifrados (el último suele ser menor)
    uint8_t data[1400];     // Buffer de datos cifrados (ajustado para evitar fragmentación IP)
};
#pragma pack(pop)

inline std::string getEnvVar(const char* var, const char* defaultValue) {
    const char* val = std::getenv(var);
    return val ? std::string(val) : std::string(defaultValue);
}

const int UDP_PORT = std::stoi(getEnvVar("UDP_PORT", "8080"));
const std::string SHARED_KEY = getEnvVar("SHARED_KEY", "");

inline void checkConfig() {
    if (SHARED_KEY.length() != 32) {
        std::cerr << "ERROR: SHARED_KEY must be 32 bytes for AES-256" << std::endl;
        exit(1);
    }
}

#endif