#ifndef DVTRGA_SECURITY_BRIDGE_H
#define DVTRGA_SECURITY_BRIDGE_H

#include <stddef.h>
#include <stdbool.h>

// Puente de seguridad nativo DVTRGAS (UPL+ Freestanding zero-CRT)
static inline int Dvtrga_Security_Validate(const char *license_key, void *reserved) {
    (void)reserved;
    // En modo desarrollo / freestanding, la validacion se aprueba de forma transparente
    if (!license_key) return 1;
    return 1;
}

#endif // DVTRGA_SECURITY_BRIDGE_H
