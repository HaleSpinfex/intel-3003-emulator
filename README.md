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
