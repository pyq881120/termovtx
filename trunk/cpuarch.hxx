#pragma once

namespace ntl {

  typedef uint64_t msr_t;
#ifdef _M_IX86
  typedef uint32_t reg_t;
#else
  typedef uint64_t reg_t;
#endif

  namespace intrinsic {

    extern "C" {
      void __cdecl __cpuid(int32_t CPUInfo[4], int32_t InfoType);


      reg_t __cdecl __readcr0();
      reg_t __cdecl __readcr2();
      reg_t __cdecl __readcr3();
      reg_t __cdecl __readcr4();

      void __cdecl __writecr0(reg_t value);
      void __cdecl __writecr2(reg_t value);
      void __cdecl __writecr3(reg_t value);
      void __cdecl __writecr4(reg_t value);

#ifdef _M_X64
      reg_t __cdecl __readcr8();
      void __cdecl __writecr8(reg_t value);
#   pragma intrinsic(__readcr8, __readcr8)
#endif

      uint64_t __cdecl __readmsr(uint32_t msr_no);
      void __cdecl __writemsr(uint32_t msr_no, msr_t value);

      // vmx
      uint8_t __cdecl __vmx_on(uint64_t *VmsSupportPhysicalAddress);
      void __cdecl __vmx_off();
      uint8_t __cdecl __vmx_vmclear(uint64_t *VmcsPhysicalAddress);

      uint8_t __cdecl __vmx_vmlaunch();
      uint8_t __cdecl __vmx_vmresume();

      uint8_t __cdecl __vmx_vmptrld(uint64_t *VmcsPhysicalAddress);
      void __cdecl __vmx_vmptrst(uint64_t *VmcsPhysicalAddress);

      uint8_t __cdecl __vmx_vmread(size_t Field, size_t* FieldValue);
      uint8_t __cdecl __vmx_vmwrite(size_t Field, size_t FieldValue);

      // SVM
      void __svm_vmrun(uintptr_t VmcbPhysicalAddress);

      void __svm_clgi(void);
      void __svm_stgi(void);

      void _enable(void);
      void _disable(void);

#pragma intrinsic(__svm_vmrun,__svm_clgi, __svm_stgi, _enable, _disable )

      inline void __sti() { _enable(); };
      inline void __cli() { _disable(); };
    }



#pragma intrinsic(__cpuid, __readmsr, __writemsr)
#pragma intrinsic(__readcr0,__readcr2,__readcr3,__readcr4)

#ifdef _M_X64
#pragma intrinsic(__vmx_on, __vmx_off, __vmx_vmclear, __vmx_vmlaunch, __vmx_vmresume, __vmx_vmptrld, __vmx_vmptrst, __vmx_vmread, __vmx_vmwrite)
#endif
  }

  namespace km {

      struct segment_selector
      {
          uint32_t selector:16;
          union attributes
          {
              uint32_t value:16;
              struct
              {
                  uint32_t TYPE:4;              
                  uint32_t S:1;                 
                  uint32_t DPL:2;               
                  uint32_t P:1;      
                  uint32_t AVL:1;   
                  uint32_t L:1;       
                  uint32_t DB:1;          
                  uint32_t G:1;            
                  uint32_t GAP:4;
              };
          };
          uint32_t limit;
          uint64_t base;
      };

    namespace MSR {

#pragma pack(push,1)

      namespace IA32 {
        // CPU Family:Model is 06:0F
        namespace Core2 {
          enum MSR {
            TSC                 = 0x0010,
            ApicBase            = 0x001B,
            FeatureControl      = 0x003A,
            LastBranchFromIP0   = 0x0040,
            LastBranchFromIP1,
            LastBranchFromIP2,
            LastBranchFromIP3,
            LastBranchToIP0     = 0x0060,
            LastBranchToIP1,
            LastBranchToIP2,
            LastBranchToIP3,
            MiscEnable          = 0x01A0,
            DebugCtl            = 0x01D9,
            LastExceptionFromIP = 0x01DD,
            LastExceptionToIP,
            LastBranchTos       = 0x01C9,
          };

