# 🚀 DÉMARRAGE RAPIDE : LANGAGE DE PROGRAMMATION UPL+
## Langage de Programmation Système et Compilateur Natif C99

Bienvenue dans la documentation officielle d'**UPL+ (Universal Polyglot Layer Plus)**. UPL+ est un **langage de programmation système compilé, fortement typé et autonome**, conçu pour offrir de très hautes performances, une interopérabilité directe avec le matériel et une arithmétique financière exacte (COMP-3).

---

### 🌟 Piliers de l'Architecture UPL+

1. **Compilateur Natif Autonome (`uplc`) :** Conçu en ANSI C99 modulaire sans aucune dépendance envers Python, LLVM ou des machines virtuelles lourdes.
2. **Syntaxe Polyglotte Mondiale :** Prise en charge native de mots-clés bilingues et multilingues au niveau lexical.
3. **Arithmétique Décimale Exacte (`comp3`) :** Types BCD financiers éliminant les erreurs d'arrondi de la norme IEEE-754.
4. **Fusion Silicium (C99 & Assembleur x86_64) :** Blocs `embed` et `asm` intégrés sans surcoût d'appel FFI.
5. **Gestion Mémoire par Arènes :** Allocations déterministes par blocs de 64 Ko, éliminant la fragmentation.

---

### 📦 Votre Premier Programme en UPL+

```upl
module exemple;

pub fn main() -> i32 {
    let mut total: i32 = 0;
    let base: i32 = 1000;
    let taux: i32 = 20;

    total = base + (base * taux / 100);
    return 0;
}
```

---

### ⚡ Compilation et Exécution

```bash
# Compilation et exécution en 1 étape :
bin/uplc.exe run mon_programme.upl

# Compilation en binaire natif :
bin/uplc.exe build mon_programme.upl -o bin/mon_programme.exe
```

---
*Universal Polyglot Layer (UPL) — Propriété Intellectuelle Enregistrée chez SafeCreative.*
