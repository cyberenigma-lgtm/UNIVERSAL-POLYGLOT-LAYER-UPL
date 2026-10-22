// ============================================================================
// TEST SUITE MAESTRA: VERIFICACION INDIVIDUAL UNO POR UNO DE MODULOS UPL+
// ============================================================================

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#endif

static int g_pass = 0;
static int g_fail = 0;

#define TEST_ASSERT(cond, msg) do { \
    if (cond) { \
        printf("  [PASS] %s\n", msg); \
        g_pass++; \
    } else { \
        printf("  [FAIL] %s (Line %d)\n", msg, __LINE__); \
        g_fail++; \
    } \
} while (0)

// 1. TEST GXNOX DUAL PARITY
typedef int32_t (*fn_GxnoxInit)(uint32_t);
typedef uint32_t (*fn_GxnoxCRC32)(uint8_t*, size_t);
typedef void (*fn_GxnoxEncodeMirror)(uint8_t*, uint8_t*, size_t, uint8_t);
typedef int32_t (*fn_GxnoxVerifyAndHeal)(uint8_t*, uint8_t*, size_t, uint8_t);
typedef uint32_t (*fn_GxnoxGetHealedCount)();

void test_gxnox(HMODULE h) {
    printf("\n=== [1/8] TEST PROYECTO: GXNOX-v3.0 (gxnox_dual_parity.dll) ===\n");
    fn_GxnoxInit f_init = (fn_GxnoxInit)GetProcAddress(h, "GxnoxInit");
    fn_GxnoxCRC32 f_crc = (fn_GxnoxCRC32)GetProcAddress(h, "GxnoxCRC32");
    fn_GxnoxEncodeMirror f_encode = (fn_GxnoxEncodeMirror)GetProcAddress(h, "GxnoxEncodeMirror");
    fn_GxnoxVerifyAndHeal f_heal = (fn_GxnoxVerifyAndHeal)GetProcAddress(h, "GxnoxVerifyAndHeal");
    fn_GxnoxGetHealedCount f_healed_cnt = (fn_GxnoxGetHealedCount)GetProcAddress(h, "GxnoxGetHealedCount");

    TEST_ASSERT(f_init && f_crc && f_encode && f_heal && f_healed_cnt, "Simbolos exportados de GXNOX DLL");

    if (f_init) {
        f_init(16);
        uint8_t par[8] = {10, 20, 30, 40, 50, 60, 70, 80};
        uint8_t impar[8] = {0};
        uint8_t salt = 0xAA;

        f_encode(par, impar, 8, salt);
        TEST_ASSERT(impar[0] == (80 ^ 0xAA), "Codificacion espejo invertida con salt");

        uint32_t crc1 = f_crc(par, 8);
        TEST_ASSERT(crc1 != 0, "Calculo de silicio CRC32");

        // Corromper intencionadamente el byte 3 de par
        par[3] = 0xFF;
        int32_t healed = f_heal(par, impar, 8, salt);
        TEST_ASSERT(healed == 1, "Deteccion y autocuracion de corrupcion hardware");
        TEST_ASSERT(par[3] == 40, "Restauracion exacta del valor original corrompido");
    }
}

// 2. TEST NEUROCOBOL
typedef int32_t (*fn_NcInit)();
typedef uint32_t (*fn_NcPackComp3)(uint8_t*, uint32_t, uint8_t*);
typedef uint32_t (*fn_NcUnpackComp3)(uint8_t*, uint32_t, uint8_t*);
typedef uint8_t (*fn_NcAddBcd)(uint8_t*, uint8_t*, uint8_t*, uint32_t);
typedef uint8_t (*fn_NcSubBcd)(uint8_t*, uint8_t*, uint8_t*, uint32_t);