          struct misc_enable
          {
            // MiscEnable = 0x01A0
            union {
              msr_t value;
              struct {
                /** When set, the fast-strings feature (for REP MOVS and REP STORS) is enabled (default); 
                when clear, fast-strings are disabled. */
                uint32_t  FastStringsEnable:1;
                uint32_t  reserved1:2;
                /** Automatic Thermal Control Circuit Enable. */
                uint32_t  TCCEnable:1;
                uint32_t  reserved2:3;
                /** Performance Monitoring Available. (R) */
                uint32_t  PerfomanceMonitoring:1;
                uint32_t  reserved3:1;
                /** Hardware Prefetcher Disable. */
                uint32_t  HardwarePrefetcherDisable:1;
                /** FERR# Multiplexing Enable. */
                uint32_t  FerrMultiplexingEnable:1;
                /** Branch Trace Storage Unavailable. (RO) */
                uint32_t  BTS:1;
                /** Precise Event Based Sampling (PEBS) Unavailable. (RO) */
                uint32_t  PEBS:1;
                /** TM2 Enable. */
                uint32_t  TM2Enable:1;
                uint32_t  reserved4:2;
                /** Enhanced Intel SpeedStep Technology Enable. (R/W) */
                uint32_t  ISSTEnable:1;
                uint32_t  reserved5:1;
                /** Enable monitor FSM */
                uint32_t  FSMEnable:1;
                /** Adjacent Cache Line Prefetch Disable. */
                uint32_t  ACLPDisable:1;
                /** Enhanced Intel SpeedStep Technology Select Lock. (R/WO) */
                uint32_t  ISSTLock:1;
                uint32_t  reserved6:1;
                /** Limit CPUID Maxval. (R/W) */
                uint32_t  LimitCpuidMax:1;
                /** xTPR Message Disable. */
                uint32_t  XTRRDisable:1;
                uint32_t  reserved7:8;
                uint32_t  reserved8:2;
                /** XD Bit Disable. */
                uint32_t  XDDisable:1;
                uint32_t  reserved9:2;
                /** DCU Prefetcher Disable */
                uint32_t  DCUPrefetcherDisable:1;
                /** IDA Disable */
                uint32_t  IDADisable:1;
                /** IP Prefetcher Disable */
                uint32_t  IPPrefetcherDisable:1;
              };
            };
          };

        } // Core2

        namespace P4 {
          enum MSR   {
            TSC                 = 0x0010,
            ApicBase            = 0x001B,
            FeatureControl      = 0x003A,

            MiscEnable          = 0x01A0,
            DebugCtl            = 0x01D9,

            /** Last Branch Record 0. (R/W). Model 0-2 */
            LastBranch0         = 0x01DB,
            LastBranch1,
            LastBranch2,
            LastBranch3,

            /** Last Branch Record 0. (R/W). Model 3-6 */
            LastBranchFromLIP0   = 0x0680,
            LastBranchFromLIP1,
            LastBranchFromLIP2,
            LastBranchFromLIP3,
            LastBranchFromLIP4,
            LastBranchFromLIP5,
            LastBranchFromLIP6,
            LastBranchFromLIP7,
            LastBranchFromLIP8,
            LastBranchFromLIP9,
            LastBranchFromLIP10,
            LastBranchFromLIP11,
            LastBranchFromLIP12,
            LastBranchFromLIP13,
            LastBranchFromLIP14,
            LastBranchFromLIP15,

            LastBranchToLIP0     = 0x06C0,
            LastBranchToLIP1,
            LastBranchToLIP2,
            LastBranchToLIP3,
            LastBranchToLIP4,
            LastBranchToLIP5,
            LastBranchToLIP6,
            LastBranchToLIP7,
            LastBranchToLIP8,
            LastBranchToLIP9,
            LastBranchToLIP10,
            LastBranchToLIP11,
            LastBranchToLIP12,
            LastBranchToLIP13,
            LastBranchToLIP14,
            LastBranchToLIP15,

            LastExceptionFromIP = 0x01D7,
            LastExceptionToIP,
            /** Last branch record top-of-stack (TOS) pointer. 
            2 bits (0-3) for models 1-2, 4 bits (0-15) for model 3. */
            LastBranchTos       = 0x01DA,
          };

