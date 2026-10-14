# 💻 UPL CODE EXAMPLES GALLERY
## Practical Demonstrations of the Sovereign Programming Language

---

### Example 1: Exact Financial Math with NeuroCOBOL (`comp3`)

Eliminates IEEE 754 precision errors:

```rust
module finance::settlement;

pub struct SovereignInvoice {
    invoice_id: u64,
    base_amount: comp3(12, 2),
    tax_rate_pct: u32,
    total_amount: comp3(12, 2)
}

pub fn calculate_total(inv: *SovereignInvoice) -> void {
    let base: comp3(12, 2) = (*inv).base_amount;
    let rate: comp3(12, 2) = (*inv).tax_rate_pct as comp3(12, 2);
    let tax: comp3(12, 2) = (base * rate) / 100;
    (*inv).total_amount = base + tax;
}
```

---

### Example 2: Silicon-Level Control (Bare-Metal Ring 0)

Direct hardware control and CPU registers without intermediaries:

```rust
module hardware::serial;

pub const COM1_PORT: u16 = 0x3F8;

@baremetal
pub fn init_serial_port() -> void {
    asm {
        mov dx, COM1_PORT + 1
        mov al, 0x00
        out dx, al        // Disable interrupts

        mov dx, COM1_PORT + 3
        mov al, 0x80
        out dx, al        // Enable DLAB

        mov dx, COM1_PORT
        mov al, 0x03
        out dx, al        // Set 38400 baud divisor

        mov dx, COM1_PORT + 3
        mov al, 0x03
        out dx, al        // 8 bits, no parity, one stop bit
    }
}
```

---

### Example 3: Immunity and Self-Healing with `@snapshot`

```rust
module security::shield;

@snapshot
pub struct TransactionState {
    tx_id: u64,
    balance: u64,
    is_locked: bool
}

pub fn process_secure(tx: *TransactionState) -> bool {
    let mut attempts: u32 = 0;
    while attempts < 3 {
        // Any memory fault automatically triggers a state rollback
        attempts = attempts + 1;
    }
    return true;
}
```
