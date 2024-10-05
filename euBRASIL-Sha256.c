#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#define ROTR(x, n) ((x >> n) | (x << (32 - n)))
#define CH(x, y, z) ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SIGMA0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SIGMA1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SIGMA_0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3))
#define SIGMA_1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10))

// Constantes da tabela k
static const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Função para inicializar o estado (hash inicial)
void sha256_init(uint32_t *state) {
    state[0] = 0x6a09e667;
    state[1] = 0xbb67ae85;
    state[2] = 0x3c6ef372;
    state[3] = 0xa54ff53a;
    state[4] = 0x510e527f;
    state[5] = 0x9b05688c;
    state[6] = 0x1f83d9ab;
    state[7] = 0x5be0cd19;
}

// Função para processar um bloco de 512 bits
void sha256_transform(uint32_t *state, const uint8_t block[64]) {
    uint32_t w[64];
    uint32_t a, b, c, d, e, f, g, h, t1, t2;

    // Preparação das palavras de mensagem
    for (int i = 0; i < 16; ++i) {
        w[i]  = (block[i * 4] << 24);
        w[i] |= (block[i * 4 + 1] << 16);
        w[i] |= (block[i * 4 + 2] << 8);
        w[i] |= (block[i * 4 + 3]);
    }
    for (int i = 16; i < 64; ++i) {
        w[i] = SIGMA_1(w[i - 2]) + w[i - 7] + SIGMA_0(w[i - 15]) + w[i - 16];
    }

    // Inicialização das variáveis de trabalho
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    f = state[5];
    g = state[6];
    h = state[7];

    // Loop principal
    for (int i = 0; i < 64; ++i) {
        t1 = h + SIGMA1(e) + CH(e, f, g) + k[i] + w[i];
        t2 = SIGMA0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    // Atualização do estado
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

// Função para preencher a mensagem de acordo com o SHA-256
void sha256_update(uint8_t *message, uint64_t message_len, uint32_t *state) {
    uint8_t block[64];
    uint64_t i = 0;

    while (message_len >= 64) {
        memcpy(block, message + i, 64);
        sha256_transform(state, block);
        message_len -= 64;
        i += 64;
    }

    // Preenchimento do último bloco
    memset(block, 0, 64);
    memcpy(block, message + i, message_len);
    block[message_len] = 0x80;

    // Adiciona o comprimento total da mensagem em bits no final
    if (message_len >= 56) {
        sha256_transform(state, block);
        memset(block, 0, 64);
    }
    uint64_t bit_len = i * 8;
    block[63] = bit_len;
    block[62] = bit_len >> 8;
    block[61] = bit_len >> 16;
    block[60] = bit_len >> 24;
    block[59] = bit_len >> 32;
    block[58] = bit_len >> 40;
    block[57] = bit_len >> 48;
    block[56] = bit_len >> 56;
    sha256_transform(state, block);
}

// Função principal que calcula o hash de uma mensagem
void sha256(uint8_t *message, uint64_t message_len, uint8_t hash[32]) {
    uint32_t state[8];
    sha256_init(state);

    sha256_update(message, message_len, state);

    // Copia o resultado final para o array de saída
    for (int i = 0; i < 8; ++i) {
        hash[i * 4] = (state[i] >> 24) & 0xff;
        hash[i * 4 + 1] = (state[i] >> 16) & 0xff;
        hash[i * 4 + 2] = (state[i] >> 8) & 0xff;
        hash[i * 4 + 3] = state[i] & 0xff;
    }
}




// Função para converter uma string hexadecimal para um array de bytes
int hex_to_bytes(const char *hex, uint8_t *bytes, size_t bytes_len) {
    size_t hex_len = strlen(hex);

    // Verifica se o comprimento é par e se cabe no array de destino
    if (hex_len % 2 != 0 || hex_len / 2 > bytes_len) {
        return -1;  // Erro: comprimento inválido
    }

    // Conversão hexadecimal para bytes
    for (size_t i = 0; i < hex_len / 2; i++) {
        sscanf(&hex[2 * i], "%2hhx", &bytes[i]);
    }
    return 0;
}




// Função principal para executar o programa
int main(int argc, char *argv[]) {
    // Verifica se a chave privada foi fornecida como argumento
    if (argc != 2) {
		printf("euBRASIL( SHA256 ) versao 1.0\n\n");
        printf("Uso: %s <chave_privada_hexadecimal>\n", argv[0]);
        return 1;
    }

    // Recebe a chave privada em formato hexadecimal da linha de comando
    const char *private_key_hex = argv[1];
    size_t private_key_len = strlen(private_key_hex) / 2;

    // Supondo que a chave privada tem 32 bytes (256 bits)
    uint8_t private_key[32];

    // Converte a chave de hexadecimal para bytes
    if (hex_to_bytes(private_key_hex, private_key, sizeof(private_key)) != 0) {
        printf("Erro: A chave privada deve ser um valor hexadecimal válido com 64 caracteres.\n");
        return 1;
    }

    // Imprime a chave privada em formato binário (bytes)
	printf("euBRASIL( SHA256 ) versao 1.0\n\n");
    printf("Chave-Privada em formato hexadecimal:\n");
    for (size_t i = 0; i < sizeof(private_key); i++) {
        printf("%02x", private_key[i]);
    }
    printf("\n");
	
	

    // uint8_t message[] = "teste de hash sha256";
    uint8_t hash[32];

    sha256(private_key, strlen((char *)private_key), hash);

    // Exibe o hash resultante
    printf("SHA-256: ");
    for (int i = 0; i < 32; ++i) {
        printf("%02x", hash[i]);
    }
    printf("\n");

    return 0;
}
