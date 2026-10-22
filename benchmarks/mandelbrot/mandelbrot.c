// === BENCHMARK DE RENDIMIENTO: C99 ESTANDAR (GCC -O2) ===
#include <stdio.h>
#include <stdint.h>
#include <windows.h>

static inline double get_time_ms(void) {
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart * 1000.0 / (double)freq.QuadPart;
}

int calcular_mandelbrot(int ancho, int alto, int max_iter) {
    int dentro = 0;
    for (int py = 0; py < alto; ++py) {
        double y0 = ((double)py * 2.0 / (double)alto) - 1.0;
        for (int px = 0; px < ancho; ++px) {
            double x0 = ((double)px * 3.5 / (double)ancho) - 2.5;
            double x = 0.0;
            double y = 0.0;
            int iter = 0;

            while ((x * x + y * y <= 4.0) && (iter < max_iter)) {
                double xtemp = x * x - y * y + x0;
                y = 2.0 * x * y + y0;
                x = xtemp;
                iter++;
            }

            if (iter == max_iter) {
                dentro++;
            }
        }
    }
    return dentro;
}

int main(void) {
    int w = 1000;
    int h = 1000;
    int max_iter = 500;

    printf("=== BENCHMARK DE RENDIMIENTO: C99 NATIVO (1000x1000, 500 iter) ===\n");

    double t0 = get_time_ms();
    int count = calcular_mandelbrot(w, h, max_iter);
    double t1 = get_time_ms();

    double ms = t1 - t0;
    printf("[C99 ESTANDAR] Puntos convergentes: %d\n", count);
    printf("[C99 ESTANDAR] Tiempo de computo: %.2f ms (%.3f segundos)\n", ms, ms / 1000.0);

    return 0;
}
