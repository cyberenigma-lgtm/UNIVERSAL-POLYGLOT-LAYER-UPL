# Suite de Pruebas y Arnés de Verificación Formal: UPL+
## Validación Automatizada de Integridad del Compilador y Runtime C99

**Autor:** José Manuel Moreno (Noxferion)  
**Script Principal de Ejecución:** [`test.bat`](../test.bat)  
**Directorio de Tests:** [`tests/`](../tests/)  
**Estado:** 6/6 Suites Pasando (0 Fallos)

---

## 1. Filosofía de Pruebas

Para garantizar que el compilador UPL+ es determinista, robusto y apto para sistemas de misión crítica, el repositorio implementa una suite de verificación continua en C99 puro.

Cada capa del compilador se prueba de forma aislada (pruebas unitarias) y en conjunto (prueba de integración end-to-end), verificando:
1. Conformidad con la especificación formal EBNF.
2. Inmunidad a desbordamientos y gestión de memoria mediante arena.
3. Precisión matemática decimal exacta con 0 errores de redondeo.
4. Protección estricta contra divagaciones o respuestas narrativas de LLMs.
5. Transpilación nativa y fiel de código de bajo nivel C99 a UPL+.

---

## 2. Ejecución de la Suite

Para ejecutar la verificación completa:

```bash
# En Windows (CMD o PowerShell desde la raíz del repositorio):
test.bat

# En Linux / macOS (Bash desde la raíz del repositorio):
./test.sh
```

---

## 3. Detalle de las 6 Suites de Verificación

### Suite 1: Analizador Léxico Formal (`tests/test_lexer.c`)
Valida el escáner de caracteres y el tokenizer bilingüe:
* **Test 1.1 - Unificación Bilingüe:** Verifica que pares de palabras clave (`fn`/`funcion`, `let`/`sea`, `return`/`retornar`, etc.) emitan exactamente el mismo token numérico `TokenType`.
* **Test 1.2 - Tracking de Línea y Columna:** Verifica la posición precisa de tokens en archivos multilínea y el filtrado correcto de comentarios de línea (`//`) y bloque (`/* ... */`).
* **Test 1.3 - Literales Numéricos y Cadenas:** Valida enteros decimales, hexadecimales (`0x...`), binarios (`0b...`), números flotantes y cadenas con secuencias de escape (`\n`, `\t`, `\"`, `\\`).
* **Test 1.4 - Operadores Compuestos:** Verifica operadores de dos caracteres (`==`, `!=`, `<=`, `>=`, `->`, `&&`, `||`).
* **Test 1.5 - Aislamiento de Bloques `embed`:** Verifica que los bloques `embed "c" { ... }` capturen el código C crudo intacto preservando directivas `#include`.
* **Test 1.6 - Detección de Errores Léxicos:** Verifica el manejo elegante de literales no terminados (cadenas sin comilla de cierre).

### Suite 2: Analizador Sintáctico y AST (`tests/test_parser.c`)
Valida la construcción del Árbol de Sintaxis Abstracta y el parser recursivo / Pratt:
* **Test 2.1 - Parseo de Funciones Bilingües:** Valida la firma de funciones con tipos de retorno (`-> i32`) y argumentos tipados en inglés y español.
* **Test 2.2 - Parseo de Estructuras y Tipos COMP-3:** Valida la sintaxis de structs agregados y tipos financieros con precisión y escala parametrizadas (`comp3(15, 2)`).
* **Test 2.3 - Precedencia de Operadores (Pratt Parsing):** Verifica que `2 + 3 * 4` se construya como `(2 + (3 * 4))` y no como `((2 + 3) * 4)`.
* **Test 2.4 - Control de Flujo:** Valida la generación correcta de ramas `if/else`, condiciones de bucles `while` y asignaciones de variables mutables.
* **Test 2.5 - Preservación de Bloques Nativos:** Valida que el nodo `AST_EMBED_BLOCK` conserve fielmente el código inyectado.
* **Test 2.6 - Localización de Errores Sintácticos:** Verifica que ante una sentencia inválida (`let x i32;` sin dos puntos) el parser reporte la línea y columna exactas del fallo.

