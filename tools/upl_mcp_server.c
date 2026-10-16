/* ============================================================================ */
/* UPL NATIVE MCP SERVER (MODEL CONTEXT PROTOCOL)                              */
/* Language: ANSI C99 / Pure Native Standalone Binary                           */
/* Author: José Manuel Moreno (Noxferion)                                      */
/* ============================================================================ */
/* 100% C99 Nativo - Cero dependencias de Python o runtimes externos.          */
/* ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
  #include <windows.h>
  #define popen _popen
  #define pclose _pclose
#else
  #include <unistd.h>
#endif

#define BUFFER_SIZE 65536

/* Extraer valor simple de un JSON plano sin dependencias externas */
static bool json_extract_string(const char *json, const char *key, char *out_val, size_t max_len) {
    char pattern[128];
    snprintf(pattern, sizeof(pattern), "\"%s\"", key);
    const char *pos = strstr(json, pattern);
    if (!pos) return false;

    pos += strlen(pattern);
    while (*pos == ' ' || *pos == ':' || *pos == '\t') pos++;
    if (*pos != '"') return false;
    pos++;

    size_t i = 0;
    while (*pos && *pos != '"' && i < max_len - 1) {
        if (*pos == '\\' && *(pos + 1)) pos++;
        out_val[i++] = *pos++;
    }
    out_val[i] = '\0';
    return true;
}

static bool json_extract_id(const char *json, char *out_id, size_t max_len) {
    const char *pos = strstr(json, "\"id\"");
    if (!pos) return false;
    pos += 4;
    while (*pos == ' ' || *pos == ':' || *pos == '\t') pos++;

    size_t i = 0;
    if (*pos == '"') {
        pos++;
        while (*pos && *pos != '"' && i < max_len - 1) {
            out_id[i++] = *pos++;
        }
    } else {
        while ((*pos >= '0' && *pos <= '9') && i < max_len - 1) {
            out_id[i++] = *pos++;
        }
    }
    out_id[i] = '\0';
    return i > 0;
}

/* Escapar texto para formato JSON */
static void json_escape_print(FILE *out, const char *str) {
    while (*str) {
        switch (*str) {
            case '"':  fputs("\\\"", out); break;
            case '\\': fputs("\\\\", out); break;
            case '\b': fputs("\\b", out); break;
            case '\f': fputs("\\f", out); break;
            case '\n': fputs("\\n", out); break;
            case '\r': fputs("\\r", out); break;
            case '\t': fputs("\\t", out); break;
            default:
                if ((unsigned char)*str < 32) {
                    fprintf(out, "\\u%04x", (unsigned char)*str);
                } else {
                    fputc(*str, out);
                }
                break;
        }
        str++;
    }
}

/* Ejecutar comando y capturar salida */
static int execute_uplc(const char *cmd_line, char *out_buf, size_t max_len) {
    out_buf[0] = '\0';
    FILE *pipe = popen(cmd_line, "r");
    if (!pipe) return -1;

    size_t bytes_read = fread(out_buf, 1, max_len - 1, pipe);
    out_buf[bytes_read] = '\0';
    return pclose(pipe);
}

