//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <stdint.h>

#include <functional>
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

enum cpuidEXX { eEAX, eEBX, eECX, eEDX };

#define STR(X) #X

// when EAX=0
#define CPUID_EAX_P4 		  	(0x02)
#define CPUID_EAX_P4_HT 	  	(0x05)
#define CPUID_EAX_PD 			(0x06)
#define CPUID_EAX_CORE_DUO 		(0x0A)
#define CPUID_EAX_CORE2_DUO 		(0x0A)
#define CPUID_EAX_XEON_3K 		(0x0A)
#define CPUID_EAX_CORE2_DUO_8K 		(0x0D)
#define CPUID_EAX_NAHALEM 		(0x0B)
#define CPUID_EAX_IVYBRIDGE 		(0x0D)
#define CPUID_EAX_SKYLAKE 		(0x16)

// when EAX=7 AND ECX=0
#define CPUID_EBX_AVX2 			(1 << 5)
#define CPUID_EBX_AVX512F 		(1 << 16)
#define CPUID_EBX_AVX512DQ 		(1 << 17)
#define CPUID_EBX_AVX512IFMA 		(1 << 21)
#define CPUID_EBX_AVX512PF 		(1 << 26)
#define CPUID_EBX_AVX512ER 		(1 << 27)
#define CPUID_EBX_AVX512BW 		(1 << 30)
#define CPUID_EBX_AVX512VL 		(1 << 31)

// when EAX=1
#define CPUID_ECX_SSE3 			(1 << 0)
#define CPUID_ECX_SSSE3 		(1 << 9)
#define CPUID_ECX_FMA 			(1 << 12)
#define CPUID_ECX_SSE41 		(1 << 19)
#define CPUID_ECX_SSE42 		(1 << 20)
#define CPUID_ECX_AES 			(1 << 25)
#define CPUID_ECX_XSAVE 		(1 << 26)
#define CPUID_ECX_AVX 			(1 << 28)
#define CPUID_ECX_F16C 			(1 << 29)
#define CPUID_ECX_HYPERVISOR 		(1 << 31)

// when EAX=80000001
#define CPUID_ECX_SSE4A 		(1 << 6)

// when EAX=80000001
#define CPUID_EDX_MMX_EX 		(1 << 22)
#define CPUID_EDX_MMX 			(1 << 23)
#define CPUID_EDX_SSE 			(1 << 25)
#define CPUID_EDX_SSE2 			(1 << 26)
#define CPUID_EDX_INTEL64 		(1 << 29)
#define CPUID_EDX_XD 			(1 << 20)
#define CPUID_EDX_3DNOW 		(1 << 30)
#define CPUID_EDX_3DNOW_EX 		(1 << 31)
// when EAX=16H
#define EFLAGS_ID_BIT 			(1 << 21)

int checkCpuidAvailable() {
  uint64_t eflags1 = 0, eflags2 = 0;
  __asm__ __volatile__("pushfq\n"
                       "popq %%rax\n"
                       "movq %%rax, %0\n"
                       "xorq $0x00200000, %%rax\n"
                       "pushq %%rax\n"
                       "popfq\n"
                       "pushfq\n"
                       "popq %%rax\n"
                       "movq %%rax,%1\n"
                       "pushq %%rax\n"
                       "popfq\n"
                       : "=g"(eflags1), "=g"(eflags2)
                       :
                       : "%rax");
  return EFLAGS_ID_BIT & (eflags1 ^ eflags2);
}

