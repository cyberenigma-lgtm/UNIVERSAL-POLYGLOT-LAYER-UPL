#include "upl_comp3.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

static void test_comp3_encoding_and_nibbles(void) {
    uint8_t buf[16] = {0};

    // +12345 en comp3(5, 0) -> 3 bytes: [0x12, 0x34, 0x5C]
    bool ok = upl_comp3_from_int(buf, sizeof(buf), 12345, 5);
    assert(ok);
    assert(buf[0] == 0x12);
    assert(buf[1] == 0x34);
    assert(buf[2] == 0x5C);

    // -12345 en comp3(5, 0) -> 3 bytes: [0x12, 0x34, 0x5D]
    ok = upl_comp3_from_int(buf, sizeof(buf), -12345, 5);
    assert(ok);
    assert(buf[0] == 0x12);
    assert(buf[1] == 0x34);
    assert(buf[2] == 0x5D);

    printf("  [PASS] 1. Codificacion binaria BCD de nibbles y signos (+0x0C / -0x0D)\n");
}

static void test_comp3_fixed_point_strings(void) {
    uint8_t buf[16] = {0};
    char str_out[64] = {0};

    // "123.45" en comp3(5, 2) -> valor empaquetado: 12345C
    bool ok = upl_comp3_from_str(buf, sizeof(buf), "123.45", 5, 2);
    assert(ok);
    assert(buf[0] == 0x12);
    assert(buf[1] == 0x34);
    assert(buf[2] == 0x5C);

    ok = upl_comp3_to_str(str_out, sizeof(str_out), buf, 5, 2);
    assert(ok);
    assert(strcmp(str_out, "123.45") == 0);

    // "-0.50" en comp3(5, 2) -> 00050D
    ok = upl_comp3_from_str(buf, sizeof(buf), "-0.50", 5, 2);
    assert(ok);
    ok = upl_comp3_to_str(str_out, sizeof(str_out), buf, 5, 2);
    assert(ok);
    assert(strcmp(str_out, "-0.50") == 0);

    printf("  [PASS] 2. Conversion bidireccional texto decimal <-> binario comp3\n");
}

static void test_comp3_exact_arithmetic(void) {
    uint8_t a[8] = {0}, b[8] = {0}, res[8] = {0};
    char str_out[64] = {0};

    // Caso financiero emblemático: 100.00 - 0.01 debe dar exactamente 99.99
    // (en coma flotante IEEE 754 f32/f64 suele dar 99.989999999999995)
    upl_comp3_from_str(a, sizeof(a), "100.00", 6, 2);
    upl_comp3_from_str(b, sizeof(b), "0.01", 6, 2);

    bool ok = upl_comp3_sub(res, a, b, 6);
    assert(ok);
    upl_comp3_to_str(str_out, sizeof(str_out), res, 6, 2);
    assert(strcmp(str_out, "99.99") == 0);

    // Suma: 175.50 + 25.25 = 200.75
    upl_comp3_from_str(a, sizeof(a), "175.50", 6, 2);
    upl_comp3_from_str(b, sizeof(b), "25.25", 6, 2);
    ok = upl_comp3_add(res, a, b, 6);
    assert(ok);
    upl_comp3_to_str(str_out, sizeof(str_out), res, 6, 2);
    assert(strcmp(str_out, "200.75") == 0);

    // Saldo negativo por sobregiro: 50.00 - 120.00 = -70.00
    upl_comp3_from_str(a, sizeof(a), "50.00", 6, 2);
    upl_comp3_from_str(b, sizeof(b), "120.00", 6, 2);
    ok = upl_comp3_sub(res, a, b, 6);
    assert(ok);
    upl_comp3_to_str(str_out, sizeof(str_out), res, 6, 2);
    assert(strcmp(str_out, "-70.00") == 0);

    printf("  [PASS] 3. Aritmetica decimal por software con 0 errores de redondeo IEEE-754\n");
}

static void test_comp3_comparisons(void) {
    uint8_t a[8] = {0}, b[8] = {0};

    upl_comp3_from_str(a, sizeof(a), "500.00", 6, 2);
    upl_comp3_from_str(b, sizeof(b), "500.00", 6, 2);
    assert(upl_comp3_cmp(a, b, 6) == 0);

    upl_comp3_from_str(a, sizeof(a), "10.00", 6, 2);
    upl_comp3_from_str(b, sizeof(b), "20.00", 6, 2);
    assert(upl_comp3_cmp(a, b, 6) < 0);
    assert(upl_comp3_cmp(b, a, 6) > 0);

    upl_comp3_from_str(a, sizeof(a), "-5.00", 6, 2);
    upl_comp3_from_str(b, sizeof(b), "1.00", 6, 2);
    assert(upl_comp3_cmp(a, b, 6) < 0);

    printf("  [PASS] 4. Comparaciones algebraicas deterministas con signos (<, ==, >)\n");
}

int main(void) {
    printf("====================================================================\n");
    printf("  UPL+ SPEC SUITE: MOTOR MATEMATICO EXACTO COMP-3 (PACKED DECIMAL) \n");
    printf("====================================================================\n");

    test_comp3_encoding_and_nibbles();
    test_comp3_fixed_point_strings();
    test_comp3_exact_arithmetic();
    test_comp3_comparisons();

    printf("====================================================================\n");
    printf("  RESULTADO: 4/4 SUITES COMP-3 COMPLETADAS CON EXITO (0 FALLOS)     \n");
    printf("====================================================================\n");
    return 0;
}
