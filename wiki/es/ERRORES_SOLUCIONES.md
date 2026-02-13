# ⚠️ ERRORES Y SOLUCIONES - Wiki UPL

En esta sección encontrarás los errores más comunes al trabajar con la **Universal Polyglot Layer** y cómo resolverlos rápidamente para mantener tu flujo de soberanía.

## 🔴 1. Error de Identificación de Bloque
**Síntoma**: El kernel no reconoce un lenguaje específico.
- **Causa**: Falta el identificador de lenguaje al inicio del bloque o está mal escrito.
- **Solución**: Asegúrate de que cada bloque empiece con el nombre del lenguaje solo en una línea.
  - *Correcto*: `python`, `rust`, `zig`.
  - *Incorrecto*: `#!python`, `language: rust`.

## 🔴 2. Fallo de Localización IR
**Síntoma**: El autocompletado no muestra mnemónicas en tu idioma.
- **Causa**: El local no está correctamente configurado o el archivo `upl_locales.json` está dañado.
- **Solución**: Verifica que el selector de **Unicidad Mundial** en el IDE esté en tu idioma. Si usas la CLI, usa el parámetro `-l es`.

## 🔴 3. Error de Unificación (Mixed Logic)
**Síntoma**: El código de un lenguaje intenta acceder directamente a variables de otro sin pasar por el Orquestador.
- **Causa**: Intento de comunicación directa no soportada.
- **Solución**: Usa las mnemónicas del **Lenguaje Madre** para intercambiar datos entre bloques de forma segura.

## 🔴 4. Proceso de Terminal Bloqueado
**Síntoma**: La Terminal Nativa no responde.
- **Causa**: Un comando exterior se ha quedado en bucle infinito.
- **Solución**: Usa `Ctrl+C` en la terminal o reinicia el IDE. La Terminal Nativa de UPL Studio v1.3 auto-gestiona el cierre de procesos huérfanos.

---
**¿No encuentras la solución?**
Consulta el manual avanzado de **NUASM** o contacta con la Comunidad Intelectual de **Neuro-OS Genesis**.
