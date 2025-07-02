#include <iostream>
#include <bitset>
#include <vector>
#include <thread>
#include <mutex>

std::mutex core_mutex;

// Basic logic gates
bool AND(bool a, bool b) { return a & b; }
bool OR(bool a, bool b) { return a | b; }
bool XOR(bool a, bool b) { return a ^ b; }
bool NOT(bool a) { return !a; }
bool NAND(bool a, bool b) { return !(a & b); }
bool NOR(bool a, bool b) { return !(a | b); }
bool XNOR(bool a, bool b) { return !(a ^ b); }
bool NXOR(bool a, bool b) { return !(a ^ b); }
bool XAND(bool a, bool b) { return a & !b; }
bool NXAND(bool a, bool b) { return !(a & !b); }
bool XNOT(bool a) { return !a; }

// Half Adder
void halfAdder(bool a, bool b, bool &sum, bool &carry) {
    sum = XOR(a, b);
    carry = AND(a, b);
}

// Full Adder
void fullAdder(bool a, bool b, bool cin, bool &sum, bool &carry) {
    bool s1, c1, c2;
    halfAdder(a, b, s1, c1);
    halfAdder(s1, cin, sum, c2);
    carry = OR(c1, c2);
}

// 16-bit ALU (Add/Subtract/Multiply/Divide)
std::bitset<16> ALU(std::bitset<16> A, std::bitset<16> B, bool subtract, bool multiply, bool divide) {
    std::bitset<16> result;
    bool carry = subtract;

    if (multiply) {
        result = std::bitset<16>(A.to_ulong() * B.to_ulong());
    } else if (divide) {
        if (B.to_ulong() != 0)
            result = std::bitset<16>(A.to_ulong() / B.to_ulong());
        else
            result = 0; // Prevent division by zero
    } else {
        for (int i = 0; i < 16; i++) {
            bool sum, c;
            fullAdder(A[i], XOR(B[i], subtract), carry, sum, c);
            result[i] = sum;
            carry = c;
        }
    }
    return result;
}

// Increment & Decrement
std::bitset<16> INC(std::bitset<16> A) {
    return ALU(A, std::bitset<16>(1), false, false, false);
}
std::bitset<16> DEC(std::bitset<16> A) {
    return ALU(A, std::bitset<16>(1), true, false, false);
}

// Shift Left and Shift Right
std::bitset<16> SHL(std::bitset<16> A) {
    return A << 1;
}

std::bitset<16> SHR(std::bitset<16> A) {
    return A >> 1;
}

// Registers and Memory
struct CPU {
    std::bitset<16> registers[8]; // 8 general-purpose registers
    std::vector<std::bitset<16>> memory;

    // Stack pointer and flags
    int sp = 0; // Stack pointer for PUSH/POP operations
    bool zeroFlag = false;
    bool carryFlag = false;

    CPU(size_t memory_size) : memory(memory_size) {}

    // MOV instruction: Move value into register
    void MOV(int reg, std::bitset<16> value) {
        registers[reg] = value;
    }

    // JMP instruction: Jump to specific memory address (unconditional)
    void JMP(int address) {
        sp = address; // Set stack pointer to the jump address (simulating jump)
    }

    // JC instruction: Jump if carry flag is set
    bool JC() {
        return carryFlag; // Jump if carry flag is set
    }

    // JZ instruction: Jump if zero flag is set
    bool JZ() {
        return zeroFlag; // Jump if zero flag is set
    }

    // PUSH instruction: Push register value to stack
    void PUSH(int reg) {
        if (sp < memory.size()) {
            memory[sp] = registers[reg]; // Store value in memory
            sp++;
        }
    }

    // POP instruction: Pop value from stack into register
    void POP(int reg) {
        if (sp > 0) {
            sp--;
            registers[reg] = memory[sp]; // Retrieve value from memory
        }
    }

    // SUB instruction: Subtract register values
    void SUB(int reg1, int reg2) {
        registers[reg1] = ALU(registers[reg1], registers[reg2], true, false, false);
    }

