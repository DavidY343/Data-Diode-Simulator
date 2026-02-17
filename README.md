# C++ Secure Data Diode Simulator (UDP + AES-256)

Este proyecto es un simulador de un **Diodo de Datos físico**. Implementa una arquitectura de red unidireccional donde un emisor transfiere archivos (imágenes) a un receptor de forma segura, sin posibilidad de respuesta o confirmación (ACK), simulando el aislamiento de infraestructuras críticas.

## Objetivos del Proyecto

- **Unidireccionalidad Estricta**: Uso de protocolo UDP para eliminar el handshake de TCP.
- **Confidencialidad Robusta**: Cifrado de archivos mediante AES-256-CBC usando OpenSSL.
- **Integridad y Reconstrucción**: Protocolo de fragmentación personalizado para manejar archivos grandes (imágenes) sobre UDP.
- **Resiliencia de Datos**: Implementación de Redundancia Simple (envío doble de paquetes) para mitigar la pérdida de datos en redes inestables.

## Especificaciones Técnicas

### 1. Protocolo de Comunicación (Custom Frame)

Cada datagrama UDP enviado tendrá la siguiente estructura binaria para permitir que el receptor sea "autónomo":

| Campo | Tipo | Tamaño | Descripción |
|-------|------|--------|-------------|
| File ID | uint32_t | 4 bytes | Identificador único de la sesión de envío. |
| Chunk Index | uint32_t | 4 bytes | Posición del fragmento actual. |
| Total Chunks | uint32_t | 4 bytes | Cantidad total de fragmentos esperados. |
| IV | unsigned char[16] | 16 bytes | Vector de Inicialización aleatorio (AES). |
| Payload | byte[] | Variable | Datos de la imagen cifrados. |

### 2. Seguridad (AES-256-CBC)

- **Clave (PSK)**: Pre-compartida de 32 bytes (256 bits).
- **IV Dinámico**: Se genera un IV único por cada fragmento. Aunque el atacante vea dos fragmentos con contenido idéntico, el resultado cifrado será diferente.
- **Librería**: OpenSSL 3.x.

### 3. Flujo de Trabajo

**Emisor (Source)**:
1. Carga una imagen (.jpg, .png).
2. La divide en bloques de ~1400 bytes para evitar fragmentación IP.
3. Cifra cada bloque con un IV nuevo.
4. Redundancia: Envía cada paquete dos veces seguidas con un pequeño delay.

**Receptor (Sink)**:
1. Escucha en el puerto UDP configurado.
2. Extrae el IV del encabezado y descifra el payload.
3. Almacena los bloques en un buffer basado en el Chunk Index.
4. Al recibir el último bloque, reconstruye la imagen y la guarda en disco.

## Estructura del Código

```
./
├── src/
│   ├── common/          # Estructuras de datos y constantes
│   ├── crypto/          # Lógica de OpenSSL (Encryptor/Decryptor)
│   ├── sender/          # Lógica del cliente y fragmentación
│   └── receiver/        # Lógica del servidor y ensamblado
├── include/             # Headers (.h)
├── keys/                # Clave pre-compartida (key.bin)
└── Makefile             # Compilación con flags de OpenSSL
```

## Requisitos

- Compilador C++17 (G++ o Clang).
- OpenSSL Development Library (`sudo apt install libssl-dev`).
- Entorno Linux (recomendado para manejo de sockets nativos).