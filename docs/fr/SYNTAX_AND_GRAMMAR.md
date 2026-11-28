# Syntaxe et Grammaire UPL+ — Spécification (v2.3.0)

> **Langue / Language:** 🇫🇷 Français | [🇬🇧 English](../en/SYNTAX_AND_GRAMMAR.md) | [🇪🇸 Español](../SPEC_GRAMATICA_Y_SISTEMA_TIPOS.md) | [🇨🇳 中文](../zh/SYNTAX_AND_GRAMMAR.md) | [🇵🇹 Português](../pt/SYNTAX_AND_GRAMMAR.md)

**Auteur :** José Manuel Moreno (Noxferion)  
**Classification :** Langage de programmation systèmes / Transpilation typée C99

---

## 1. Table d'Équivalence des Mots-clés Bilingues

Le lexer mappe de façon déterministe les mots-clés anglais et espagnols vers le même identifiant de token (`UPLTokenType`) :

| Token Lexical | Mot-clé (EN) | Mot-clé (ES) | Sémantique |
|---|---|---|---|
| `TOK_MODULE` | `module` | `modulo` | Déclaration de l'espace de noms de l'unité de compilation |
| `TOK_IMPORT` | `import` | `importar` | Inclusion de module ou d'unité externe |
| `TOK_STRUCT` | `struct` | `estructura` | Définition de structure de données agrégée |
| `TOK_PUB` | `pub` | `publico` | Modificateur de visibilité exportable |
| `TOK_FN` | `fn` | `funcion` | Déclaration ou définition de sous-programme |
| `TOK_LET` | `let` | `sea` | Déclaration de variable immuable par défaut |
| `TOK_MUT` | `mut` | `mut` | Modificateur de mutabilité pour les variables |
| `TOK_CONST` | `const` | `constante` | Constante immuable évaluable à la compilation |
| `TOK_IF` | `if` | `si` | Branchement conditionnel |
| `TOK_ELSE` | `else` | `sino` | Branche alternative du conditionnel |
| `TOK_WHILE` | `while` | `mientras` | Boucle d'itération conditionnelle pré-test |
| `TOK_FOR` | `for` | `para` | Boucle d'itération structurée |
| `TOK_RETURN` | `return` | `retornar` | Retour du contrôle et de la valeur depuis une fonction |
| `TOK_AS` | `as` | `como` | Conversion de type explicite (cast) |
| `TOK_EMBED` | `embed` | `incrustar` | Bloc de code natif littéral (C ou langage hôte) |
| `TOK_ASM` | `asm` | `ensamblador` | Bloc d'instructions assembleur inline |

---

## 2. Système de Types

### 2.1 Types Primitifs

| Type | Taille | Description |
|------|--------|-------------|
| `i8` | 1 octet | Entier signé 8 bits |
| `i16` | 2 octets | Entier signé 16 bits |
| `i32` | 4 octets | Entier signé 32 bits |
| `i64` | 8 octets | Entier signé 64 bits |
| `u8` | 1 octet | Entier non signé 8 bits |
| `u64` | 8 octets | Entier non signé 64 bits |
| `f32` | 4 octets | Flottant simple précision IEEE 754 |
| `f64` | 8 octets | Flottant double précision IEEE 754 |
| `bool` | 1 octet | Booléen (`true` / `false`) |
| `str` | pointeur + longueur | Tranche de chaîne UTF-8 |
| `void` | — | Aucune valeur (retour de fonction) |

### 2.2 Propriété et `defer`

```upl
fn process() -> i64 {
    let handle = open_resource()
    defer close_resource(handle)   // Libération garantie à la sortie du scope
    return 0
}
```

---

## 3. Exemple de Syntaxe

```upl
module exemple

import std/io

fn ajouter(a: i64, b: i64) -> i64 {
    return a + b
}

funcion multiplicar(a: i64, b: i64) -> i64 {
    retornar a * b
}

fn main() -> i64 {
    let resultat = ajouter(10, 20)

    si resultat > 0 {
        io::println("Résultat positif : ", resultat)
    } sino {
        io::println("Résultat nul ou négatif")
    }

    sea mut compteur: i64 = 0
    mientras compteur < 5 {
        io::println("Compte : ", compteur)
        compteur = compteur + 1
    }

    return 0
}
```

---

## 4. Interopérabilité Native C99

```upl
module interop

fn exemple_natif() -> i64 {
    embed "c" {
        #include <stdio.h>
        printf("UPL+ depuis C99 !\n");
        return 0LL;
    }
}
```

---

## 5. Voir Aussi

- [Guide de Démarrage Rapide](QUICKSTART.md)
- [Architecture du Compilateur](COMPILER_ARCHITECTURE.md)
- [Index Principal de la Documentation](../INDEX.md)
