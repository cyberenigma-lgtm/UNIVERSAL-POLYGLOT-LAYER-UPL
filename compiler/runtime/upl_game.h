/*
 * UPL+ Game Engine Runtime (upl_game.h)
 * Motor Nativo de Videojuegos 2D/3D de Alto Rendimiento para UPL+
 * 
 * Copyright (C) 2026 Sovereign Systems & Neuro-OS
 * Zero-CRT / Freestanding Silicon Graphics Pipeline
 */

#ifndef UPL_GAME_H
#define UPL_GAME_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- [MATEMÁTICA VECTORIAL NATIVA] ---
typedef struct { float x, y; } upl_vec2_t;
typedef struct { float x, y, z; } upl_vec3_t;
typedef struct { float r, g, b, a; } upl_color_t;

static inline upl_vec3_t upl_vec3(float x, float y, float z) { return (upl_vec3_t){ x, y, z }; }
static inline upl_vec3_t upl_vec3_add(upl_vec3_t a, upl_vec3_t b) { return (upl_vec3_t){ a.x + b.x, a.y + b.y, a.z + b.z }; }
static inline upl_vec3_t upl_vec3_sub(upl_vec3_t a, upl_vec3_t b) { return (upl_vec3_t){ a.x - b.x, a.y - b.y, a.z - b.z }; }
static inline upl_vec3_t upl_vec3_scale(upl_vec3_t a, float s) { return (upl_vec3_t){ a.x * s, a.y * s, a.z * s }; }
static inline float upl_vec3_dot(upl_vec3_t a, upl_vec3_t b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline float upl_vec3_len(upl_vec3_t a) { return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z); }
static inline upl_vec3_t upl_vec3_norm(upl_vec3_t a) {
    float l = upl_vec3_len(a);
    if (l < 0.0001f) return (upl_vec3_t){ 0, 0, 0 };
    return (upl_vec3_t){ a.x / l, a.y / l, a.z / l };
}

// --- [CONTEXTO GLOBAL DEL MOTOR] ---
typedef struct {
    int width;
    int height;
    bool running;
    bool keys[256];
    bool keys_pressed[256];
    int mouse_x;
    int mouse_y;
    int mouse_dx;
    int mouse_dy;
    bool mouse_left;
    bool mouse_right;
    bool mouse_locked;
    float delta_time;
    uint32_t last_tick;
    int fps;
    int frame_count;
    uint32_t fps_timer;

#ifdef _WIN32
    HWND hwnd;
    HDC hdc;
    HDC mem_dc;
    HBITMAP mem_bmp;
    uint32_t *pixels;
#endif
} upl_game_ctx_t;

static upl_game_ctx_t g_upl_game = {0};

// --- [CÁMARA 3D] ---
typedef struct {
    upl_vec3_t pos;
    float yaw;
    float pitch;
    float fov;
} upl_camera_t;

// --- [POLÍGONO 3D CON ORDEN Z] ---
typedef struct {
    POINT pts[4];
    int count;
    float depth;
    uint32_t color;
} upl_poly3d_t;

#define UPL_MAX_POLYS 8000
static upl_poly3d_t g_upl_polys[UPL_MAX_POLYS];
static int g_upl_poly_count = 0;

static int upl_poly_depth_cmp(const void *a, const void *b) {
    const upl_poly3d_t *pa = (const upl_poly3d_t*)a;
    const upl_poly3d_t *pb = (const upl_poly3d_t*)b;
    if (pb->depth > pa->depth) return 1;
    if (pb->depth < pa->depth) return -1;
    return 0;
}

#ifdef _WIN32
static LRESULT CALLBACK upl_game_wndproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_KEYDOWN:
            if (wParam < 256) {
                if (!g_upl_game.keys[wParam]) g_upl_game.keys_pressed[wParam] = true;
                g_upl_game.keys[wParam] = true;
            }
            if (wParam == VK_ESCAPE) {
                g_upl_game.mouse_locked = !g_upl_game.mouse_locked;
                ShowCursor(!g_upl_game.mouse_locked);
            }
            return 0;

        case WM_KEYUP:
            if (wParam < 256) g_upl_game.keys[wParam] = false;
            return 0;

        case WM_LBUTTONDOWN:
            g_upl_game.mouse_left = true;
            if (!g_upl_game.mouse_locked) {
                g_upl_game.mouse_locked = true;
                ShowCursor(FALSE);
            }
            return 0;

        case WM_LBUTTONUP:
            g_upl_game.mouse_left = false;
            return 0;

        case WM_RBUTTONDOWN:
            g_upl_game.mouse_right = true;
            return 0;

        case WM_RBUTTONUP:
            g_upl_game.mouse_right = false;
            return 0;

        case WM_MOUSEMOVE: {
            int mx = LOWORD(lParam);
            int my = HIWORD(lParam);

            if (g_upl_game.mouse_locked) {
                int cx = g_upl_game.width / 2;
                int cy = g_upl_game.height / 2;
                g_upl_game.mouse_dx = mx - cx;
                g_upl_game.mouse_dy = my - cy;

                if (abs(g_upl_game.mouse_dx) > 0 || abs(g_upl_game.mouse_dy) > 0) {
                    POINT pt = { cx, cy };
                    ClientToScreen(hwnd, &pt);
                    SetCursorPos(pt.x, pt.y);
                }
            } else {
                g_upl_game.mouse_dx = mx - g_upl_game.mouse_x;
                g_upl_game.mouse_dy = my - g_upl_game.mouse_y;
            }
            g_upl_game.mouse_x = mx;
            g_upl_game.mouse_y = my;
            return 0;
        }

        case WM_CLOSE:
        case WM_DESTROY:
            g_upl_game.running = false;
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}
#endif