    // ADD instruction: Add register values
    void ADD(int reg1, int reg2) {
        registers[reg1] = ALU(registers[reg1], registers[reg2], false, false, false);
    }

    // Lock Instruction: For multi-core synchronization
    void LOCK() {
        std::lock_guard<std::mutex> lock(core_mutex);
    }

    // XCHG Instruction: Exchange register values (for atomic operations)
    void XCHG(int reg1, int reg2) {
        std::bitset<16> temp = registers[reg1];
        registers[reg1] = registers[reg2];
        registers[reg2] = temp;
    }

    // CMPXCHG Instruction: Compare and exchange register values
    void CMPXCHG(int reg1, int reg2) {
        if (registers[reg1] == registers[reg2]) {
            registers[reg1] = registers[reg2]; // Exchange if equal
        }
    }

    // CLI Instruction: Clear interrupt flag
    void CLI() {
        // In a real CPU, this would disable interrupts.
        std::cout << "Interrupt flag cleared.\n";
    }

    // STI Instruction: Set interrupt flag
    void STI() {
        // In a real CPU, this would enable interrupts.
        std::cout << "Interrupt flag set.\n";
    }

    // SFENCE Instruction: Memory fence to ensure stores are ordered
    void SFENCE() {
        // In a real CPU, this would enforce memory ordering between cores.
        std::cout << "Store fence applied.\n";
    }

    // LFENCE Instruction: Memory fence to ensure loads are ordered
    void LFENCE() {
        // In a real CPU, this would enforce load ordering between cores.
        std::cout << "Load fence applied.\n";
    }

    // MFENCE Instruction: Memory fence to ensure all memory operations are ordered
    void MFENCE() {
        // In a real CPU, this would ensure that all memory operations are complete before proceeding.
        std::cout << "Memory fence applied.\n";
    }
};

// Simulate Execution for a Core
void execute_core(CPU &cpu, int core_id) {
    std::lock_guard<std::mutex> lock(core_mutex);
    std::cout << "Executing on core " << core_id << "\n";

    // MOV instruction example
    cpu.MOV(0, std::bitset<16>("0000000000001010")); // MOV 10 to register 0
    cpu.MOV(1, std::bitset<16>("0000000000000101")); // MOV 5 to register 1
    
    cpu.ADD(0, 1); // ADD register 1 to register 0 (10 + 5)
    cpu.PUSH(0);   // PUSH register 0 onto the stack
    cpu.POP(2);    // POP the value into register 2
    
    cpu.SUB(2, 1); // SUB register 1 from register 2 (result should be 0)
    
    // Test atomic exchange and compare exchange
    cpu.XCHG(0, 1);
    cpu.CMPXCHG(0, 1);
}

// Main function
int main() {
    CPU core1(1024), core2(1024);
    
    std::bitset<16> A("0000000000001010"); // 10
    std::bitset<16> B("0000000000000101"); // 5
    
    // ALU operations
    std::cout << "A + B = " << ALU(A, B, false, false, false) << "\n";
    std::cout << "A - B = " << ALU(A, B, true, false, false) << "\n";
    std::cout << "A * B = " << ALU(A, B, false, true, false) << "\n";
    std::cout << "A / B = " << ALU(A, B, false, false, true) << "\n";
    std::cout << "INC(A) = " << INC(A) << "\n";
    std::cout << "DEC(A) = " << DEC(A) << "\n";
    
    // Shift operations
    std::cout << "SHL(A) = " << SHL(A) << "\n";
    std::cout << "SHR(A) = " << SHR(A) << "\n";
    
    // Multi-core execution
    std::thread t1(execute_core, std::ref(core1), 1);
    std::thread t2(execute_core, std::ref(core2), 2);
    
    t1.join();
    t2.join();

    // Example of using conditional jump (JZ and JC)
    if (core1.JZ()) {
        std::cout << "Jump to address (Zero flag is set)\n";
    }

    if (core1.JC()) {
        std::cout << "Jump to address (Carry flag is set)\n";
    }

    return 0;
}