int main(int argc, char **argv) {
    char line[BUFFER_SIZE];
    char exec_buf[BUFFER_SIZE];

    /* Localizar ejecutable uplc.exe relativo al ejecutable o ruta conocida */
    char uplc_path[512] = "bin\\uplc.exe";
    FILE *fchk = fopen(uplc_path, "r");
    if (fchk) {
        fclose(fchk);
    } else {
        snprintf(uplc_path, sizeof(uplc_path), "uplc.exe");
    }

    while (fgets(line, sizeof(line), stdin)) {
        char method[128] = "";
        char id_str[64] = "null";

        json_extract_string(line, "method", method, sizeof(method));
        json_extract_id(line, id_str, sizeof(id_str));

        if (strcmp(method, "initialize") == 0) {
            printf("{\"jsonrpc\":\"2.0\",\"id\":%s,\"result\":{\"protocolVersion\":\"2024-11-05\",\"capabilities\":{\"tools\":{}},\"serverInfo\":{\"name\":\"upl-native-mcp\",\"version\":\"1.5.0\"}}}\n", id_str);
            fflush(stdout);
            continue;
        }

        if (strcmp(method, "tools/list") == 0) {
            printf("{\"jsonrpc\":\"2.0\",\"id\":%s,\"result\":{\"tools\":[", id_str);
            printf("{\"name\":\"upl_run\",\"description\":\"Compila en memoria y ejecuta un archivo .upl inmediatamente de forma nativa.\",\"inputSchema\":{\"type\":\"object\",\"properties\":{\"file_path\":{\"type\":\"string\",\"description\":\"Ruta al archivo .upl\"}},\"required\":[\"file_path\"]}},");
            printf("{\"name\":\"upl_build\",\"description\":\"Compila un archivo .upl a ejecutable nativo, C99 o ensamblador NASM.\",\"inputSchema\":{\"type\":\"object\",\"properties\":{\"file_path\":{\"type\":\"string\",\"description\":\"Ruta al archivo .upl\"},\"target\":{\"type\":\"string\",\"description\":\"Target: bin, c, asm, deb, rpm, app, apk\",\"default\":\"bin\"},\"output_file\":{\"type\":\"string\",\"description\":\"Ruta del binario de salida\"}},\"required\":[\"file_path\"]}},");
            printf("{\"name\":\"upl_list_modules\",\"description\":\"Lista todos los modulos de sistemas registrados en el ecosistema UPL.\",\"inputSchema\":{\"type\":\"object\",\"properties\":{}}}");
            printf("]}}\n");
            fflush(stdout);
            continue;
        }

        if (strcmp(method, "tools/call") == 0) {
            char tool_name[128] = "";
            char file_path[512] = "";
            char target[64] = "bin";
            char output_file[512] = "";

            json_extract_string(line, "name", tool_name, sizeof(tool_name));
            json_extract_string(line, "file_path", file_path, sizeof(file_path));
            json_extract_string(line, "target", target, sizeof(target));
            json_extract_string(line, "output_file", output_file, sizeof(output_file));

            char cmd[2048] = "";

            if (strcmp(tool_name, "upl_run") == 0) {
                snprintf(cmd, sizeof(cmd), "\"%s\" run \"%s\" 2>&1", uplc_path, file_path);
                int code = execute_uplc(cmd, exec_buf, sizeof(exec_buf));

                printf("{\"jsonrpc\":\"2.0\",\"id\":%s,\"result\":{\"content\":[{\"type\":\"text\",\"text\":\"", id_str);
                json_escape_print(stdout, exec_buf);
                printf("\"}],\"exitCode\":%d}}\n", code);
                fflush(stdout);
            } else if (strcmp(tool_name, "upl_build") == 0) {
                if (strlen(output_file) > 0) {
                    snprintf(cmd, sizeof(cmd), "\"%s\" build \"%s\" -t %s -o \"%s\" 2>&1", uplc_path, file_path, target, output_file);
                } else {
                    snprintf(cmd, sizeof(cmd), "\"%s\" build \"%s\" -t %s 2>&1", uplc_path, file_path, target);
                }
                int code = execute_uplc(cmd, exec_buf, sizeof(exec_buf));

                printf("{\"jsonrpc\":\"2.0\",\"id\":%s,\"result\":{\"content\":[{\"type\":\"text\",\"text\":\"", id_str);
                json_escape_print(stdout, exec_buf);
                printf("\"}],\"exitCode\":%d}}\n", code);
                fflush(stdout);
            } else if (strcmp(tool_name, "upl_list_modules") == 0) {
                snprintf(cmd, sizeof(cmd), "\"%s\" list 2>&1", uplc_path);
                int code = execute_uplc(cmd, exec_buf, sizeof(exec_buf));

                printf("{\"jsonrpc\":\"2.0\",\"id\":%s,\"result\":{\"content\":[{\"type\":\"text\",\"text\":\"", id_str);
                json_escape_print(stdout, exec_buf);
                printf("\"}],\"exitCode\":%d}}\n", code);
                fflush(stdout);
            } else {
                printf("{\"jsonrpc\":\"2.0\",\"id\":%s,\"error\":{\"code\":-32601,\"message\":\"Tool desconocida\"}}\n", id_str);
                fflush(stdout);
            }
            continue;
        }

        /* Ping u otros metodos */
        printf("{\"jsonrpc\":\"2.0\",\"id\":%s,\"result\":{}}\n", id_str);
        fflush(stdout);
    }

    return 0;
}
