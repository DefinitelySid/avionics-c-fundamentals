# Avionics C Fundamentals

Safety-critical avionics and flight-software primitives written from scratch in pure C (zero dynamic memory allocation, fixed-width types, and deterministic execution).

## Modules

| File | Description | Core Concepts |
| :--- | :--- | :--- |
| `arinc429.c` | 32-bit ARINC 429 word encoder, decoder, and odd-parity error detector with simulated bus interference. | Bitwise shifts/masks, 32-bit protocol packing, odd parity (`^`) |
| `flightEnvelope.c` | Real-time flight envelope protection monitor tracking stall (`< 150 kts`) and overspeed (`> 350 kts`) states. | Custom `typedef struct`, pointer mutation (`->`), status bitflags |
| `blackboxFDR.c` | Deterministic circular ring-buffer Flight Data Recorder (FDR) with chronological crash-dump reconstruction. | Fixed-size arrays (`no malloc`), modulo wrap-around (`%`), `const` pointers |
| `blackboxFDR2.c` | Upgraded Black-Box FDR with per-frame XOR integrity checksums and crash-trigger freeze lock. | Multi-byte XOR checksums, bit-flip corruption detection, post-crash write protection |

## Build & Run

Compile any module using GCC:

```bash
gcc blackboxFDR2.c -o blackboxFDR2
./blackboxFDR2
```

## Design Principles
* **Deterministic Memory:** No `malloc()` or `free()`—all buffers are statically allocated at compile time per DO-178C / MISRA C safety guidelines.
* **Explicit Widths:** Uses `<stdint.h>` fixed-width integers (`uint8_t`, `int16_t`, `uint32_t`) for hardware portability across PC, AVR (Arduino), and ESP32 targets.