          struct misc_enable
          {
            // MiscEnable = 0x01A0
            union {
              msr_t value;
              struct {
                uint32_t  FastStringsEnable:1;
                uint32_t  reserved1:1;
                uint32_t  FPUFopcodeCompat:1;
                uint32_t  TCCEnable:1;
                uint32_t  SplitLockEnable:1;
                uint32_t  reserved2:1;
                uint32_t  L3Disable:1;
                uint32_t  PerfomanceMonitoring:1;
                uint32_t  SuppressLockEnable:1;
                uint32_t  PrefetchQueueDisable:1;
                uint32_t  FerrReportEnable:1;
                uint32_t  BTS:1;
                uint32_t  PEBS:1;
                uint32_t  TM2Enable:1;
                uint32_t  reserved3:4;
                uint32_t  FSMEnable:1;
                /** Adjacent Cache Line Prefetch Disable. (R/W)  */
                uint32_t  ACLPDisable:1;
                uint32_t  reserved5:2;
                uint32_t  LimitCpuidMax:1;
                uint32_t  XTRRDisable:1;
                uint32_t  L1DataCacheContextMode:1;
                uint32_t  reserved7:7;
                uint32_t  reserved8:2;
                uint32_t  XDDisable:1;
              };
            };
          };
        } // P4

        // Common MSR's
        enum MSR {
          TSC               = 0x0010,     // in practical, it's value is common to all processors
          ApicBase          = 0x001B,
          SysenterCs        = 0x0174,
          SysenterEsp,
          SysenterEip,
          MiscEnable        = 0x01A0,
          DebugCtl          = 0x01D9,
          PerfCapabilities  = 0x0345,
          PebsEnable        = 0x04F1,
          VmxBasic          = 0x0480,
          VmxPinbasedCtls,
          VmxProcbasedCtls,
          VmxExitCtls,
          VmxEntryCtls,
          VmxMisc,
          VmxCr0Fixed0,
          VmxCr0Fixed1,
          VmxCr4Fixed0,
          VmxCr4Fixed1,
          VmxVmcsEnum,
          VmxProcbasedCtls2,
          // 0F:00+
          DSArea            = 0x600,

          Efer              = 0xC0000080,
          Star,
          Lstar,
          FSBase            = 0xC0000100,
          GSBase,
          KernelGSBase
        };

        struct feature_control
        {
          // FeatureControl
          union {
            msr_t value;
            struct {
              uint32_t VmxLock:1;
              uint32_t EnableInsideSmx:1;
              uint32_t EnableOutsideSmx:1;
              uint32_t reserved1:5;
              uint32_t SenterLocalEnable:7;
              uint32_t SenterGlocalEnable:1;
              uint32_t reserved2:16;
              uint32_t reserved3;
            };
          };
        };


        struct debug_ctl
        {
          // DebugCtl
          union {
            msr_t value;
            struct {
              /** Enables the processor to record a running trace of the most recent 
              branches taken by the processor in the LBR stack. */
              uint32_t  LBR:1;
              /** Enables the processor to treate EFLAGS.TF as single-step on 
              branches instead of single-step on instructions */
              uint32_t  BTF:1;
              /** Performance monitoring/breakpoint pin flags. PM only */
              uint32_t  PB0:1;
              uint32_t  PB1:1;
              uint32_t  PB2:1;
              uint32_t  PB3:1;
              /** Enables branch trace messages to be sent */
              uint32_t  TR:1;
              // Following on PIV+
              /** Enables branch trace messages (BTMs) to be logged in a BTS buffer */
              uint32_t  BTS:1;
              /** When clear, BTMs are logged in a BTS buffer in circular fashion. When this bit is set, an 
              interrupt is generated by the BTS facility when the BTS buffer is full. */
              uint32_t  BTINT:1;
              // Following on PIV (NetBurst)
              /** When set, BTS or BTM is skepped if CPL = 0. */
              uint32_t  BTF_OFF_OS:1;
              /** When set, BTS or BTM is skepped if CPL > 0. */
              uint32_t  BTF_OFF_USR:1;
              /** When set, the LBR stack is frozen on a PMI request. If CPUID.01H:ECX[15] = 1. */
              uint32_t  FreezeLibsOnPmi:1;
              /** When set, each ENABLE bit of the global counter control MSR are frozen (address 3BFH) on a 
              PMI request. If CPUID.01H:ECX[15] = 1. */
              uint32_t  FreezePerfmonOnPmi:1;
              uint32_t  reserved2:1;
              /** When set, freezes perfmon and trace messages while in SMM */
              uint32_t  FreezeOnSmm:1;
            };
          };
        };

