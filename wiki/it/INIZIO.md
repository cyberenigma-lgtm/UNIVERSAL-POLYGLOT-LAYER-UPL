# 🚀 GUIDA RAPIDA: LINGUAGGIO DI PROGRAMMAZIONE UPL+
## Linguaggio di Programmazione di Sistema e Compilatore Nativo C99

Benvenuti nella documentazione ufficiale di **UPL+ (Universal Polyglot Layer Plus)**. UPL+ è un **linguaggio di programmazione di sistema compilato, fortemente tipizzato e autonomo**, progettato per garantire prestazioni estreme, interoperabilità diretta a livello di silicio e aritmetica decimale esatta.

---

### 🌟 Pilastri Architetturali di UPL+

1. **Compilatore Nativo Indipendente (`uplc`):** Sviluppato interamente in standard C99 ANSI (senza dipendenze esterne, runtime pesanti o garbage collector).
2. **Sintassi Multilingue Naturale:** Supporto per parole chiave bilingui e internazionali unificate direttamente nell'AST.
3. **Aritmetica Decimale Esatta (`comp3`):** Numeri BCD finanziari con zero errori di arrotondamento virgola mobile.
4. **Integrazione Basso Livello (`embed` e `asm`):** Iniezione diretta di codice C99 e istruzioni x86_64 senza perdite di cicli di clock.
5. **Memoria ad Arena ad Alte Prestazioni:** Gestione deterministica in blocchi da 64 KB con rilascio istantaneo $O(1)$.

---

### 📦 Il Tuo Primo Programma in UPL+

```upl
modulo esempio;

pubblico funzione main() -> i32 {
    sea mut totale: i32 = 0;
    sea base: i32 = 1000;
    sea aliquota: i32 = 22;

    totale = base + (base * aliquota / 100);
    ritornar 0;
}
```

---

### ⚡ Compilazione ed Esecuzione

```bash
# Compilazione ed esecuzione immediata:
bin/uplc.exe run mio_programma.upl

# Compilazione ad eseguibile nativo:
bin/uplc.exe build mio_programma.upl -o bin/mio_programma.exe
```

---
*Universal Polyglot Layer (UPL) — Proprietà Intellettuale Registrata su SafeCreative.*
