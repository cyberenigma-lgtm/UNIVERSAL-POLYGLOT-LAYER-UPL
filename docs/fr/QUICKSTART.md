# UPL+ Démarrage Rapide en 30 Secondes

> **Langue / Language:** 🇫🇷 Français | [🇬🇧 English](../en/QUICKSTART.md) | [🇪🇸 Español](../GUIA_OFICIAL_SINTAXIS_Y_COMANDOS_UPL.md) | [🇨🇳 中文](../zh/QUICKSTART.md) | [🇵🇹 Português](../pt/QUICKSTART.md)

**Auteur :** José Manuel Moreno (Noxferion)  
**Toolchain :** UPL+ v2.3.0 — Compilateur ANSI C99 sans dépendance  
**Prérequis :** `gcc` (MinGW-w64 sur Windows, `gcc`/`clang` système sur Linux/macOS)

---

## 1. Installation

**Windows (PowerShell) :**
```powershell
Invoke-WebRequest -Uri https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL/releases/download/v2.3.0/uplc-v2.3.0-windows-x64.zip -OutFile uplc.zip
Expand-Archive uplc.zip -DestinationPath uplc
```

**Linux / macOS (compilation depuis les sources) :**
```bash
git clone https://github.com/cyberenigma-lgtm/UNIVERSAL-POLYGLOT-LAYER-UPL.git
cd UNIVERSAL-POLYGLOT-LAYER-UPL
bash upl.sh
export PATH="$PWD/bin:$PATH"
```

---

## 2. Premier Programme (Télémétrie Noyau Zéro Fuite)

Enregistrez le fichier suivant sous `examples/quickstart_30s.upl` :

```upl
// quickstart_30s.upl — Démo de télémétrie noyau zéro fuite UPL+
// Syntaxe bilingue : les mots-clés anglais et espagnols sont équivalents

module quickstart

import std/io
import std/kernel_telemetry

fn main() -> i64 {
    let handle = open_file("test.txt")
    kernel_telemetry::audit_handle(handle, "pre-read")

    let data = read_bytes(handle, 128)
    io::println("Octets lus : ", data.len)

    defer close_file(handle)   // Nettoyage déterministe — pas de GC, pas de fuite
    kernel_telemetry::audit_handle(handle, "post-read")

    return 0
}
```

**Compilation et exécution :**
```bash
uplc build examples/quickstart_30s.upl -o quickstart
./quickstart          # Linux / macOS
quickstart.exe        # Windows
```

**Audit mémoire et handles :**
```bash
uplc audit examples/quickstart_30s.upl
```

---

## 3. Commandes CLI Essentielles

| Commande | Description |
|----------|-------------|
| `uplc build <fichier.upl> -o <sortie>` | Compile `.upl` → binaire natif via C99 |
| `uplc audit <fichier.upl>` | Analyse les handles, la propriété mémoire et les rangs de verrou |
| `uplc audit <fichier.upl> --format sarif` | Export SARIF v2.1.0 pour GitHub Code Scanning |
| `uplc test <fichier.upl>` | Exécute les blocs `test { }` / `prueba { }` inline |
| `uplc --emit-c <fichier.upl>` | Génère le code C99 intermédiaire sans compiler |
| `uplc --dump-ast <fichier.upl>` | Affiche l'Arbre de Syntaxe Abstraite |
| `uplc --version` | Affiche la version de la toolchain |

---

## 4. Aide-Mémoire Syntaxe Bilingue

```upl
// Déclarations de fonctions (anglais / espagnol — les deux valides)
fn greet(name: str) -> str { ... }
funcion saludar(nombre: str) -> str { ... }

// Variables immuables
let x: i64 = 42
sea y: i64 = 42

// Variables mutables
let mut counter: i64 = 0
sea mut contador: i64 = 0

// Structures
struct Point { x: f64, y: f64 }
estructura Punto { x: f64, y: f64 }

// Flux de contrôle
if x > 0 { ... } else { ... }
si x > 0 { ... } sino { ... }

while running { ... }
mientras corriendo { ... }

// Bloc natif C99
embed "c" {
    #include <stdio.h>
    printf("Bonjour depuis C99 !\n");
}
```

---

## 5. Étapes Suivantes

- **Architecture du compilateur →** [COMPILER_ARCHITECTURE.md](COMPILER_ARCHITECTURE.md)
- **Référence syntaxique complète →** [SYNTAX_AND_GRAMMAR.md](SYNTAX_AND_GRAMMAR.md)
- **Serveur MCP →** [../MCP_SERVER.md](../MCP_SERVER.md)
- **Feuille de route v2.4.0 →** [../ROADMAP_v2.4.0.md](../ROADMAP_v2.4.0.md)
- **Index principal →** [../INDEX.md](../INDEX.md)
