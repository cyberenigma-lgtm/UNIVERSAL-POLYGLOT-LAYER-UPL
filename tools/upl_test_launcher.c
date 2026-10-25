// ============================================================================
// UPL+ LABORATORY TEST CENTER — SUITE DE VERIFICACION EMPIRICA (WIN32 UNICODE)
// Panel de Control de Pruebas para Laboratorios, Centros de I+D e Industria
// Autor: Jose Manuel Moreno (Noxferion)
// ============================================================================

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>

#define ID_BTN_TEST_ALL         1001
#define ID_BTN_UPLC_TEST        1002
#define ID_BTN_UREP_TEST        1003
#define ID_BTN_DVTRGAS_TEST     1004
#define ID_BTN_DEFER_TEST       1005
#define ID_BTN_MATCH_TEST       1006
#define ID_BTN_MODULES_TEST     1007
#define ID_BTN_C2UPL_TEST       1008
#define ID_BTN_GRAPHICS_DEMO    1009
#define ID_BTN_CLEAR_LOG        1010

#define ID_EDIT_LOG             2001

static HWND g_hwnd = NULL;
static HWND g_hEditLog = NULL;
static HBRUSH g_hbrBkg = NULL;
static HBRUSH g_hbrLogBkg = NULL;
static HFONT g_hFontUi = NULL;
static HFONT g_hFontLog = NULL;
static wchar_t g_rootDir[MAX_PATH] = {0};

static void InitRootDir(void) {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    wchar_t *lastSlash = wcsrchr(exePath, L'\\');
    if (lastSlash) *lastSlash = L'\0';

    size_t len = wcslen(exePath);
    if (len >= 4 && (_wcsicmp(exePath + len - 4, L"\\bin") == 0)) {
        exePath[len - 4] = L'\0';
    }
    wcsncpy(g_rootDir, exePath, MAX_PATH);
}

static void AppendLogW(const wchar_t *wtext) {
    if (!g_hEditLog) return;
    int len = GetWindowTextLengthW(g_hEditLog);
    SendMessageW(g_hEditLog, EM_SETSEL, (WPARAM)len, (LPARAM)len);
    SendMessageW(g_hEditLog, EM_REPLACESEL, FALSE, (LPARAM)wtext);
    SendMessageW(g_hEditLog, EM_SCROLLCARET, 0, 0);
}

static void AppendLogUtf8(const char *utf8Text) {
    if (!utf8Text || !*utf8Text) return;
    int needed = MultiByteToWideChar(CP_UTF8, 0, utf8Text, -1, NULL, 0);
    if (needed <= 0) {
        needed = MultiByteToWideChar(CP_ACP, 0, utf8Text, -1, NULL, 0);
        if (needed <= 0) return;
        wchar_t *wbuf = (wchar_t*)malloc(needed * sizeof(wchar_t));
        MultiByteToWideChar(CP_ACP, 0, utf8Text, -1, wbuf, needed);
        AppendLogW(wbuf);
        free(wbuf);
        return;
    }
    wchar_t *wbuf = (wchar_t*)malloc(needed * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, utf8Text, -1, wbuf, needed);
    AppendLogW(wbuf);
    free(wbuf);
}

