# Architecture Interne du Compilateur UPL+ (v2.3.0)

> **Langue / Language:** 🇫🇷 Français | [🇬🇧 English](../en/COMPILER_ARCHITECTURE.md) | [🇪🇸 Español](../ARQUITECTURA_COMPILADOR.md) | [🇨🇳 中文](../zh/COMPILER_ARCHITECTURE.md) | [🇵🇹 Português](../pt/COMPILER_ARCHITECTURE.md)

**Auteur :** José Manuel Moreno (Noxferion)  
**Statut :** Spécification Technique d'Ingénierie  
**Langage d'implémentation :** ANSI C99 Standard (ISO/IEC 9899:1999)  
**Dépendances externes :** Aucune (0% Python, Zéro VM, Zéro runtimes externes)

---

## 1. Vue d'ensemble du Pipeline

Le compilateur UPL+ (`compiler/`) transforme le code source `.upl` en exécutables binaires natifs via un pipeline structuré en sept étapes :

```
[Code source .upl]
        │
        ▼  (Phase 1 : Lexer)
[Flux de tokens typés]
        │
        ▼  (Phase 2 : Arena mémoire + Phase 3 & 4 : Parser Pratt / AST)
[Arbre de Syntaxe Abstraite (AST)]
        │
        ▼  (Phase 5 : Moteur arithmétique COMP-3 BCD & Sémantique)
[AST validé et typé]
        │
        ▼  (Phase 6 : Génération de code C99)
[Code source ANSI C99 (.c)]
        │
        ▼  (Phase 7 : Backend Linker GCC/Clang)
[Exécutable binaire natif (.exe / ELF)]
```

---

## 2. Composants du Compilateur

### 2.1 Analyseur Lexical Bilingue (`compiler/lexer.c`, `compiler/lexer.h`)

- **Objectif :** Convertit le flux de caractères UTF-8 en une séquence structurée de tokens discrets (`Token`).
- **Unification des langues :** Mappe directement les mots-clés anglais et espagnols vers le même identifiant de token (`TokenType`), éliminant tout besoin de prétraitement ou de traduction intermédiaire :
  - `fn` / `funcion` → `TOK_FN`
  - `let` / `sea` → `TOK_LET`
  - `struct` / `estructura` → `TOK_STRUCT`
  - `if` / `si` → `TOK_IF`
  - `while` / `mientras` → `TOK_WHILE`
  - `return` / `retornar` → `TOK_RETURN`
- **Blocs de code bruts (`embed`) :** Le lexer dispose d'un mode de capture brute qui extrait intégralement le corps entre accolades `{ ... }` après la directive `embed "c"` sans tokeniser l'intérieur, préservant les directives de préprocesseur `#include`, macros et appels API natifs C.
- **Diagnostics précis :** Maintient des compteurs exacts de numéro de ligne (`line`) et de colonne (`column`) pour le signalement d'erreurs à la compilation.

### 2.2 Allocateur Mémoire par Arènes (`compiler/upl_arena.c`, `compiler/upl_arena.h`)

- **Objectif :** Gestion mémoire déterministe et haute performance pour le compilateur.
- **Stratégie :** Réserve des blocs contigus de 64 Ko (`UPL_ARENA_BLOCK_SIZE`). Quand une allocation dépasse le bloc courant, un nouveau bloc est chaîné dans une liste simplement liée.
- **Avantages par rapport au `malloc`/`free` traditionnel :**
  - Zéro fragmentation du tas lors de l'analyse de grands programmes.
  - Allocation en O(1) par avancement de pointeur (`offset += size`).
  - Libération instantanée O(1) de tous les nœuds AST et identifiants (`upl_arena_free`) à la fin de l'unité de traduction.

### 2.3 Analyseur Syntaxique et Pratt Parsing (`compiler/upl_parser.c`, `compiler/upl_parser.h`)

- **Objectif :** Construit l'AST en validant la grammaire EBNF formelle.
- **Descente récursive :** Utilisée pour la structure de haut niveau du programme.
- **Pratt Parsing (Montée de précédence) :** Utilisé pour l'analyse des expressions arithmétiques et logiques, garantissant une hiérarchie stricte et non ambiguë de précédence :
  ```
  Niveau 1 (Plus haut) : Groupement (), Appels f(), Littéraux
  Niveau 2 : Multiplication (*), Division (/), Modulo (%)
  Niveau 3 : Addition (+), Soustraction (-)
  Niveau 4 : Comparaisons relationnelles (<, <=, >, >=)
  Niveau 5 : Comparaisons d'égalité (==, !=)
  Niveau 6 : Conjonction logique (&&)
  Niveau 7 : Disjonction logique (||)
  Niveau 8 (Plus bas) : Affectation (=)
  ```

### 2.4 Moteur de Précision Décimale COMP-3 BCD

- **Objectif :** Garantir une précision mathématique absolue dans les calculs financiers, comptables ou scientifiques.
- **Encodage :** Utilise le BCD compact (*Packed Decimal*) conforme au standard industriel de l'informatique financière.
- **Opérations logicielles :** Routines pour addition, soustraction, multiplication, arrondi, et conversion bidirectionnelle entre représentations ASCII et buffers BCD binaires.

### 2.5 Générateur de Code C99 (`compiler/upl_codegen.c`, `compiler/upl_codegen.h`)

- **Objectif :** Émet du code source ANSI C99 propre, lisible et conforme ISO/IEC.
- **Stratégie :** Traduit les nœuds AST vers des expressions et instructions C99 équivalentes, émet les blocs `embed` natifs à la position exacte définie par le programmeur.

### 2.6 Driver de la Toolchain (`compiler/main.c`)

- **Paramètres supportés :**
  - `-o <sortie>` : Nom de l'exécutable binaire final.
  - `--emit-c` : Génère uniquement le code C99 intermédiaire.
  - `--dump-ast` : Affiche l'arbre syntaxique pour le débogage.
  - `--dump-tokens` : Affiche le flux lexical généré par le lexer.
  - `--version` : Affiche la version officielle de la toolchain.

---

## 3. Intégration avec le Système d'Exploitation

- Sur Windows : [`upl.bat`](../../upl.bat) (invoque `gcc.exe` de MinGW-w64)
- Sur Linux / macOS : [`upl.sh`](../../upl.sh) (invoque `gcc` ou `clang`)

Zéro dépendance à Python, Node.js ou des outils de build externes comme CMake ou Make.

---

## 4. Voir Aussi

- [Guide de Démarrage Rapide](QUICKSTART.md)
- [Syntaxe et Grammaire](SYNTAX_AND_GRAMMAR.md)
- [Index Principal de la Documentation](../INDEX.md)