void test_neurocobol(HMODULE h) {
    printf("\n=== [2/8] TEST PROYECTO: NeuroCOBOL-V3.0 (neurocobol_core.dll) ===\n");
    fn_NcInit f_init = (fn_NcInit)GetProcAddress(h, "NcInit");
    fn_NcPackComp3 f_pack = (fn_NcPackComp3)GetProcAddress(h, "NcPackComp3");
    fn_NcUnpackComp3 f_unpack = (fn_NcUnpackComp3)GetProcAddress(h, "NcUnpackComp3");
    fn_NcAddBcd f_add = (fn_NcAddBcd)GetProcAddress(h, "NcAddBcd");
    fn_NcSubBcd f_sub = (fn_NcSubBcd)GetProcAddress(h, "NcSubBcd");

    TEST_ASSERT(f_init && f_pack && f_unpack && f_add && f_sub, "Simbolos exportados de NeuroCOBOL DLL");

    if (f_init) {
        f_init();
        uint8_t num_ascii[] = "12345";
        uint8_t comp3_buf[8] = {0};
        uint8_t out_ascii[16] = {0};

        uint32_t packed_len = f_pack(num_ascii, 5, comp3_buf);
        TEST_ASSERT(packed_len > 0, "Empaquetado BCD COMP-3");

        f_unpack(comp3_buf, packed_len, out_ascii);
        TEST_ASSERT(strncmp((char*)out_ascii, "12345", 5) == 0, "Desempaquetado exacto de COMP-3");

        // Suma BCD de 4 digitos: 0520 + 0480 = 1000
        uint8_t a[4] = {0, 5, 2, 0};
        uint8_t b[4] = {0, 4, 8, 0};
        uint8_t res[4] = {0};
        f_add(a, b, res, 4);
        TEST_ASSERT(res[0] == 1 && res[1] == 0 && res[2] == 0 && res[3] == 0, "Suma decimal exacta BCD (1000)");
    }
}

// 3. TEST NEURO-GFX
typedef int32_t (*fn_GfxInit)(uint32_t, uint32_t);
typedef void (*fn_GfxClear)(uint32_t*, uint32_t, uint32_t);
typedef void (*fn_GfxSetPixel)(uint32_t*, int32_t, int32_t, uint32_t);
typedef void (*fn_GfxDrawLine)(uint32_t*, int32_t, int32_t, int32_t, int32_t, uint32_t);
typedef uint32_t (*fn_GfxBlendPixel)(uint32_t, uint32_t);

void test_neuro_gfx(HMODULE h) {
    printf("\n=== [3/8] TEST PROYECTO: NEURO-GFX (neuro_gfx_core.dll) ===\n");
    fn_GfxInit f_init = (fn_GfxInit)GetProcAddress(h, "GfxInit");
    fn_GfxClear f_clear = (fn_GfxClear)GetProcAddress(h, "GfxClear");
    fn_GfxSetPixel f_set = (fn_GfxSetPixel)GetProcAddress(h, "GfxSetPixel");
    fn_GfxDrawLine f_line = (fn_GfxDrawLine)GetProcAddress(h, "GfxDrawLine");
    fn_GfxBlendPixel f_blend = (fn_GfxBlendPixel)GetProcAddress(h, "GfxBlendPixel");

    TEST_ASSERT(f_init && f_clear && f_set && f_line && f_blend, "Simbolos exportados de NEURO-GFX DLL");

    if (f_init) {
        f_init(640, 480);
        uint32_t fb[100] = {0};
        f_clear(fb, 100, 0xFF0000FF); // Azul opaco
        TEST_ASSERT(fb[0] == 0xFF0000FF && fb[99] == 0xFF0000FF, "Limpieza de Framebuffer ARGB32");

        uint32_t blend = f_blend(0xFF000000, 0xFFFFFFFF); // Dest negro, Src blanco 100% opaco
        TEST_ASSERT(blend == 0xFFFFFFFF, "Alpha Blend total opaco");
    }
}

// 4. TEST NEURO-PROBE
typedef int32_t (*fn_ProbeInit)(int32_t, uint32_t);
typedef uint32_t (*fn_ProbeScanSerialStream)(uint8_t*, size_t, uint8_t*);
typedef uint32_t (*fn_ProbeCompareStreams)(uint8_t*, uint8_t*, size_t);

void test_neuro_probe(HMODULE h) {
    printf("\n=== [4/8] TEST PROYECTO: Neuro-probe (probe_engine.dll) ===\n");
    fn_ProbeInit f_init = (fn_ProbeInit)GetProcAddress(h, "ProbeInit");
    fn_ProbeScanSerialStream f_scan = (fn_ProbeScanSerialStream)GetProcAddress(h, "ProbeScanSerialStream");
    fn_ProbeCompareStreams f_comp = (fn_ProbeCompareStreams)GetProcAddress(h, "ProbeCompareStreams");

    TEST_ASSERT(f_init && f_scan && f_comp, "Simbolos exportados de Neuro-probe DLL");

    if (f_init) {
        f_init(1, 5);
        uint8_t stream[] = "\r\n[KERNEL_BOOT]: CPU_OK! #MEM_READY$\x00";
        uint8_t markers[64] = {0};
        uint32_t m_cnt = f_scan(stream, strlen((char*)stream), markers);
        TEST_ASSERT(m_cnt > 0, "Extraccion de markers de flujo serial");

        uint8_t st_a[4] = {1, 2, 3, 4};
        uint8_t st_b[4] = {1, 2, 9, 4};
        uint32_t diff = f_comp(st_a, st_b, 4);
        TEST_ASSERT(diff == 1, "Deteccion diferencial de paridad entre emuladores");
    }
}