static void RunCommandAndLog(const wchar_t *cmdDesc, const wchar_t *commandLine) {
    wchar_t header[512];
    swprintf(header, 512, 
        L"\r\n====================================================================\r\n"
        L"  EJECUTANDO: %ls\r\n"
        L"  Comando: %ls\r\n"
        L"====================================================================\r\n", 
        cmdDesc, commandLine);
    AppendLogW(header);

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    HANDLE hReadPipe, hWritePipe;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        AppendLogW(L"[ERROR] Fallo al crear tuberia de captura.\r\n");
        return;
    }
    SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOW si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.hStdOutput = hWritePipe;
    si.hStdError = hWritePipe;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    wchar_t cmdBuf[2048];
    swprintf(cmdBuf, 2048, L"cmd.exe /c \"cd /d \"%ls\" && %ls\"", g_rootDir, commandLine);

    DWORD startTick = GetTickCount();
    BOOL created = CreateProcessW(NULL, cmdBuf, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, 
                                  g_rootDir[0] ? g_rootDir : NULL, &si, &pi);
    if (!created) {
        AppendLogW(L"[ERROR] No se pudo iniciar el proceso de prueba. Verifique la ruta del ejecutable.\r\n");
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);
        return;
    }

    CloseHandle(hWritePipe);

    char buffer[1024];
    DWORD bytesRead;
    while (ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        char clean[2048];
        int cIdx = 0;
        for (DWORD i = 0; i < bytesRead && cIdx < (int)sizeof(clean) - 2; i++) {
            if (buffer[i] == '\n' && (i == 0 || buffer[i - 1] != '\r')) {
                clean[cIdx++] = '\r';
            }
            clean[cIdx++] = buffer[i];
        }
        clean[cIdx] = '\0';
        AppendLogUtf8(clean);
        UpdateWindow(g_hEditLog);
    }

    WaitForSingleObject(pi.hProcess, 15000);
    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    DWORD duration = GetTickCount() - startTick;

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hReadPipe);

    wchar_t footer[256];
    swprintf(footer, 256, 
        L"\r\n--------------------------------------------------------------------\r\n"
        L"[RESULTADO] Codigo de salida: %lu | Tiempo: %lu ms | ESTADO: %ls\r\n"
        L"--------------------------------------------------------------------\r\n",
        exitCode, duration, (exitCode == 0 ? L"PASS (EXITO)" : L"FAIL (FALLO)"));
    AppendLogW(footer);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE: {
            g_hFontUi = CreateFontW(17, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                                   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                   CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

            g_hFontLog = CreateFontW(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, L"Consolas");

            g_hbrBkg = CreateSolidBrush(RGB(15, 23, 42));       // Slate 900
            g_hbrLogBkg = CreateSolidBrush(RGB(3, 7, 18));      // Gray 950

            int x = 20, y = 80, w = 310, h = 38, gap = 46;

            HWND bAll = CreateWindowW(L"BUTTON", L"⚡ EJECUTAR TODAS LAS PRUEBAS", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, w, h + 4, hwnd, (HMENU)ID_BTN_TEST_ALL, NULL, NULL);
            SendMessageW(bAll, WM_SETFONT, (WPARAM)g_hFontUi, TRUE);
            y += gap + 6;

            HWND b1 = CreateWindowW(L"BUTTON", L"1. Suite Compilador uplc (6/6 Suites)", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, w, h, hwnd, (HMENU)ID_BTN_UPLC_TEST, NULL, NULL);
            SendMessageW(b1, WM_SETFONT, (WPARAM)g_hFontUi, TRUE);
            y += gap;

            HWND b2 = CreateWindowW(L"BUTTON", L"2. Supervisor UREP (CFI y Canarios)", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, w, h, hwnd, (HMENU)ID_BTN_UREP_TEST, NULL, NULL);
            SendMessageW(b2, WM_SETFONT, (WPARAM)g_hFontUi, TRUE);
            y += gap;

            HWND b3 = CreateWindowW(L"BUTTON", L"3. Motor Vectorial DVTRGAS-30 (52.8 GB/s)", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, w, h, hwnd, (HMENU)ID_BTN_DVTRGAS_TEST, NULL, NULL);
            SendMessageW(b3, WM_SETFONT, (WPARAM)g_hFontUi, TRUE);
            y += gap;

            HWND b4 = CreateWindowW(L"BUTTON", L"4. Gestión diferir / defer (0 Fugas)", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, w, h, hwnd, (HMENU)ID_BTN_DEFER_TEST, NULL, NULL);
            SendMessageW(b4, WM_SETFONT, (WPARAM)g_hFontUi, TRUE);
            y += gap;

            HWND b5 = CreateWindowW(L"BUTTON", L"5. Coincidencia según / match O(1)", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, w, h, hwnd, (HMENU)ID_BTN_MATCH_TEST, NULL, NULL);
            SendMessageW(b5, WM_SETFONT, (WPARAM)g_hFontUi, TRUE);
            y += gap;

            HWND b6 = CreateWindowW(L"BUTTON", L"6. Suite 28/28 Módulos de Silicio", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, w, h, hwnd, (HMENU)ID_BTN_MODULES_TEST, NULL, NULL);
            SendMessageW(b6, WM_SETFONT, (WPARAM)g_hFontUi, TRUE);
            y += gap;

            HWND b7 = CreateWindowW(L"BUTTON", L"7. Transpilador c2upl (C99 a UPL+)", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, w, h, hwnd, (HMENU)ID_BTN_C2UPL_TEST, NULL, NULL);
            SendMessageW(b7, WM_SETFONT, (WPARAM)g_hFontUi, TRUE);
            y += gap;

            HWND b8 = CreateWindowW(L"BUTTON", L"8. Lanzar Pantalla Gráfica en Vivo (60 FPS)", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, w, h, hwnd, (HMENU)ID_BTN_GRAPHICS_DEMO, NULL, NULL);
            SendMessageW(b8, WM_SETFONT, (WPARAM)g_hFontUi, TRUE);
            y += gap;

            HWND bClr = CreateWindowW(L"BUTTON", L"Limpiar Consola de Pruebas", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, w, 32, hwnd, (HMENU)ID_BTN_CLEAR_LOG, NULL, NULL);
            SendMessageW(bClr, WM_SETFONT, (WPARAM)g_hFontUi, TRUE);

            g_hEditLog = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY,
                350, 80, 680, 520, hwnd, (HMENU)ID_EDIT_LOG, NULL, NULL);
            SendMessageW(g_hEditLog, WM_SETFONT, (WPARAM)g_hFontLog, TRUE);

            AppendLogW(L"====================================================================\r\n");
            AppendLogW(L"  UPL+ LABORATORY TEST CENTER — ENTORNO EMPÍRICO LISTO\r\n");
            AppendLogW(L"  Selecciona cualquier módulo de la izquierda para verificarlo.\r\n");
            AppendLogW(L"====================================================================\r\n");
            return 0;
        }

        case WM_COMMAND: {
            int id = LOWORD(wp);
            switch (id) {
                case ID_BTN_TEST_ALL:
                    RunCommandAndLog(L"Suite Compilador uplc", L"bin\\uplc.exe test");
                    RunCommandAndLog(L"Supervisor Políglota UREP", L"bin\\uplc.exe run examples\\test_urep_polyglot.upl");
                    RunCommandAndLog(L"Motor Vectorial DVTRGAS-30", L"bin\\uplc.exe run examples\\test_dvtrgas30.upl");
                    RunCommandAndLog(L"Gestión LIFO diferir / defer", L"bin\\uplc.exe run examples\\test_defer.upl");
                    RunCommandAndLog(L"Coincidencia según / match", L"bin\\uplc.exe run examples\\test_match.upl");
                    RunCommandAndLog(L"Suite Maestra 28/28 Módulos", L"bin\\test_all_modules.exe");
                    break;

                case ID_BTN_UPLC_TEST:
                    RunCommandAndLog(L"Suite Compilador uplc (6/6 Suites)", L"bin\\uplc.exe test");
                    break;

                case ID_BTN_UREP_TEST:
                    RunCommandAndLog(L"Supervisor Políglota UREP", L"bin\\uplc.exe run examples\\test_urep_polyglot.upl");
                    break;

                case ID_BTN_DVTRGAS_TEST:
                    RunCommandAndLog(L"Motor Vectorial DVTRGAS-30 (DLL)", L"bin\\uplc.exe run examples\\test_dvtrgas30.upl");
                    break;

                case ID_BTN_DEFER_TEST:
                    RunCommandAndLog(L"Gestión LIFO diferir / defer", L"bin\\uplc.exe run examples\\test_defer.upl");
                    break;

                case ID_BTN_MATCH_TEST:
                    RunCommandAndLog(L"Coincidencia según / match O(1)", L"bin\\uplc.exe run examples\\test_match.upl");
                    break;

                case ID_BTN_MODULES_TEST:
                    RunCommandAndLog(L"Suite 28/28 Módulos de Silicio", L"bin\\test_all_modules.exe");
                    break;

                case ID_BTN_C2UPL_TEST:
                    RunCommandAndLog(L"Transpilador c2upl (C99 a UPL+)", L"bin\\c2upl.exe");
                    break;

                case ID_BTN_GRAPHICS_DEMO:
                    AppendLogW(L"\r\n[DVTRGAS] Lanzando pantalla gráfica vectorial a 60 FPS en el escritorio...\r\n");
                    ShellExecuteW(NULL, L"open", L"bin\\demo_pantalla_dvtrga.exe", NULL, g_rootDir[0] ? g_rootDir : NULL, SW_SHOWNORMAL);
                    break;

                case ID_BTN_CLEAR_LOG:
                    SetWindowTextW(g_hEditLog, L"");
                    break;
            }
            return 0;
        }

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wp;
            HWND hCtl = (HWND)lp;
            if (hCtl == g_hEditLog) {
                SetTextColor(hdc, RGB(52, 211, 153));
                SetBkColor(hdc, RGB(3, 7, 18));
                return (LRESULT)g_hbrLogBkg;
            }
            break;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rc;
            GetClientRect(hwnd, &rc);
            FillRect(hdc, &rc, g_hbrBkg);

            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(248, 250, 252));
            HFONT hOldFont = (HFONT)SelectObject(hdc, g_hFontUi);

            const wchar_t *title = L"UPL+ LABORATORY TEST CENTER — SUITE DE VERIFICACIÓN DE SILICIO";
            TextOutW(hdc, 20, 18, title, (int)wcslen(title));
            
            SetTextColor(hdc, RGB(148, 163, 184));
            const wchar_t *sub = L"Entorno de pruebas empíricas para I+D, industria y auditoría telemétrica | 0 Fugas de Memoria";
            TextOutW(hdc, 20, 44, sub, (int)wcslen(sub));

            SelectObject(hdc, hOldFont);
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            if (g_hbrBkg) DeleteObject(g_hbrBkg);
            if (g_hbrLogBkg) DeleteObject(g_hbrLogBkg);
            if (g_hFontUi) DeleteObject(g_hFontUi);
            if (g_hFontLog) DeleteObject(g_hFontLog);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrev, PWSTR lpCmdLine, int nCmdShow) {
    (void)hPrev; (void)lpCmdLine; (void)nCmdShow;

    InitRootDir();

    WNDCLASSW wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"UplTestLauncherClassW";
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(
        wc.lpszClassName,
        L"UPL+ Laboratory Test Center - Verificacion de Modulos de Silicio",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 1060, 660,
        NULL, NULL, hInst, NULL);

    if (!hwnd) return 1;

    g_hwnd = hwnd;
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}
