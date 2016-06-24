#ifndef __BL_PROCESSOR_PAUSE_H__
#define __BL_PROCESSOR_PAUSE_H__
/*---------------------------------------------------------------------------*/
#include <bl/base/platform.h>
/*---------------------------------------------------------------------------*/
#ifdef _MSC_VER
  #include <bl/base/include_windows.h>
  #if WINVER < 0x0600
    #error "Update this code. YieldProcessor may not be available"
  #endif  
  #define processor_pause() YieldProcessor()
#endif
/*---------------------------------------------------------------------------*/
#if defined (__GNUC__) || defined (GCC)

  /*When looking to add platforms just search the "cpu_relax()" function on 
    the Linux Kernel tree for hints.*/
  #if defined (BL_INTEL_AMD_PC)
    /*same opcode than PAUSE (_mm_pause): F3 90*/
    #define processor_pause()  __asm__ __volatile__ ("rep;nop": : :"memory")
  #endif

  #if defined (BL_ARM)
    /*will execute as NOP on processor that doesn't support it:
       http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0489f/CIHEGBBF.html
     */    
    #define processor_pause() __asm__ __volatile__ ("yield": : :"memory")
  #endif
#endif
/*---------------------------------------------------------------------------*/
#if !defined (processor_pause)
  #error
    "processor_pause() unavailable (minimal impl is a compiler barrier)"
#if 0
  /*defined (__GNUC__) || defined (GCC)*/
  #define compiler_barrier() __asm__ __volatile__ ("" : : : "memory")
#endif
#endif
/*---------------------------------------------------------------------------*/
#endif /* __BL_PROCESSOR_PAUSE_H__ */