// 5. TEST NEUROUNIVERSALASM (NUASM)
typedef int32_t (*fn_NuasmInit)(size_t);
typedef void (*fn_NuasmEmitNop)(uint8_t*);
typedef void (*fn_NuasmEmitRet)(uint8_t*);
typedef void (*fn_NuasmEmitXorRegReg)(uint8_t*, uint8_t, uint8_t);
typedef size_t (*fn_NuasmGetByteSize)();

void test_nuasm(HMODULE h) {
    printf("\n=== [5/8] TEST PROYECTO: NeuroUniversalASM (nuasm_core.dll) ===\n");
    fn_NuasmInit f_init = (fn_NuasmInit)GetProcAddress(h, "NuasmInit");
    fn_NuasmEmitNop f_nop = (fn_NuasmEmitNop)GetProcAddress(h, "NuasmEmitNop");
    fn_NuasmEmitRet f_ret = (fn_NuasmEmitRet)GetProcAddress(h, "NuasmEmitRet");
    fn_NuasmEmitXorRegReg f_xor = (fn_NuasmEmitXorRegReg)GetProcAddress(h, "NuasmEmitXorRegReg");
    fn_NuasmGetByteSize f_size = (fn_NuasmGetByteSize)GetProcAddress(h, "NuasmGetByteSize");

    TEST_ASSERT(f_init && f_nop && f_ret && f_xor && f_size, "Simbolos exportados de NUASM DLL");

    if (f_init) {
        f_init(1024);
        uint8_t code_buf[64] = {0};
        f_nop(code_buf); // 0x90
        TEST_ASSERT(code_buf[0] == 0x90, "Emision de NOP (0x90)");

        f_ret(code_buf); // 0xC3
        TEST_ASSERT(code_buf[1] == 0xC3, "Emision de RET (0xC3)");

        // XOR RAX, RAX: 48 31 C0
        f_xor(code_buf, 0, 0);
        TEST_ASSERT(code_buf[2] == 0x48 && code_buf[3] == 0x31 && code_buf[4] == 0xC0, "Emision de XOR RAX, RAX (48 31 C0)");
    }
}

// 6. TEST NEUROWILL
typedef int32_t (*fn_WillInit)();
typedef void (*fn_WillSetRegister)(uint32_t, uint64_t);
typedef uint64_t (*fn_WillGetRegister)(uint32_t);
typedef int32_t (*fn_WillExecuteBytecode)(uint8_t*, uint32_t, uint32_t);
typedef int32_t (*fn_WillGetZeroFlag)();

void test_neurowill(HMODULE h) {
    printf("\n=== [6/8] TEST PROYECTO: NeuroWill-Code (neurowill_core.dll) ===\n");
    fn_WillInit f_init = (fn_WillInit)GetProcAddress(h, "WillInit");
    fn_WillSetRegister f_set_reg = (fn_WillSetRegister)GetProcAddress(h, "WillSetRegister");
    fn_WillGetRegister f_get_reg = (fn_WillGetRegister)GetProcAddress(h, "WillGetRegister");
    fn_WillExecuteBytecode f_exec = (fn_WillExecuteBytecode)GetProcAddress(h, "WillExecuteBytecode");
    fn_WillGetZeroFlag f_zf = (fn_WillGetZeroFlag)GetProcAddress(h, "WillGetZeroFlag");

    TEST_ASSERT(f_init && f_set_reg && f_get_reg && f_exec && f_zf, "Simbolos exportados de NeuroWill DLL");

    if (f_init) {
        f_init();
        f_set_reg(0, 100);
        f_set_reg(1, 250);
        TEST_ASSERT(f_get_reg(0) == 100 && f_get_reg(1) == 250, "Carga directa en banco de registros R0, R1");

        // Bytecode: ADD R0, R1 (0x02, 0x00, 0x01) + HALT (0xFF)
        uint8_t prog[] = { 2, 0, 1, 255 };
        int32_t res = f_exec(prog, sizeof(prog), 100);
        TEST_ASSERT(res == 0, "Ejecucion limpia de ciclo de bytecode in-memory");
        TEST_ASSERT(f_get_reg(0) == 350, "Resultado determinista R0 = R0 + R1 (350)");
    }
}