std::vector<std::tuple<cpuidEXX, uint32_t, const char*>> cpuidInfo{
    std::make_tuple(eEAX, CPUID_EAX_P4, 		STR( CPUID_EAX_P4 	    )),
    std::make_tuple(eEAX, CPUID_EAX_P4, 		STR( CPUID_EAX_P4 	    )),
    std::make_tuple(eEAX, CPUID_EAX_P4, 		STR( CPUID_EAX_P4 	    )),
    std::make_tuple(eEAX, CPUID_EAX_P4_HT, 		STR( CPUID_EAX_P4_HT 	    )),
    std::make_tuple(eEAX, CPUID_EAX_PD, 		STR( CPUID_EAX_PD 	    )),
    std::make_tuple(eEAX, CPUID_EAX_CORE_DUO, 		STR( CPUID_EAX_CORE_DUO     )),
    std::make_tuple(eEAX, CPUID_EAX_CORE2_DUO, 		STR( CPUID_EAX_CORE2_DUO    )),
    std::make_tuple(eEAX, CPUID_EAX_XEON_3K, 		STR( CPUID_EAX_XEON_3K      )),
    std::make_tuple(eEAX, CPUID_EAX_CORE2_DUO_8K, 	STR( CPUID_EAX_CORE2_DUO_8K )),
    std::make_tuple(eEAX, CPUID_EAX_NAHALEM, 		STR( CPUID_EAX_NAHALEM      )),
    std::make_tuple(eEAX, CPUID_EAX_IVYBRIDGE, 		STR( CPUID_EAX_IVYBRIDGE    )),
    std::make_tuple(eEAX, CPUID_EAX_SKYLAKE, 		STR( CPUID_EAX_SKYLAKE      )),

    std::make_tuple(eEBX, CPUID_EBX_AVX2, 		STR( CPUID_EBX_AVX2         )),
    std::make_tuple(eEBX, CPUID_EBX_AVX512F, 		STR( CPUID_EBX_AVX512F      )),
    std::make_tuple(eEBX, CPUID_EBX_AVX512DQ, 		STR( CPUID_EBX_AVX512DQ     )),
    std::make_tuple(eEBX, CPUID_EBX_AVX512IFMA, 	STR( CPUID_EBX_AVX512IFMA   )),
    std::make_tuple(eEBX, CPUID_EBX_AVX512PF, 		STR( CPUID_EBX_AVX512PF     )),
    std::make_tuple(eEBX, CPUID_EBX_AVX512ER, 		STR( CPUID_EBX_AVX512ER     )),
    std::make_tuple(eEBX, CPUID_EBX_AVX512BW, 		STR( CPUID_EBX_AVX512BW     )),
    std::make_tuple(eEBX, CPUID_EBX_AVX512VL, 		STR( CPUID_EBX_AVX512VL     )),

    std::make_tuple(eECX, CPUID_ECX_SSE3, 		STR( CPUID_ECX_SSE3 	    )),
    std::make_tuple(eECX, CPUID_ECX_SSSE3, 		STR( CPUID_ECX_SSSE3   	    )),
    std::make_tuple(eECX, CPUID_ECX_SSE4A, 		STR( CPUID_ECX_SSE4A	    )),
    std::make_tuple(eECX, CPUID_ECX_FMA, 		STR( CPUID_ECX_FMA          )),
    std::make_tuple(eECX, CPUID_ECX_SSE41, 		STR( CPUID_ECX_SSE41        )),
    std::make_tuple(eECX, CPUID_ECX_SSE42, 		STR( CPUID_ECX_SSE42        )),
    std::make_tuple(eECX, CPUID_ECX_AES, 		STR( CPUID_ECX_AES          )),
    std::make_tuple(eECX, CPUID_ECX_XSAVE, 		STR( CPUID_ECX_XSAVE        )),
    std::make_tuple(eECX, CPUID_ECX_AVX, 		STR( CPUID_ECX_AVX          )),
    std::make_tuple(eECX, CPUID_ECX_F16C, 		STR( CPUID_ECX_F16C         )),
    std::make_tuple(eECX, CPUID_ECX_HYPERVISOR, 	STR( CPUID_ECX_HYPERVISOR   )),

    std::make_tuple(eEDX, CPUID_EDX_MMX, 		STR( CPUID_EDX_MMX          )),
    std::make_tuple(eEDX, CPUID_EDX_SSE, 		STR( CPUID_EDX_SSE          )),
    std::make_tuple(eEDX, CPUID_EDX_SSE2, 		STR( CPUID_EDX_SSE2         )),
    std::make_tuple(eEDX, CPUID_EDX_INTEL64, 		STR( CPUID_EDX_INTEL64      )),
    std::make_tuple(eEDX, CPUID_EDX_XD, 		STR( CPUID_EDX_XD           ))  };


void streamInfo(cpuidEXX regEXX, uint32_t output) {
  std::cout << std::endl;
  for (auto f : cpuidInfo) {
    if ((std::get<0>(f) == regEXX) && ((output & std::get<1>(f)) != 0)) {
      //      std::cout << "This CPU supports " << std::get<2>(f) << std::endl;
      std::cout << "    " << std::get<2>(f) << std::endl;
    }
  }
}


int main() {
  uint32_t output = 0;
  uint32_t input  = 0;

#if 0   
  if(checkCpuidAvailable()){
    std::cout << "CPUID is not available!!" << endl ;
    return 1;
  } else {
    std::cout << "CPUID support found" << endl ;
  }
#endif

  std::cout << "\nSUPPORTED FEATURES" << std::endl;

  input = 0;
  __asm__("cpuid;"
          : "=a"(output)                // EAX into output
          : "a"(input)                  // input into EAX
          : "%ebx", "%ecx", "%edx");    // clobbered registers
  streamInfo(eEAX, output);

  input = 7;
  __asm__("xorl %%ecx,%%ecx ;"    // ECX = 0
          "cpuid;"
          : "=b"(output)        // EAX into output
          : "a"(input)          // i input into EAX
          : "%ecx", "%edx");    // clobbered registers
  streamInfo(eEBX, output);

  input = 1;
  __asm__("cpuid;"
          : "=c"(output)        // EAX into output
          : "a"(input)          // i input into EAX
          : "%ebx", "%edx");    // clobbered registers
  streamInfo(eECX, output);

  input = 0x80000001;
  __asm__("cpuid;"
          : "=d"(output)        // EAX into output
          : "a"(input)          // input into EAX
          : "%ebx", "%ecx");    // clobbered registers
  streamInfo(eEDX, output);

  input = 0x16;
  uint16_t cpufBase;
  uint16_t cpufMax;
  uint16_t cpufBus;
  __asm__("cpuid;"
          : "=a"(cpufBase), "=b"(cpufMax), "=c"(cpufBus)    // EAX into output
          : "a"(input)                                      // input into EAX
          : "%edx");                                        // clobbered registers

  std::cout << std::endl << "    ";
  std::cout << "Basefreq : " << cpufBase << " MHz ";
  std::cout << "Maxfreq : "  << cpufMax  << " MHz ";
  std::cout << "Busfreq : "  << cpufBus  << " MHz" << endl << endl;

  return 0;
}
