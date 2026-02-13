# 🛠️ Resolución de Errores Comunes en UPL

Como sistema único en el mundo, UPL requiere entender su flujo de unificación. Aquí tienes los problemas más frecuentes.

## 1. "Language Tag Not Found"
**Error**: El código no se traduce y aparece como bloque desconocido.
**Causa**: La etiqueta del lenguaje no está bien escrita o no está en su propia línea.
**Solución**: Asegúrate de que `#python`, `//c` o `zig` esté al inicio de una línea limpia.

## 2. "IR Logic Inconsistency"
**Error**: El código intermedio generado no tiene sentido.
**Causa**: Estás intentando usar variables de un bloque en otro sin inicializarlas correctamente en el flujo.
**Solución**: Recuerda que UPL unifica la lógica, pero cada bloque debe ser sintácticamente válido para su parser respectivo.

## 3. "ImportError: No module named 'parsers'"
**Error**: El orquestador falla al arrancar.
**Causa**: Estás ejecutando el orquestador desde una carpeta distinta a la raíz `Universal-Polyglot-Layer`.
**Solución**: Ejecuta siempre desde la raíz para que el sistema de rutas soberanas funcione.

## 4. "Sovereign Locale Key missing"
**Error**: No se traduce al español el IR.
**Causa**: El archivo `upl_locales.json` no tiene la entrada para ese mnemónico.
**Solución**: Añade la traducción en `core/upl_locales.json`.
