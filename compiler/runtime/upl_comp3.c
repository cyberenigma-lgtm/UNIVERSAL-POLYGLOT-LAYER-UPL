#include "upl_comp3.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t get_nibble(const uint8_t *bytes, size_t idx) {
    size_t byte_idx = idx / 2;
    return (idx % 2 == 1) ? (bytes[byte_idx] & 0x0F) : ((bytes[byte_idx] >> 4) & 0x0F);
}

static void set_nibble(uint8_t *bytes, size_t idx, uint8_t val) {
    size_t byte_idx = idx / 2;
    if (idx % 2 == 1) {
        bytes[byte_idx] = (bytes[byte_idx] & 0xF0) | (val & 0x0F);
    } else {
        bytes[byte_idx] = (bytes[byte_idx] & 0x0F) | ((val & 0x0F) << 4);
    }
}

static int get_sign_multiplier(const uint8_t *bytes, int precision) {
    size_t byte_len = upl_comp3_byte_length(precision);
    size_t sign_idx = (byte_len * 2) - 1;
    uint8_t sign_nibble = get_nibble(bytes, sign_idx);
    return (sign_nibble == UPL_COMP3_SIGN_NEG) ? -1 : 1;
}

bool upl_comp3_from_int(uint8_t *dest, size_t dest_size, int64_t value, int precision) {
    size_t byte_len = upl_comp3_byte_length(precision);
    if (dest_size < byte_len) return false;
    memset(dest, 0, byte_len);

    uint8_t sign = (value < 0) ? UPL_COMP3_SIGN_NEG : UPL_COMP3_SIGN_POS;
    uint64_t mag = (value < 0) ? (uint64_t)(-value) : (uint64_t)value;

    size_t total_nibbles = byte_len * 2;
    set_nibble(dest, total_nibbles - 1, sign);

    for (int i = (int)total_nibbles - 2; i >= 0; --i) {
        set_nibble(dest, (size_t)i, (uint8_t)(mag % 10));
        mag /= 10;
    }

    return (mag == 0); // Si mag > 0 hubo desbordamiento de precisión
}

bool upl_comp3_from_str(uint8_t *dest, size_t dest_size, const char *str, int precision, int scale) {
    size_t byte_len = upl_comp3_byte_length(precision);
    if (dest_size < byte_len || !str) return false;
    memset(dest, 0, byte_len);

    while (isspace((unsigned char)*str)) str++;

    uint8_t sign = UPL_COMP3_SIGN_POS;
    if (*str == '-') {
        sign = UPL_COMP3_SIGN_NEG;
        str++;
    } else if (*str == '+') {
        str++;
    }

    char int_part[64] = {0};
    char frac_part[64] = {0};
    size_t int_len = 0;
    size_t frac_len = 0;
    bool has_dot = false;

    while (*str) {
        if (*str == '.') {
            has_dot = true;
            str++;
            continue;
        }
        if (!isdigit((unsigned char)*str)) break;

        if (!has_dot) {
            if (int_len < sizeof(int_part) - 1) int_part[int_len++] = *str;
        } else {
            if (frac_len < sizeof(frac_part) - 1) frac_part[frac_len++] = *str;
        }
        str++;
    }

    // Alinear parte fraccionaria según escala
    while (frac_len < (size_t)scale && frac_len < sizeof(frac_part) - 1) {
        frac_part[frac_len++] = '0';
    }
    frac_len = (size_t)scale;

    // Concatenar todos los dígitos
    char full_digits[128] = {0};
    snprintf(full_digits, sizeof(full_digits), "%s%.*s", int_part, scale, frac_part);
    size_t full_len = strlen(full_digits);

    if ((int)full_len > precision) return false; // Desbordamiento

    // Rellenar con ceros a la izquierda
    size_t total_nibbles = byte_len * 2;
    set_nibble(dest, total_nibbles - 1, sign);

    int digit_ptr = (int)full_len - 1;
    for (int i = (int)total_nibbles - 2; i >= 0; --i) {
        if (digit_ptr >= 0) {
            set_nibble(dest, (size_t)i, (uint8_t)(full_digits[digit_ptr] - '0'));
            digit_ptr--;
        } else {
            set_nibble(dest, (size_t)i, 0);
        }
    }

    return true;
}

bool upl_comp3_to_str(char *dest, size_t dest_size, const uint8_t *src, int precision, int scale) {
    if (!dest || !src || dest_size < 4) return false;
    size_t byte_len = upl_comp3_byte_length(precision);
    size_t total_nibbles = byte_len * 2;

    int sign = get_sign_multiplier(src, precision);
    size_t d_idx = 0;

    if (sign < 0 && d_idx < dest_size - 1) {
        dest[d_idx++] = '-';
    }

    // Extraer dígitos (omitir nibble de signo final)
    char raw_digits[128] = {0};
    size_t r_len = 0;
    // Omitir primer nibble si la precisión es par
    size_t start_nibble = (precision % 2 == 0) ? 1 : 0;
    for (size_t i = start_nibble; i < total_nibbles - 1; ++i) {
        raw_digits[r_len++] = (char)('0' + get_nibble(src, i));
    }

    // Omitir ceros a la izquierda antes del punto
    size_t int_digits_count = (r_len > (size_t)scale) ? (r_len - (size_t)scale) : 1;
    size_t first_non_zero = 0;
    while (first_non_zero + 1 < int_digits_count && raw_digits[first_non_zero] == '0') {
        first_non_zero++;
    }

    for (size_t i = first_non_zero; i < int_digits_count && d_idx < dest_size - 1; ++i) {
        dest[d_idx++] = raw_digits[i];
    }

    if (scale > 0 && d_idx < dest_size - 1) {
        dest[d_idx++] = '.';
        for (size_t i = int_digits_count; i < r_len && d_idx < dest_size - 1; ++i) {
            dest[d_idx++] = raw_digits[i];
        }
    }

    dest[d_idx] = '\0';
    return true;
}