// --- [INICIALIZACIÓN DE MOTOR] ---
static inline bool upl_game_init(const char *title, int width, int height) {
    g_upl_game.width = width;
    g_upl_game.height = height;
    g_upl_game.running = true;
    g_upl_game.mouse_locked = true;
    g_upl_game.last_tick = GetTickCount();
    g_upl_game.fps_timer = GetTickCount();

#ifdef _WIN32
    WNDCLASSEXA wc = {0};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.lpfnWndProc = upl_game_wndproc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "UPL_Game_Window";
    RegisterClassExA(&wc);

    DWORD style = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;
    RECT rc = { 0, 0, width, height };
    AdjustWindowRect(&rc, style, FALSE);

    g_upl_game.hwnd = CreateWindowExA(
        0, "UPL_Game_Window", title, style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        NULL, NULL, wc.hInstance, NULL
    );

    if (!g_upl_game.hwnd) return false;

    ShowWindow(g_upl_game.hwnd, SW_SHOW);
    UpdateWindow(g_upl_game.hwnd);

    g_upl_game.hdc = GetDC(g_upl_game.hwnd);
    g_upl_game.mem_dc = CreateCompatibleDC(g_upl_game.hdc);
    g_upl_game.mem_bmp = CreateCompatibleBitmap(g_upl_game.hdc, width, height);
    SelectObject(g_upl_game.mem_dc, g_upl_game.mem_bmp);

    ShowCursor(FALSE);
    POINT pt = { width / 2, height / 2 };
    ClientToScreen(g_upl_game.hwnd, &pt);
    SetCursorPos(pt.x, pt.y);
#endif
    return true;
}

// --- [ACTUALIZACIÓN DE EVENTOS Y RELOJ] ---
static inline bool upl_game_poll_events(void) {
#ifdef _WIN32
    memset(g_upl_game.keys_pressed, 0, sizeof(g_upl_game.keys_pressed));
    g_upl_game.mouse_dx = 0;
    g_upl_game.mouse_dy = 0;

    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            g_upl_game.running = false;
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    uint32_t now = GetTickCount();
    g_upl_game.delta_time = (float)(now - g_upl_game.last_tick) / 1000.0f;
    if (g_upl_game.delta_time > 0.1f) g_upl_game.delta_time = 0.1f;
    g_upl_game.last_tick = now;

    g_upl_game.frame_count++;
    if (now - g_upl_game.fps_timer >= 1000) {
        g_upl_game.fps = g_upl_game.frame_count;
        g_upl_game.frame_count = 0;
        g_upl_game.fps_timer = now;
    }
#endif
    return g_upl_game.running;
}

// --- [LIMPIEZA Y PRESENTACIÓN DE PANTALLA] ---
static inline void upl_game_clear(uint32_t color) {
#ifdef _WIN32
    HBRUSH br = CreateSolidBrush(color);
    RECT rc = { 0, 0, g_upl_game.width, g_upl_game.height };
    FillRect(g_upl_game.mem_dc, &rc, br);
    DeleteObject(br);
    g_upl_poly_count = 0;
#endif
}

static inline void upl_game_present(void) {
#ifdef _WIN32
    // Ordenar y dibujar polígonos 3D
    if (g_upl_poly_count > 0) {
        qsort(g_upl_polys, g_upl_poly_count, sizeof(upl_poly3d_t), upl_poly_depth_cmp);
        SelectObject(g_upl_game.mem_dc, GetStockObject(NULL_PEN));
        for (int i = 0; i < g_upl_poly_count; i++) {
            upl_poly3d_t *p = &g_upl_polys[i];
            HBRUSH br = CreateSolidBrush(p->color);
            HGDIOBJ old = SelectObject(g_upl_game.mem_dc, br);
            Polygon(g_upl_game.mem_dc, p->pts, p->count);
            SelectObject(g_upl_game.mem_dc, old);
            DeleteObject(br);
        }
    }

    BitBlt(g_upl_game.hdc, 0, 0, g_upl_game.width, g_upl_game.height, g_upl_game.mem_dc, 0, 0, SRCCOPY);
    Sleep(1); // Control de CPU
#endif
}

