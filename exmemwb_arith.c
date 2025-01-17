#include <stdlib.h>
#include <assert.h>
#include "exmemwb.h"
#include "decode.h"

///--- Add operations --------------------------------------------///

// ADCS - add with carry and update flags
u32 adcs()
{
    diss_printf("adcs r%u, r%u\n", decoded.rD, decoded.rM);

    u32 opA = cpu_get_gpr(decoded.rD);
    u32 opB = cpu_get_gpr(decoded.rM);
    u32 result = opA + opB + cpu_get_flag_c();

    cpu_set_gpr(decoded.rD, result);

    do_nflag(result);
    do_zflag(result);
    do_cflag(opA, opB, cpu_get_flag_c());
    do_vflag(opA, opB, result);

    return 1;
}

// ADD - add small immediate to a register and update flags
u32 adds_i3()
{
    diss_printf("adds r%u, r%u, #0x%X\n", decoded.rD, decoded.rN, decoded.imm);

    u32 opA = cpu_get_gpr(decoded.rN);
    u32 opB = zeroExtend32(decoded.imm);
    u32 result = opA + opB;

    cpu_set_gpr(decoded.rD, result);

    do_nflag(result);
    do_zflag(result);
    do_cflag(opA, opB, 0);
    do_vflag(opA, opB, result);

    return 1;
}

// ADD - add large immediate to a register and update flags
u32 adds_i8()
{
    diss_printf("adds r%u, #0x%X\n", decoded.rD, decoded.imm);

    u32 opA = cpu_get_gpr(decoded.rD);
    u32 opB = zeroExtend32(decoded.imm);
    u32 result = opA + opB;

    cpu_set_gpr(decoded.rD, result);

    do_nflag(result);
    do_zflag(result);
    do_cflag(opA, opB, 0);
    do_vflag(opA, opB, result);

    return 1;
}

// ADD - add two registers and update flags
u32 adds_r()
{
    diss_printf("adds r%u, r%u, r%u\n", decoded.rD, decoded.rN, decoded.rM);

    u32 opA = cpu_get_gpr(decoded.rN);
    u32 opB = cpu_get_gpr(decoded.rM);
    u32 result = opA + opB;

    cpu_set_gpr(decoded.rD, result);

    do_nflag(result);
    do_zflag(result);
    do_cflag(opA, opB, 0);
    do_vflag(opA, opB, result);

    return 1;
}

// ADD - add two registers, one or both high no flags
u32 add_r()
{
    diss_printf("add r%u, r%u\n", decoded.rD, decoded.rM);

    // Check for malformed instruction
    if(decoded.rD == 15 && decoded.rM == 15)
    {
        //UNPREDICTABLE
        fprintf(stderr, "Error: Instruction format error.\n");
        sim_exit(1);
    }

    u32 opA = cpu_get_gpr(decoded.rD);
    u32 opB = cpu_get_gpr(decoded.rM);
    u32 result = opA + opB;

    // If changing the PC, check that thumb mode maintained
    if(decoded.rD == GPR_PC)
        alu_write_pc(result);
    else
        cpu_set_gpr(decoded.rD, result);

    // Instruction takes two cycles when PC is the destination
    return (decoded.rD == GPR_PC) ? 2 : 1;
}

// ADD - add an immpediate to SP
u32 add_sp()
{
    diss_printf("add r%u, SP, #0x%02X\n", decoded.rD, decoded.imm);
    
    u32 opA = cpu_get_sp();
    u32 opB = zeroExtend32(decoded.imm << 2);
    u32 result = opA + opB;
    
    cpu_set_gpr(decoded.rD, result);

    return 1;
}

// ADR - add an immpediate to PC
u32 adr()
{
    diss_printf("adr r%u, PC, #0x%02X\n", decoded.rD, decoded.imm);
    
    u32 opA = cpu_get_pc();
    // Align PC to 4 bytes
    opA = opA & 0xFFFFFFFC;
    u32 opB = zeroExtend32(decoded.imm << 2);
    u32 result = opA + opB;
    
    cpu_set_gpr(decoded.rD, result);

    return 1;
}

///--- Subtract operations --------------------------------------------///

u32 subs_i3()
{
    diss_printf("subs r%u, r%u, #0x%X\n", decoded.rD, decoded.rN, decoded.imm);

    u32 opA = cpu_get_gpr(decoded.rN);
    u32 opB = ~zeroExtend32(decoded.imm);
    u32 result = opA + opB + 1;

    cpu_set_gpr(decoded.rD, result);

    do_nflag(result);
    do_zflag(result);
    do_cflag(opA, opB, 1);
    do_vflag(opA, opB, result);

    return 1;
}

u32 subs_i8()
{
    diss_printf("subs r%u, #0x%02X\n", decoded.rD, decoded.imm);

    u32 opA = cpu_get_gpr(decoded.rD);
    u32 opB = ~zeroExtend32(decoded.imm);
    u32 result = opA + opB + 1;

    cpu_set_gpr(decoded.rD, result);

    do_nflag(result);
    do_zflag(result);
    do_cflag(opA, opB, 1);
    do_vflag(opA, opB, result);

    return 1;
}

u32 subs()
{
    diss_printf("subs r%u, r%u, r%u\n", decoded.rD, decoded.rN, decoded.rM);
    
    u32 opA = cpu_get_gpr(decoded.rN);
    u32 opB = ~cpu_get_gpr(decoded.rM);
    u32 result = opA + opB + 1;
    
    cpu_set_gpr(decoded.rD, result);

    do_nflag(result);
    do_zflag(result);
    do_cflag(opA, opB, 1);
    do_vflag(opA, opB, result);

    return 1;
}

u32 sub_sp()
{
    diss_printf("sub SP, #0x%02X\n", decoded.imm);

    u32 opA = cpu_get_sp();
    u32 opB = ~zeroExtend32(decoded.imm << 2);
    u32 result = opA + opB + 1;

    cpu_set_sp(result);

    return 1;
}

u32 sbcs()
{
    diss_printf("sbcs r%u, r%u\n", decoded.rD, decoded.rM);

    u32 opA = cpu_get_gpr(decoded.rD);
    u32 opB = ~cpu_get_gpr(decoded.rM);
    u32 result = opA + opB + cpu_get_flag_c();

    cpu_set_gpr(decoded.rD, result);

    do_nflag(result);
    do_zflag(result);
    do_cflag(opA, opB, cpu_get_flag_c());
    do_vflag(opA, opB, result);

    return 1;
}

u32 rsbs()
{
    diss_printf("rsbs r%u, r%u\n, #0", decoded.rD, decoded.rN);

    u32 opA = 0;
    u32 opB = ~(cpu_get_gpr(decoded.rN));
    u32 result = opA + opB + 1;

    cpu_set_gpr(decoded.rD, result);

    do_nflag(result);
    do_zflag(result);
    do_cflag(opA, opB, 1);
    do_vflag(opA, opB, result);

    return 1;
}

///--- Multiply operations --------------------------------------------///

// MULS - multiply the source and destination and store 32-bits in dest
// Does not update carry or overflow: simple mult
u32 muls()
{
    diss_printf("muls r%u, r%u\n", decoded.rD, decoded.rM);

    u32 opA = cpu_get_gpr(decoded.rD);
    u32 opB = cpu_get_gpr(decoded.rM);
    u32 result = opA * opB;
    
    cpu_set_gpr(decoded.rD, result);

    do_nflag(result);
    do_zflag(result);

    // On STM32F0xx the multiplier is single-cycle.
    // return 32;
    return 1;
}