        struct perf_capabilities
        {
          // PerfCapabilities  = 0x0345
          enum LBRFormatType {
            LBRFormatIP32,
            LBRFormatLIP64,
            LBRFormatIP64
          };

          union {
            msr_t value;
            struct {
              uint32_t    LBRFormat:6;
              uint32_t    PEBSTrap:1;
              uint32_t    PEBSSaveArchRegs:1;
              uint32_t    PEBSRecordFormat:4;
              uint32_t    FreezeOnSmm:1;
            };
          };
        };

        struct vmx_basic
        {
          enum memory_type {
            UC,
            WM = 6
          };
          union {
            msr_t value;
            struct {
              uint32_t Revision;
              uint32_t RegionSize:12;
              uint32_t RegionSizeZero:1;
              uint32_t reserved1:3;
              uint32_t PhysicalAddressLimit:1;
              uint32_t SmmSupport:1;
              uint32_t MemoryType:4;
              uint32_t InstrInfoSupported:1;
              uint32_t reserved2:9;
            };
          };
        };

        struct vmx_ctls
        {
          union {
            msr_t value;
            struct {
              uint32_t AllowedZero;
              uint32_t AllowedNonzero;
            };
          };
        };

        struct vmx_misc
        {
          union {
            msr_t value;
            struct {
              uint8_t reserved1:6;
              uint8_t ActivityHlt:1;
              uint8_t ActivityShudown:1;
              uint8_t ActivitySipiWait:1;
              uint8_t reserved2:7;
              uint8_t SupportedCR3Zero:1;
              uint8_t MsrListMaxCount:3;
              uint8_t reserved3:4;
              uint32_t MsegRevision;
            };
          };
        };

        struct vmx_vmcs_enum
        {
          union {
            msr_t value;
            struct {
              uint32_t reserved1:1;
              uint32_t HighIndex:9;
              uint32_t reserved2:6;
              uint32_t reserved3;
            };
          };
        };

        struct efer
        {
          union {
              msr_t value;
              struct {
                  uint32_t SyscallEnable:1;
                  uint32_t reserved1:7;
                  uint32_t LME:1;
                  uint32_t reserved2:1;
                  uint32_t LMA:1;
                  uint32_t ExecuteDisableBit:1;
                  uint32_t reserved3:20;
                  uint32_t reserved4;
            };
          };
        };


        STATIC_ASSERT(sizeof(feature_control) == sizeof(uint64_t));
        STATIC_ASSERT(sizeof(Core2::misc_enable) == sizeof(uint64_t));
        STATIC_ASSERT(sizeof(debug_ctl) == sizeof(uint64_t));
        STATIC_ASSERT(sizeof(perf_capabilities) == sizeof(uint64_t));
        STATIC_ASSERT(sizeof(vmx_basic) == sizeof(uint64_t));
        STATIC_ASSERT(sizeof(vmx_ctls) == sizeof(uint64_t));
        STATIC_ASSERT(sizeof(vmx_misc) == sizeof(uint64_t));
        STATIC_ASSERT(sizeof(vmx_vmcs_enum) == sizeof(uint64_t));
        STATIC_ASSERT(sizeof(efer) == sizeof(uint64_t));

      } // IA32

    } // MSR

    namespace AMD {

        // AMD MSR's
        enum MSR {
            TSC               = 0x0010,     // in practical, it's value is common to all processors
            ApicBase          = 0x001B,
            MTRRCap           = 0x00FE,
            SysenterCs        = 0x0174,
            SysenterEsp,
            SysenterEip,
            DebugCtl          = 0x01D9,

            LastBranchFromIP   = 0x01DB,
            LastBranchToIP,
            LastExceptionFromIP,
            LastExceptionToIP,

            Efer              = 0xC0000080,
            Star,
            Lstar,
            Cstar,
            SF_MASK,
            FSBase            = 0xC0000100,
            GSBase,
            KernelGSBase,

            MCG_CAP             = 0x0179,
            MCG_STATUS          = 0x017A,
            MCG_CTL             = 0x017B,