// --- [PRIMITIVAS 2D Y TEXTO] ---
static inline void upl_draw_rect(int x, int y, int w, int h, uint32_t color) {
#ifdef _WIN32
    HBRUSH br = CreateSolidBrush(color);
    RECT rc = { x, y, x + w, y + h };
    FillRect(g_upl_game.mem_dc, &rc, br);
    DeleteObject(br);
#endif
}

static inline void upl_draw_text(int x, int y, const char *text, uint32_t color) {
#ifdef _WIN32
    SetBkMode(g_upl_game.mem_dc, TRANSPARENT);
    SetTextColor(g_upl_game.mem_dc, color);
    TextOutA(g_upl_game.mem_dc, x, y, text, (int)strlen(text));
#endif
}

// --- [MOTOR 3D Y VÓXELES] ---
static inline bool upl_project_point(upl_vec3_t v, upl_camera_t cam, POINT *out_pt, float *out_depth) {
    float dx = v.x - cam.pos.x;
    float dy = v.y - cam.pos.y;
    float dz = v.z - cam.pos.z;

    float cos_yaw = cosf(-cam.yaw);
    float sin_yaw = sinf(-cam.yaw);
    float x1 = dx * cos_yaw - dz * sin_yaw;
    float z1 = dx * sin_yaw + dz * cos_yaw;

    float cos_pitch = cosf(cam.pitch);
    float sin_pitch = sinf(cam.pitch);
    float y2 = dy * cos_pitch - z1 * sin_pitch;
    float z2 = dy * sin_pitch + z1 * cos_pitch;

    if (z2 <= 0.35f) return false;

    float fov = (cam.fov > 10.0f) ? cam.fov : 700.0f;
    out_pt->x = (LONG)((g_upl_game.width / 2) + (x1 * fov / z2));
    out_pt->y = (LONG)((g_upl_game.height / 2) - (y2 * fov / z2));
    *out_depth = z2;
    return true;
}

static inline void upl_add_quad3d(upl_vec3_t v0, upl_vec3_t v1, upl_vec3_t v2, upl_vec3_t v3, uint32_t color, upl_camera_t cam) {
    if (g_upl_poly_count >= UPL_MAX_POLYS) return;
    POINT p0, p1, p2, p3;
    float d0, d1, d2, d3;
    if (!upl_project_point(v0, cam, &p0, &d0)) return;
    if (!upl_project_point(v1, cam, &p1, &d1)) return;
    if (!upl_project_point(v2, cam, &p2, &d2)) return;
    if (!upl_project_point(v3, cam, &p3, &d3)) return;

    upl_poly3d_t *poly = &g_upl_polys[g_upl_poly_count++];
    poly->pts[0] = p0; poly->pts[1] = p1; poly->pts[2] = p2; poly->pts[3] = p3;
    poly->count = 4;
    poly->depth = (d0 + d1 + d2 + d3) * 0.25f;
    poly->color = color;
}

static inline void upl_draw_cube(float x, float y, float z, float w, float h, float d, uint32_t top_col, uint32_t side_col, upl_camera_t cam) {
    float x0 = x - w * 0.5f, x1 = x + w * 0.5f;
    float y0 = y - h * 0.5f, y1 = y + h * 0.5f;
    float z0 = z - d * 0.5f, z1 = z + d * 0.5f;

    uint32_t dark_col = (((side_col >> 16) & 0xFF) * 8 / 10) << 16 |
                        (((side_col >> 8)  & 0xFF) * 8 / 10) << 8  |
                        (((side_col)       & 0xFF) * 8 / 10);

    upl_add_quad3d((upl_vec3_t){x0, y1, z0}, (upl_vec3_t){x1, y1, z0}, (upl_vec3_t){x1, y1, z1}, (upl_vec3_t){x0, y1, z1}, top_col, cam);
    upl_add_quad3d((upl_vec3_t){x0, y0, z1}, (upl_vec3_t){x1, y0, z1}, (upl_vec3_t){x1, y1, z1}, (upl_vec3_t){x0, y1, z1}, side_col, cam);
    upl_add_quad3d((upl_vec3_t){x1, y0, z0}, (upl_vec3_t){x0, y0, z0}, (upl_vec3_t){x0, y1, z0}, (upl_vec3_t){x1, y1, z0}, side_col, cam);
    upl_add_quad3d((upl_vec3_t){x1, y0, z1}, (upl_vec3_t){x1, y0, z0}, (upl_vec3_t){x1, y1, z0}, (upl_vec3_t){x1, y1, z1}, dark_col, cam);
    upl_add_quad3d((upl_vec3_t){x0, y0, z0}, (upl_vec3_t){x0, y0, z1}, (upl_vec3_t){x0, y1, z1}, (upl_vec3_t){x0, y1, z0}, dark_col, cam);
}

// --- [SÍNTESIS DE AUDIO RETRO CHIPTUNE] ---
static inline void upl_play_tone(uint32_t freq_hz, uint32_t duration_ms) {
#ifdef _WIN32
    Beep(freq_hz, duration_ms);
#endif
}

#endif // UPL_GAME_H
