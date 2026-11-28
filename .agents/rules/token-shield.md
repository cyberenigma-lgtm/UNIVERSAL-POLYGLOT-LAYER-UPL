# Token Shield — Regla Permanente de Eficiencia Máxima

## 1. Lectura Quirúrgica
- NUNCA leer archivos completos. Usar `grep`/`Select-String` primero, luego leer solo rangos de 30-50 líneas alrededor del código a tocar.
- Si necesito una función, busco su número de línea y leo ±25 líneas.

## 2. Carpetas y Archivos Prohibidos (salvo petición explícita)
Ignorar completamente: `bin/`, `dist/`, `assets/`, `Proyectos_en_Revision/`, `*.exe`, `*.wasm`, `*.zip`, `*.diff`, logs, `*.gif`, `*.jpg`, `*.png`.

## 3. Diffs Mínimos — Nunca Archivos Completos
- Editar código con cambios quirúrgicos (`edit_file` con Target exacto).
- Jamás reescribir un archivo completo si solo cambia 1-5 líneas.

## 4. Respuestas Ultra-Concisas (Modo CLI)
- Máximo 3-5 líneas de respuesta al grano.
- Prohibido: introducciones largas, repetir código ya escrito, resúmenes innecesarios, explicaciones obvias.
- Si algo fue exitoso: una línea con ✅ y el hecho. Fin.

## 5. Comandos Silenciosos
- Capturar solo código de salida o últimas 10 líneas: `... | Select-Object -Last 10`
- Nunca volcar salidas completas de compilación o tests.

## 6. Paralelismo Obligatorio
- Todas las operaciones independientes van en la misma llamada de herramientas.
- Nunca una herramienta por turno si se pueden batched.