// 7. TEST IAGROK CORE
typedef int32_t (*fn_IagrokInit)();
typedef int32_t (*fn_IagrokEnqueueTask)(uint32_t, uint32_t, uint64_t);
typedef uint32_t (*fn_IagrokDispatchHighestPriority)();
typedef uint64_t (*fn_IagrokGetTasksProcessed)();

void test_iagrok(HMODULE h) {
    printf("\n=== [7/8] TEST PROYECTO: IAGROK-V5 (iagrok_core.dll) ===\n");
    fn_IagrokInit f_init = (fn_IagrokInit)GetProcAddress(h, "IagrokInit");
    fn_IagrokEnqueueTask f_enqueue = (fn_IagrokEnqueueTask)GetProcAddress(h, "IagrokEnqueueTask");
    fn_IagrokDispatchHighestPriority f_dispatch = (fn_IagrokDispatchHighestPriority)GetProcAddress(h, "IagrokDispatchHighestPriority");
    fn_IagrokGetTasksProcessed f_proc = (fn_IagrokGetTasksProcessed)GetProcAddress(h, "IagrokGetTasksProcessed");

    TEST_ASSERT(f_init && f_enqueue && f_dispatch && f_proc, "Simbolos exportados de IAGROK DLL");

    if (f_init) {
        f_init();
        f_enqueue(101, 10, 0xAAAA); // Prioridad 10
        f_enqueue(202, 1, 0xBBBB);  // Prioridad 1 (Max urgente)
        f_enqueue(303, 5, 0xCCCC);  // Prioridad 5

        uint32_t first = f_dispatch();
        TEST_ASSERT(first == 202, "Despacho por atencion prioritaria (Tarea urgente 202 primero)");
        uint32_t second = f_dispatch();
        TEST_ASSERT(second == 303, "Segundo despacho correcto (Tarea 303)");
        TEST_ASSERT(f_proc() == 2, "Contador de eventos procesados en silicio");
    }
}

// 8. TEST DVTRGAS
typedef int32_t (*fn_DvtrgasInit)(void*);
void test_dvtrgas(HMODULE h) {
    printf("\n=== [8/8] TEST PROYECTO: DVTRGAS (dvtrgas.dll) ===\n");
    fn_DvtrgasInit f_init = (fn_DvtrgasInit)GetProcAddress(h, "DvtrgaInit");
    if (!f_init) f_init = (fn_DvtrgasInit)GetProcAddress(h, "DvtrgaInit_ext");
    TEST_ASSERT(f_init != NULL, "Simbolo exportado de DVTRGAS DLL (DvtrgaInit)");
}

int main() {
    printf("====================================================================\n");
    printf("  SUITE MAESTRA DE VALIDACION INDIVIDUAL UNO POR UNO (UPL+ NATIVO)  \n");
    printf("====================================================================\n");

    HMODULE h_gxnox = LoadLibraryA("bin\\gxnox_dual_parity.dll");
    test_gxnox(h_gxnox);

    HMODULE h_nc = LoadLibraryA("bin\\neurocobol_core.dll");
    test_neurocobol(h_nc);

    HMODULE h_gfx = LoadLibraryA("bin\\neuro_gfx_core.dll");
    test_neuro_gfx(h_gfx);

    HMODULE h_probe = LoadLibraryA("bin\\probe_engine.dll");
    test_neuro_probe(h_probe);

    HMODULE h_nuasm = LoadLibraryA("bin\\nuasm_core.dll");
    test_nuasm(h_nuasm);

    HMODULE h_will = LoadLibraryA("bin\\neurowill_core.dll");
    test_neurowill(h_will);

    HMODULE h_iagrok = LoadLibraryA("bin\\iagrok_core.dll");
    test_iagrok(h_iagrok);

    HMODULE h_dvtrgas = LoadLibraryA("dvtrgas.dll");
    test_dvtrgas(h_dvtrgas);

    printf("\n====================================================================\n");
    printf("  RESUMEN FINAL DE VALIDACION:\n");
    printf("  [PASS]: %d pruebas superadas\n", g_pass);
    printf("  [FAIL]: %d pruebas fallidas\n", g_fail);
    printf("====================================================================\n");

    return (g_fail == 0) ? 0 : 1;
}