            MTRRphysBase0       = 0x0200,
            MTRRphysMask0       = 0x0201,
            MTRRphysBase1       = 0x0202,
            MTRRphysMask1       = 0x0203,
            MTRRphysBase2       = 0x0204,
            MTRRphysMask2       = 0x0205,
            MTRRphysBase3       = 0x0206,
            MTRRphysMask3       = 0x0207,
            MTRRphysBase4       = 0x0208,
            MTRRphysMask4       = 0x0209,
            MTRRphysBase5       = 0x020A,
            MTRRphysMask5       = 0x020B,
            MTRRphysBase6       = 0x020C,
            MTRRphysMask6       = 0x020D,
            MTRRphysBase7       = 0x020E,
            MTRRphysMask7       = 0x020F,
            MTRRfix64K_00000    = 0x0250,
            MTRRfix16K_80000    = 0x0258,
            MTRRfix16K_A0000    = 0x0259,
            MTRRfix4K_C0000     = 0x0268,
            MTRRfix4K_C8000     = 0x0269,
            MTRRfix4K_D0000     = 0x026A,
            MTRRfix4K_D8000     = 0x026B,
            MTRRfix4K_E0000     = 0x026C,
            MTRRfix4K_E8000     = 0x026D,
            MTRRfix4K_F0000     = 0x026E,
            MTRRfix4K_F8000     = 0x026F,
            PAT                 = 0x0277,
            MTRRdefType         = 0x02FF,
            MC0_CTL             = 0x0400,
            MC1_CTL             = 0x0404,
            MC2_CTL             = 0x0408,
            MC3_CTL             = 0x040C,
            MC4_CTL             = 0x0410,
            MC0_STATUS          = 0x0401,
            MC1_STATUS          = 0x0405,
            MC2_STATUS          = 0x0409,
            MC3_STATUS          = 0x040D,
            MC4_STATUS          = 0x0411,
            MC0_ADDR            = 0x0402,
            MC1_ADDR            = 0x0406,
            MC2_ADDR            = 0x040A,
            MC3_ADDR            = 0x040E,
            MC4_ADDR            = 0x0412,
            MC0_MISC            = 0x0403,
            MC1_MISC            = 0x0407,
            MC2_MISC            = 0x040B,
            MC3_MISC            = 0x040F,
            MC4_MISC            = 0x0413,
            TSC_AUX             = 0xC0000103,
            PerfEvtSel0         = 0xC0010000,
            PerfEvtSel1         = 0xC0010001,
            PerfEvtSel2         = 0xC0010002,
            PerfEvtSel3         = 0xC0010003,
            PerfCtr0            = 0xC0010004,
            PerfCtr1            = 0xC0010005,
            PerfCtr2            = 0xC0010006,
            PerfCtr3            = 0xC0010007,
            SYSCFG              = 0xC0010010,
            IORRBase0           = 0xC0010016,
            IORRMask0           = 0xC0010017,
            IORRBase1           = 0xC0010018,
            IORRMask1           = 0xC0010019,
            TOP_MEM             = 0xC001001A,
            TOP_MEM2            = 0xC001001D,
            VM_CR               = 0xC0010114,
            IGNNE               = 0xC0010115,
            SMM_CTL             = 0xC0010116,
            VM_HSAVE_PA         = 0xC0010117
        };

        struct debug_ctl
        {
            // DebugCtl
            union {
                msr_t value;
                struct {
                    /** Software sets this bit to 1 to cause the processor to
                    record the source and target addresses of the last control transfer taken before a debug exception
                    occurs. The recorded control transfers include branch instructions, interrupts, and exceptions */
                    uint32_t  LBR:1;
                    /** Software uses this bit to change the behavior of the
                    rFLAGS.TF bit. When this bit is cleared to 0, the rFLAGS.TF bit controls instruction single
                    stepping, (normal behavior). When this bit is set to 1, the rFLAGS.TF bit controls single stepping
                    on control transfers. The single-stepped control transfers include branch instructions, interrupts,
                    and exceptions. Control-transfer single stepping requires both BTF=1 and rFLAGS.TF=1. */
                    uint32_t  BTF:1;
                    /** Performance monitoring/breakpoint pin flags. AMD spec 
                    Software uses these bits to control the type of information reported by the four external 
                    performancemonitoring/breakpoint pins on the processor. When a PBi bit is cleared to 0, the corresponding
                    external pin (BPi) reports performance-monitor information. When a PBi bit is set to 1, the
                    corresponding external pin (BPi) reports breakpoint information.*/
                    uint32_t  PB0:1;
                    uint32_t  PB1:1;
                    uint32_t  PB2:1;
                    uint32_t  PB3:1;
                };
            };
        };