### Suite 3: Motor Matemático Decimal COMP-3 BCD (`tests/test_comp3.c`)
Valida el subsistema de aritmética empaquetada en software:
* **Test 3.1 - Codificación BCD de Nibbles:** Verifica la compactación de 2 dígitos por byte y la inclusión del nibble de signo (`0x0C` para positivo, `0x0D` para negativo).
* **Test 3.2 - Conversión Bidireccional:** Valida que una cadena `"1234.56"` se codifique en bytes BCD y se decodifique de vuelta a la cadena exacta sin pérdida de dígitos.
* **Test 3.3 - Aritmética sin Deriva IEEE-754:** Valida operaciones de suma y resta decimal (`0.1 + 0.2 = 0.3000...`) donde la aritmética binaria tradicional produce `0.30000000000000004`.
* **Test 3.4 - Comparaciones Algebraicas:** Valida operadores de comparación (`<`, `==`, `>`) teniendo en cuenta signos y escalas.

### Suite 4: Guardián Técnico y Supervisor Antidivagación (`tools/upl_supervisor.c`)
Valida el filtro determinista contra alucinaciones o jerga narrativa de modelos de lenguaje:
* **Test 4.1 - Filtrado de Jerga:** Verifica la eliminación de párrafos introductorios o conclusiones hiperbólicas, conservando exclusivamente bloques de código limpios.
* **Test 4.2 - Intercepción de Divagación:** Bloquea y emite código de salida no-cero cuando una respuesta contiene texto grandilocuente sin ningún bloque de código válido.
* **Test 4.3 - Preservación de Código Técnico:** Verifica que código fuente C99 o UPL válido no sufra alteraciones en su sintaxis al pasar por el supervisor.

### Suite 5: Compilación End-to-End con `uplc`
Prueba de integración completa del pipeline:
1. Toma el archivo fuente de prueba [`examples/test_compiler_real.upl`](../examples/test_compiler_real.upl).
2. El driver `bin/uplc.exe` lo compila a `bin/test_compiler_real.exe` utilizando el generador C99 y GCC.
3. Se ejecuta el binario nativo generado.
4. El programa calcula una base de 1000 EUR más un 21% de IVA (210 EUR), resultando exactamente en 1210 EUR.
5. El código de retorno `0` confirma la operatividad del pipeline de inicio a fin.

### Suite 6: Transpilador Nativo C99 a UPL+ (`tools/c2upl.c`)
Valida la conversión estricta y sin pérdidas de bases de código C99 hacia sintaxis nativa UPL+:
* **Test 6.1 - Mapeo Estricto de Tipos C99:** Verifica la conversión unívoca de tipos elementales (`int32_t` -> `i32`, `uint64_t` -> `u64`, `void*` -> `*void`, etc.).
* **Test 6.2 - Funciones y Firmas en Inglés:** Verifica la emisión de firmas `pub fn` y sentencias `return`.
* **Test 6.3 - Funciones y Firmas en Español:** Verifica la emisión bilingüe con `-l es` (`pub funcion` y `retornar`).
* **Test 6.4 - Structs y Arrays:** Transpila declaraciones `typedef struct { ... }` con miembros de array de longitud fija a `pub struct` de UPL+.
* **Test 6.5 - Mutabilidad y Control de Flujo:** Convierte variables locales a `let mut` / `sea mut` y preserva condiciones de bifurcación (`if`, `while`).

---

## 4. Registro de Salida de Referencia

