Created By HaleSpinfex under the GNU license

Instruction	Description
AND	Performs a bitwise AND operation between two operands.
XAND	Exclusive AND, performs a bitwise AND operation only where both operands are 1.
NAND	Performs a bitwise NAND operation (NOT AND).
XNAND	Exclusive NAND, the inverse of Exclusive AND.
OR	Performs a bitwise OR operation between two operands.
XOR	Performs a bitwise XOR (exclusive OR) operation between two operands.
NOR	Performs a bitwise NOR operation (NOT OR).
XNOR	Exclusive NOR, the inverse of XOR (if operands are the same, result is 1).
NOT	Performs a bitwise NOT operation (inverts all bits of the operand).
XNOT	Exclusive NOT, a variation that works with a single operand, but with exclusive logic.
POP	Pops the top value from the stack into a register or memory.
JMP	Jump to a specified address or label unconditionally.
JC	Jump if carry flag is set (used for conditional branching based on the carry flag).
JZ	Jump if zero flag is set (used for conditional branching based on the zero flag).
ADD	Adds two operands together and stores the result in the destination.
SUB	Subtracts the second operand from the first operand and stores the result in the destination.
MUL	Unsigned multiplication of the accumulator register by a specified operand. Result is stored across two registers (AX, DX).
DIV	Unsigned division, divides the value in AX by the operand. Quotient is stored in AX, remainder in DX.
DEC	Decrements the value of a register or memory operand by 1.
INC	Increments the value of a register or memory operand by 1.
MOV	Moves (copies) data from one register/memory location to another.
SHL	Shift left, shifts bits of the operand to the left, inserting zeroes into the rightmost bits.
SHR	Shift right, shifts bits of the operand to the right, inserting zeroes into the leftmost bits.
LOCK	Used to acquire a lock on a memory location, ensuring atomicity for multi-threaded operations.
XCHG	Exchanges the contents of two registers or a register and memory location. Often used in synchronization.
CMPXCHG	Compares two operands, and if they are equal, replaces the value in memory with the second operand. Often used for atomic operations.
PUSH	Pushes a value onto the stack, used in multi-threaded environments for saving state.
POPF	Pops the flags register off the stack, restoring its state. Can be used to save/restore processor state in multi-threading.
CLI	Clears the interrupt flag (disables interrupts), often used in multi-core systems to prevent interruptions during critical code sections.
STI	Sets the interrupt flag (enables interrupts), typically used after critical sections of code in multi-core systems to allow interrupt handling again.
FWAIT	Waits for an FPU (Floating Point Unit) operation to finish, used in synchronization in multi-threaded contexts.
SFENCE	Ensures that all previous store operations to memory are completed before the fence, useful for memory ordering in multi-core systems.
LFENCE	Ensures that all previous load operations are completed before the fence, used for synchronization in multi-threaded contexts.
MFENCE	Ensures that all previous memory operations (loads and stores) are completed before the fence. It’s used to enforce ordering in multi-core systems.

Intel 3003 Emulator — Ultra Technical Notes
Purpose
This emulator serves as a low-level x86-16 inspired microprocessor testbed, focused on faithful reproduction of Intel 3003 legacy instruction behaviors combined with modern concurrency primitives for dual-core simulation. Intended strictly for users with solid x86 assembly and microprocessor architecture knowledge. No hand-holding.

Instruction Set Specifics
Base Instructions:

Fully implement standard Intel 3003 arithmetic/logical ops: ADD, SUB, INC, DEC, AND, OR, XOR, NOT.

Control flow instructions: JMP, JC, JZ use status flags (CF, ZF) accurately.

Data movement via MOV, stack ops PUSH, POP.

Shift ops SHL and SHR behave as logical shifts; no arithmetic shifts implemented.

Extended Bitwise Ops:

XAND, XNAND, XNOR, XNOT are custom additions providing exclusive variants of standard bitwise operations.

All extended ops operate bitwise across 16-bit operands, results stored in destination registers/memory.

Side effects on flags mirror standard logical operations, except XNOT only affects zero flag (ZF).

Atomic & Concurrency Instructions:

LOCK prefix disables interrupts and acquires software lock on memory location for atomic ops.

XCHG exchanges values atomically between registers/memory.

CMPXCHG performs compare-and-swap: compares accumulator to memory; if equal, swaps in new value. Sets ZF accordingly.

Memory fences SFENCE, LFENCE, MFENCE enforce ordering of store/load/memory operations to simulate multi-core coherency.

CLI and STI clear and set interrupt flags respectively, to protect critical sections.

Multi-Core Synchronization
Each core executes instructions independently but synchronizes memory through fences and locks.

Atomic ops (LOCK + XCHG, CMPXCHG) guarantee mutual exclusion across cores; emulation uses spinlocks internally.

Memory fences ensure ordering:

SFENCE: Ensures all prior stores complete before subsequent stores.

LFENCE: Ensures all prior loads complete before subsequent loads.

MFENCE: Ensures all prior memory ops complete before any following ops.

Flags and Status Register
Standard flags implemented:

CF (Carry Flag) — affected by arithmetic ops and checked by JC.

ZF (Zero Flag) — set if result is zero, used by JZ and atomic ops.

SF (Sign Flag), OF (Overflow Flag) currently not emulated.

Programming Model and Execution
Instructions execute sequentially within each core; no out-of-order or speculative execution emulated.

Timing model: single cycle per instruction; no pipeline stalls simulated.

Stack grows downward; PUSH decrements stack pointer before storing value, POP reads value before incrementing.

Interrupts disabled/enabled via CLI/STI affect core-level interrupt handling only; no external interrupt sources simulated.

Known Limitations
No floating-point or MMX/SSE instructions emulated.

No privileged instructions or I/O port simulation yet.

Memory model is flat; no segmentation or paging.

Emulation assumes little-endian byte order.

Multi-core cache coherence is approximated via fences; no true cache model implemented.

Recommended Usage
Assemble and run test programs using standard x86 assembly with adjustments for extended bitwise instructions.

Use atomic ops and fences to test multi-threaded synchronization primitives.

Debug by inspecting flags, registers, and memory dumps exposed via emulator API.
