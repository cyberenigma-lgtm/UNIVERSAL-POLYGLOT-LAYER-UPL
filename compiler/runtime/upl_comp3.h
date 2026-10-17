#ifndef UPL_COMP3_H
#define UPL_COMP3_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * UPL+ COMP-3 (Packed Decimal / BCD Empaquetado) Runtime Engine
 * 
 * Implementación canónica de aritmética decimal exacta por software.
 * Formato estándar:
 * - 2 dígitos decimales (0-9) por byte en nibbles de 4 bits.
 * - Nibble final para signo: 0xC (+), 0xD (-), 0xF (sin signo/+).
 * - Tamaño en bytes: (precision / 2) + 1.
 * - Totalmente libre de coma flotante IEEE 754 (0 errores de redondeo).
 */

#define UPL_COMP3_SIGN_POS 0x0C
#define UPL_COMP3_SIGN_NEG 0x0D
#define UPL_COMP3_SIGN_UNS 0x0F

/* Calcular el número de bytes requeridos para una precisión dada */
static inline size_t upl_comp3_byte_length(int precision) {
    return (size_t)((precision / 2) + 1);
}

/* Codificar un entero de 64 bits en formato COMP-3 */
bool upl_comp3_from_int(uint8_t *dest, size_t dest_size, int64_t value, int precision);

/* Codificar una cadena decimal ("123.45" o "-987.60") en formato COMP-3 */
bool upl_comp3_from_str(uint8_t *dest, size_t dest_size, const char *str, int precision, int scale);

/* Decodificar un valor COMP-3 a una cadena decimal con signo y punto fijo */
bool upl_comp3_to_str(char *dest, size_t dest_size, const uint8_t *src, int precision, int scale);

/* Suma decimal exacta por software: res = a + b */
bool upl_comp3_add(uint8_t *res, const uint8_t *a, const uint8_t *b, int precision);

/* Resta decimal exacta por software: res = a - b */
bool upl_comp3_sub(uint8_t *res, const uint8_t *a, const uint8_t *b, int precision);

/* Comparación de dos valores COMP-3: retorna < 0 si a < b, 0 si a == b, > 0 si a > b */
int upl_comp3_cmp(const uint8_t *a, const uint8_t *b, int precision);

#endif /* UPL_COMP3_H */