```
====================================================================
  SUITE MAESTRA DE VERIFICACION INTEGRAL: LENGUAJE UPL+ (C99)
====================================================================
[1/6] Verificando Analizador Lexico Formal (Lexer)...
====================================================================
  UPL+ SPEC SUITE: VERIFICACION FORMAL DEL ANALIZADOR LEXICO C99   
====================================================================
  [PASS] 1. Unificacion bilingue de palabras clave (EN/ES)
  [PASS] 2. Tracking de lineas, columnas y comentarios (linea y bloque)
  [PASS] 3. Literales numericos (enteros, hex 0x, bin 0b, flotantes) y strings
  [PASS] 4. Operadores multicaracter y delimitadores EBNF (==, !=, <=, >=, ->, &&, ||, !, [, ], .)
  [PASS] 5. Aislamiento y captura de bloques de codigo crudo (embed / incrustar)
  [PASS] 6. Deteccion rigurosa de errores lexicos (string sin cerrar)
====================================================================
  RESULTADO: 6/6 SUITES COMPLETADAS SATISFACTORIAMENTE (0 FALLOS)   
====================================================================
[2/6] Verificando Analizador Sintactico y AST (Parser)...
====================================================================
  UPL+ SPEC SUITE: VERIFICACION FORMAL DEL ANALIZADOR SINTACTICO   
====================================================================
  [PASS] 1. Parseo formal de funciones bilingues (fn/funcion, return/retornar)
  [PASS] 2. Parseo de estructuras y tipos financieros comp3(precision, escala)
  [PASS] 3. Precedencia matematica de operadores (Dragon Book Pratt Parsing)
  [PASS] 4. Estructuras de control de flujo (si/sino, mutabilidad)
  [PASS] 5. Captura y preservacion de bloques C99 nativos (embed/incrustar)
  [PASS] 6. Deteccion y localizacion precisa de errores sintacticos (L:C)
====================================================================
  RESULTADO: 6/6 SUITES PARSER COMPLETADAS CON EXITO (0 FALLOS)     
====================================================================
[3/6] Verificando Motor Matematico Decimal Exacto (COMP-3 BCD)...
====================================================================
  UPL+ SPEC SUITE: MOTOR MATEMATICO EXACTO COMP-3 (PACKED DECIMAL) 
====================================================================
  [PASS] 1. Codificacion binaria BCD de nibbles y signos (+0x0C / -0x0D)
  [PASS] 2. Conversion bidireccional texto decimal <-> binario comp3
  [PASS] 3. Aritmetica decimal por software con 0 errores de redondeo IEEE-754
  [PASS] 4. Comparaciones algebraicas deterministas con signos (<, ==, >)
====================================================================
  RESULTADO: 4/4 SUITES COMP-3 COMPLETADAS CON EXITO (0 FALLOS)     
====================================================================
[4/6] Verificando Supervisor y Guardian Nativo contra Divagaciones...
=== INICIANDO AUDITORIA DEL MODULO GUARDIAN NATIVO C99/UPL ===
  [PASS] 1. Filtrado de morralla hiperbolica y extraccion de codigo limpio
  [PASS] 2. Intercepcion y bloqueo de divagacion absoluta sin codigo
  [PASS] 3. Preservacion intacta de codigo tecnico verificado
=== MONITOR DE CONTROL NATIVO: OPERATIVO (0 ERRORES) ===
[5/6] Verificando Compilacion Binaria End-to-End con uplc...
[OK] Compilacion completada con exito: bin/test_compiler_real.exe
[UPL+ BINARY EXECUTABLE] Subtotal: 1000 EUR, IVA (21%): 210 EUR
[UPL+ BINARY EXECUTABLE] Total Calculado: 1210 EUR
[UPL+ BINARY EXECUTABLE] Verificacion de pipeline Dragon Book: EXITOSA (0 ERRORES)
[6/6] Verificando Transpilador C99 a UPL+ (c2upl)...
====================================================================
  SUITE DE PRUEBAS: TRANSPILADOR NATIVO C99 A UPL+ (c2upl.c)        
====================================================================
  [PASS] 1. Mapeo estricto de tipos C99 (int32_t->i32, uint64_t->u64, void*->*void)
  [PASS] 2. Transpilacion de funciones y firmas con dialecto ingles (fn / return)
  [PASS] 3. Transpilacion de funciones y firmas con dialecto espanol (funcion / retornar)
  [PASS] 4. Transpilacion de structs agregados (typedef struct -> pub struct)
  [PASS] 5. Transpilacion de variables mutables (let mut) y control condicional (if)
====================================================================
  RESULTADO: 5/5 PRUEBAS c2upl SUPERADAS SATISFACTORIAMENTE (0 FALLOS)
====================================================================
====================================================================
  TODAS LAS PRUEBAS (6/6) SE COMPLETARON CON EXITO (0 FALLOS)
====================================================================
```
