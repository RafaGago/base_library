#ifndef __BL_PROCESSOR_PAUSE_H__
#define __BL_PROCESSOR_PAUSE_H__
/*---------------------------------------------------------------------------*/
#include <bl/base/platform.h>
/*---------------------------------------------------------------------------*/
#if BL_COMPILER_IS (MICROSOFT_VC)
  #include <bl/base/include_windows.h>
  #if WINVER < 0x0600
    #error "Update this code. YieldProcessor may not be available"
  #endif
  #define bl_processor_pause() YieldProcessor()
#endif
/*---------------------------------------------------------------------------*/
#if (BL_COMPILER_IS (GCC) || BL_COMPILER_IS (CLANG))

  /*When looking to add platforms just search the "cpu_relax()" function on
    the Linux Kernel tree for hints.*/
  #if BL_ARCH_IS_X64_X86_IA64
    /*same opcode than PAUSE (_mm_pause): F3 90*/
    #define bl_processor_pause()  __asm__ __volatile__ ("rep;nop": : :"memory")
  #endif

  #if BL_ARCH_IS (ARM)
    /*will execute as NOP on processors that don't support the instruction:
       http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0489f/CIHEGBBF.html
     */
    #define bl_processor_pause() __asm__ __volatile__ ("yield": : :"memory")
  #endif
#endif
/*---------------------------------------------------------------------------*/
#if !defined (bl_processor_pause)
  #error
    "bl_processor_pause() unavailable (minimal impl is a compiler barrier)"
#if 0
  /*defined (__GNUC__) || defined (GCC)*/
  #define compiler_barrier() __asm__ __volatile__ ("" : : : "memory")
#endif
#endif
/*---------------------------------------------------------------------------*/
#endif /* __BL_PROCESSOR_PAUSE_H__ */

