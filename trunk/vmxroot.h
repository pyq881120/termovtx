#pragma once

namespace vmxroot 
{

	enum cpu {Athlon,Intel,unsup};
	cpu CheckCPU();

	namespace detail
	{
		enum regs { regEax, regEbx, regEcx, regEdx };

		/**
		*	@brief LBR Branch Record Layout for the Pentium M Processor (and AMD cpus)
		**/
		struct branch_trace_record_x86
		{
			uint32_t  From;
			uint32_t  To;
		};

		/**
		*	@brief LBR Trace Record Layout for the P4+ (or core2+?)
		**/
		struct branch_trace_record
		{
			uint64_t  From;
			uint64_t  To;
			uint32_t  reserved1:4;
			uint32_t  Predicted:1;
			uint32_t  reserved2:27;
			uint32_t  reserved3;
		};

		/**
		*	@brief PEBS record layout
		**/
		struct pebs_record
		{
			uint64_t  RFlags;
			uint64_t  RIP;
			uint64_t  RAX;
			uint64_t  RBX;
			uint64_t  RCX;
			uint64_t  RDX;
			uint64_t  RSI;
			uint64_t  RDI;
			uint64_t  RBP;
			uint64_t  RSP;
			uint64_t  R8;
			uint64_t  R9;
			uint64_t  R10;
			uint64_t  R11;
			uint64_t  R12;
			uint64_t  R13;
			uint64_t  R14;
			uint64_t  R15;
		};

		STATIC_ASSERT(sizeof(branch_trace_record) == sizeof(uint64_t)*3);
		STATIC_ASSERT(sizeof(pebs_record) == 0x88+sizeof(uint64_t));

	} // detail

    namespace vmxintel
    {
        bool CheckHardware();
        void Entering();
        void Leaving();
    }

    namespace vmxamd
    {
        bool CheckHardware();
        void Entering();
        void Leaving();
    }

} // vmxroot