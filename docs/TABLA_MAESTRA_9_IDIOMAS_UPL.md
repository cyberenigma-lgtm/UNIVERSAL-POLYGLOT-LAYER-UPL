# TABLA MAESTRA UNIVERSAL DE PALABRAS CLAVE EN 9 IDIOMAS (UPL+)
# ============================================================================
# Idiomas Soportados: Castellano | Inglés | Francés | Italiano | Chino | Japonés | Ruso | Coreano | Hindi
# Arquitectura: MultiLang-ASM & UPL Polyglot Lexer Standard
# ============================================================================

Esta tabla define la equivalencia formal de las directivas y palabras clave de UPL+ en los 9 idiomas más hablados del planeta. El compilador `uplc` normaliza todas estas entradas al mismo nodo de sintaxis abstracta (AST) sin ningún coste en tiempo de ejecución.

---

## 1. Estructuras de Control y Definición de Funciones

| Token Canónico | Castellano | English | Français | Italiano | Chino (中文) | Japonés (日本語) | Ruso (Русский) | Coreano (한국어) | Hindi (हिन्दी) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `TOKEN_MODULE` | `modulo` | `module` | `module` | `modulo` | `模块` | `モジュール` | `модуль` | `모듈` | `मॉड्यूल` |
| `TOKEN_FN` | `funcion` | `fn` | `fonction` | `funzione` | `函数` | `関数` | `функция` | `함수` | `फ़ंक्शन` |
| `TOKEN_PUB` | `publico` | `pub` | `public` | `pubblico` | `公开` | `公開` | `публичный` | `공개` | `सार्वजनिक` |
| `TOKEN_LET` | `sea` | `let` | `soit` | `sia` | `设` | `変数` | `пусть` | `선언` | `मान` |
| `TOKEN_MUT` | `mut` | `mut` | `mut` | `mut` | `可变` | `変更可能` | `изменяемый` | `가변` | `परिवर्तनशील` |
| `TOKEN_IF` | `si` | `if` | `si` | `se` | `如果` | `もし` | `если` | `만약` | `यदि` |
| `TOKEN_ELSE` | `sino` | `else` | `sinon` | `altrimenti` | `否则` | `そうでなければ` | `иначе` | `아니면` | `अन्यथा` |
| `TOKEN_WHILE` | `mientras` | `while` | `tantque` | `mentre` | `当` | `間` | `пока` | `동안` | `जबतक` |
| `TOKEN_RETURN` | `retornar` | `return` | `retourner` | `ritorna` | `返回` | `戻る` | `вернуть` | `반환` | `वापसी` |
| `TOKEN_STRUCT` | `estructura` | `struct` | `structure` | `struttura` | `结构` | `構造体` | `структура` | `구조체` | `संरचना` |
| `TOKEN_ASM` | `ensamblador` | `asm` | `assembleur` | `assembler` | `汇编` | `アセンブラ` | `ассемблер` | `어셈블러` | `असेंबलर` |
| `TOKEN_EMBED` | `incrustar` | `embed` | `integrer` | `incorpora` | `嵌入` | `埋め込み` | `встроить` | `임베드` | `एम्बेड` |

---

## 2. Instrucciones de Ensamblador de Silicio (MultiLang-ASM)

Equivalencia de mnemónicos de bajo nivel x86_64 para bloques `asm { ... }` en los 9 idiomas:

| x86_64 Canónico | Castellano | English | Français | Italiano | Chino (中文) | Japonés (日本語) | Ruso (Русский) | Coreano (한국어) | Hindi (हिन्दी) |
| :---: | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `MOV` | `mover` | `mov` | `deplacer` | `sposta` | `移动` | `移動` | `переместить` | `이동` | `स्थानांतरण` |
| `ADD` | `sumar` | `add` | `ajouter` | `aggiungi` | `加` | `加算` | `сложить` | `더하기` | `जोड़ना` |
| `SUB` | `restar` | `sub` | `soustraire` | `sottrai` | `减` | `減算` | `вычесть` | `빼기` | `घटाना` |
| `CMP` | `comparar` | `cmp` | `comparer` | `confronta` | `比较` | `比較` | `сравнить` | `비교` | `तुलना` |
| `JMP` | `saltar` | `jmp` | `sauter` | `salta` | `跳转` | `ジャンプ` | `перейти` | `점프` | `कूदना` |
| `CALL` | `llamar` | `call` | `appeler` | `chiama` | `调用` | `呼出` | `вызвать` | `호출` | `बुलाना` |
| `RET` | `retorno` | `ret` | `retour` | `ritorno` | `返回` | `リターン` | `возврат` | `복귀` | `वापस` |
| `PUSH` | `apilar` | `push` | `empiler` | `spingi` | `压栈` | `プッシュ` | `втолкнуть` | `밀기` | `धकेलना` |
| `POP` | `desapilar` | `pop` | `depiler` | `estrai` | `出栈` | `ポップ` | `извлечь` | `당기기` | `निकालना` |
| `NOP` | `nop` | `nop` | `nop` | `nop` | `空操作` | `無操作` | `нет_операции` | `무작업` | `कोई_काम_नहीं` |

---

## 3. Ejemplo Práctico de Código Universal Políglota

El siguiente código es 100% equivalente en compilación:

### Opción A (Castellano):
```upl
modulo matematicas;

publico funcion calcular_doble(x: i32) -> i32 {
    si x > 0 {
        retornar x * 2;
    }
    retornar 0;
}
```

### Opción B (Français):
```upl
module mathematiques;

public fonction calculer_double(x: i32) -> i32 {
    si x > 0 {
        retourner x * 2;
    }
    retourner 0;
}
```

### Opción C (Italiano):
```upl
modulo matematica;

pubblico funzione calcola_doppio(x: i32) -> i32 {
    se x > 0 {
        ritorna x * 2;
    }
    ritorna 0;
}
```

---
*UPL+ es el primer lenguaje del mundo donde el código habla la lengua materna del ser humano sin perder ni un solo ciclo de reloj de la máquina.*