        struct efer
        {
            union {
                msr_t value;
                struct {
                    uint32_t SyscallEnable:1;
                    uint32_t reserved1:7;
                    uint32_t LME:1;
                    uint32_t reserved2:1;
                    uint32_t LMA:1;
                    uint32_t ExecuteDisableBit:1;
                    uint32_t SVME:1;
                    uint32_t reserved3:1;
                    uint32_t FFXSR:1;
                    uint32_t reserved4:17;
                    uint32_t reserved5;
                };
            };
        };

        struct vm_cr
        {
            union {
                msr_t value;
                struct{
                    uint32_t  DPD:1;
                    uint32_t  R_INIT:1;
                    uint32_t  DIS_A20M:1;
                    uint32_t  LOCK:1;
                    uint32_t  SVMDIS:1;
                    uint64_t  reserved1;
                };
            };
        };

        union eventinj
        {
            uint64_t eventinj_value;
            struct
            {
                uint32_t vector:8;
                uint32_t type:3;
                uint32_t ev:1;
                uint32_t reserved1:19;
                uint32_t v:1;
                uint32_t errorcode:32;
            };
        };

        struct vmcb
        {
            uint32_t cr_intercepts;        
            uint32_t dr_intercepts;        
            uint32_t excptvector_intercepts; 
            union {
                uint32_t general1_intercepts;
                struct{
                    uint32_t INTR: 1;
                    uint32_t NMI:  1;
                    uint32_t SMI:  1;
                    uint32_t INIT: 1;
                    uint32_t VINTR:1;
                    uint32_t CR0:  1;
                    uint32_t IDTRr:1;
                    uint32_t GDTRr:1;
                    uint32_t LDTRr:1;
                    uint32_t TRr:  1;
                    uint32_t IDTRw:1;
                    uint32_t GDTRw:1;
                    uint32_t LDTRw:1;
                    uint32_t TRw:  1;
                    uint32_t RDTCS:1;
                    uint32_t RDPMC:1;
                    uint32_t PUSHF:1;
                    uint32_t POPF: 1;
                    uint32_t CPUID:1;
                    uint32_t RSM:  1;
                    uint32_t IRET: 1;
                    uint32_t INTn: 1;
                    uint32_t INVD: 1;
                    uint32_t PAUSE:1;
                    uint32_t HLT:  1;
                    uint32_t INVLPG:1;
                    uint32_t INVLPGA:1;
                    uint32_t IOIO_PROT:1;
                    uint32_t MSR_PROT:1;
                    uint32_t FERR_FREEZE:1;
                    uint32_t shutdown_event:1;
                };
            };
            union {
                uint32_t general2_intercepts;
                struct{
                    uint32_t VMRUN:  1;
                    uint32_t VMMCALL:1;
                    uint32_t VMLOAD: 1;
                    uint32_t VMSAVE: 1;
                    uint32_t STGI:   1;
                    uint32_t CLGI:   1;
                    uint32_t SCINIT: 1;
                    uint32_t RDTSCP: 1;
                    uint32_t ICEBP:  1;
                    uint32_t WBINVD: 1;
                    uint32_t MONITOR:1;
                    uint32_t MWAIT:  1;
                    uint32_t MWAIT_mon:1;
                    uint32_t rezerved1:19;
                };
            };
            uint32_t reserved1;          
            uint64_t reserved2;           
            uint64_t reserved3;     
            uint64_t reserved4;  
            uint64_t reserved5;  
            uint64_t reserved6;  
            uint64_t IOPM_BASE_PA;    
            uint64_t MSRPM_BASE_PA;   
            uint64_t TSC_OFFSET;    
            uint32_t QUEST_ASID;     
            uint32_t TLB_CONTROL:8;    
            uint32_t reserved7:24;
            union vintr
            {
                uint64_t vintr_value;
                struct
                {
                    uint32_t V_TPR:8;
                    uint32_t V_IRQ:1;
                    uint32_t reserved1:7;
                    uint32_t V_INTR_PRIO:4;
                    uint32_t V_IGN_TPR:1;
                    uint32_t reserved2:3;
                    uint32_t V_INTR_MASKING:1;
                    uint32_t reserved3:7;
                    uint32_t V_INTR_VECTOR:8;
                    uint32_t reserved4:24;
                };
            };
            uint64_t INTERRUPT_SHADOW:1;     
            uint64_t reserved8:63;  
            uint64_t EXITCODE;           
            uint64_t EXITINFO1;            
            uint64_t EXITINFO2;            
            eventinj EXITINTINFO;     
            uint64_t NP_ENABLE;          
            uint64_t reserved9:63;
            uint64_t reserved10;
            eventinj EXITINJ;          
            uint64_t N_CR3;               
            uint64_t reserved11[105];          //400h