static void extract_digits(const uint8_t *src, int precision, uint8_t *digits, size_t count) {
    size_t byte_len = upl_comp3_byte_length(precision);
    size_t total_nibbles = byte_len * 2;
    size_t start = (precision % 2 == 0) ? 1 : 0;
    size_t out = 0;
    for (size_t i = start; i < total_nibbles - 1 && out < count; ++i) {
        digits[out++] = get_nibble(src, i);
    }
}

static void pack_digits(uint8_t *dest, int precision, const uint8_t *digits, size_t count, uint8_t sign) {
    size_t byte_len = upl_comp3_byte_length(precision);
    size_t total_nibbles = byte_len * 2;
    memset(dest, 0, byte_len);
    set_nibble(dest, total_nibbles - 1, sign);

    int d_ptr = (int)count - 1;
    for (int i = (int)total_nibbles - 2; i >= 0; --i) {
        if (d_ptr >= 0) {
            set_nibble(dest, (size_t)i, digits[d_ptr]);
            d_ptr--;
        } else {
            set_nibble(dest, (size_t)i, 0);
        }
    }
}

static int compare_magnitudes(const uint8_t *a_dig, const uint8_t *b_dig, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (a_dig[i] < b_dig[i]) return -1;
        if (a_dig[i] > b_dig[i]) return 1;
    }
    return 0;
}

int upl_comp3_cmp(const uint8_t *a, const uint8_t *b, int precision) {
    int sign_a = get_sign_multiplier(a, precision);
    int sign_b = get_sign_multiplier(b, precision);

    if (sign_a != sign_b) return (sign_a < sign_b) ? -1 : 1;

    uint8_t a_dig[128] = {0}, b_dig[128] = {0};
    extract_digits(a, precision, a_dig, (size_t)precision);
    extract_digits(b, precision, b_dig, (size_t)precision);

    int cmp = compare_magnitudes(a_dig, b_dig, (size_t)precision);
    return (sign_a < 0) ? -cmp : cmp;
}

bool upl_comp3_add(uint8_t *res, const uint8_t *a, const uint8_t *b, int precision) {
    int sign_a = get_sign_multiplier(a, precision);
    int sign_b = get_sign_multiplier(b, precision);

    uint8_t a_dig[128] = {0}, b_dig[128] = {0}, r_dig[128] = {0};
    extract_digits(a, precision, a_dig, (size_t)precision);
    extract_digits(b, precision, b_dig, (size_t)precision);

    if (sign_a == sign_b) {
        // Misma polaridad: suma de magnitudes
        int carry = 0;
        for (int i = precision - 1; i >= 0; --i) {
            int sum = a_dig[i] + b_dig[i] + carry;
            r_dig[i] = (uint8_t)(sum % 10);
            carry = sum / 10;
        }
        if (carry > 0) return false; // Desbordamiento de dígitos
        pack_digits(res, precision, r_dig, (size_t)precision, (sign_a < 0) ? UPL_COMP3_SIGN_NEG : UPL_COMP3_SIGN_POS);
    } else {
        // Polaridad opuesta: resta de magnitudes
        int cmp = compare_magnitudes(a_dig, b_dig, (size_t)precision);
        const uint8_t *larger = (cmp >= 0) ? a_dig : b_dig;
        const uint8_t *smaller = (cmp >= 0) ? b_dig : a_dig;
        uint8_t res_sign = (cmp >= 0) ? ((sign_a < 0) ? UPL_COMP3_SIGN_NEG : UPL_COMP3_SIGN_POS)
                                      : ((sign_b < 0) ? UPL_COMP3_SIGN_NEG : UPL_COMP3_SIGN_POS);

        int borrow = 0;
        for (int i = precision - 1; i >= 0; --i) {
            int diff = larger[i] - smaller[i] - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            r_dig[i] = (uint8_t)diff;
        }
        pack_digits(res, precision, r_dig, (size_t)precision, res_sign);
    }

    return true;
}

bool upl_comp3_sub(uint8_t *res, const uint8_t *a, const uint8_t *b, int precision) {
    size_t byte_len = upl_comp3_byte_length(precision);
    uint8_t neg_b[64] = {0};
    memcpy(neg_b, b, byte_len);

    // Invertir el signo de b
    size_t total_nibbles = byte_len * 2;
    uint8_t sign_b = get_nibble(b, total_nibbles - 1);
    uint8_t inverted = (sign_b == UPL_COMP3_SIGN_NEG) ? UPL_COMP3_SIGN_POS : UPL_COMP3_SIGN_NEG;
    set_nibble(neg_b, total_nibbles - 1, inverted);

    return upl_comp3_add(res, a, neg_b, precision);
}