            // --- guest state area ---

            segment_selector es;          /* offset 1024 */
            segment_selector cs;
            segment_selector ss;
            segment_selector ds;
            segment_selector fs;
            segment_selector gs;
            segment_selector gdtr;
            segment_selector ldtr;
            segment_selector idtr;
            segment_selector tr;
            uint64_t reserved12[5];
            uint64_t reserved13:24;
            uint64_t CPL:8;
            uint32_t reserved14;
            efer EFER; 
            uint64_t reserved15[14];
            uint64_t _CR4;                  /* loffset 1024 + 0x148 */
            uint64_t _CR3;
            uint64_t _CR0;
            uint64_t dr7;
            uint64_t dr6;
            uint64_t rflags;               // 1024+0x170
            uint64_t rip;                  // 1024+0x178
            uint64_t reserved16[11];
            uint64_t rsp;                  // 1024+0x1d8
            uint64_t reserved17[3];
            uint64_t rax;
            uint64_t star;
            uint64_t lstar;
            uint64_t cstar;
            uint64_t sfmask;
            uint64_t kerngsbase;
            uint64_t sysenter_cs;
            uint64_t sysenter_esp;
            uint64_t sysenter_eip;
            uint64_t cr2;
            uint64_t pdpe0;
            uint64_t pdpe1;
            uint64_t pdpe2;
            uint64_t pdpe3;
            uint64_t g_pat;
            uint64_t res16[50];
            uint64_t res17[128];
            uint64_t res18[128];
        };
    }

#pragma pack(pop)

    namespace cpuarch {

#pragma pack(push,1)

      struct CR0
      {
        union {
          reg_t value;
          struct {
            uint32_t PE:1;
            uint32_t MP:1;
            uint32_t EM:1;
            uint32_t TS:1;
            uint32_t ET:1;
            uint32_t NE:1;
            uint32_t reserved1:10;
            uint32_t WP:1;
            uint32_t reserved2:1;
            uint32_t AM:1;
            uint32_t reserved3:10;
            uint32_t NW:1;
            uint32_t CD:1;
            uint32_t PG:1;
#ifdef _M_X64
            uint32_t reserved4;
#endif
          };
        };
      };

      struct CR3
      {
        union {
          reg_t value;
          struct {
            reg_t reserved1:3;
            reg_t PWT:1;
            reg_t PCD:1;
            reg_t reserved2:7;
#ifdef _M_IX86
            reg_t PageDirectoryBase:20;
#else
            reg_t PageDirectoryBase:52;
#endif
          };
        };
      };

      struct CR4
      {
        union {
          reg_t value;
          struct {
            uint32_t WEM:1;
            uint32_t PVI:1;
            uint32_t TSD:1;
            uint32_t DE:1;
            uint32_t PSE:1;
            uint32_t PAE:1;
            uint32_t MCE:1;
            uint32_t PGE:1;
            uint32_t PCE:1;
            uint32_t OSFXSR:1;
            uint32_t OSXMMEXCPT:1;
            uint32_t reserved1:2;
            uint32_t VMXE:1;
            uint32_t SMXE:1;
            uint32_t reserved2:17;
#ifdef _M_X64
            uint32_t reserved3;
#endif
          };
        };
      };

#ifdef _M_X64
      struct CR8
      {
        union {
          reg_t value;
          struct {
            uint32_t TPL:4;
            uint32_t reserved1:28;
            uint32_t reserved2;
          };
        };
      };
      STATIC_ASSERT(sizeof(CR8) == sizeof(void*));
#endif

      STATIC_ASSERT(sizeof(CR0) == sizeof(void*));
      STATIC_ASSERT(sizeof(CR3) == sizeof(void*));
      STATIC_ASSERT(sizeof(CR4) == sizeof(void*));

#pragma pack(pop)

    } // cpuarch
  } // km
} // ntl

