
#include "Utilities.h"

#include "ASM_Implementation.h"

using namespace asmi;

/*static*/ CInterpreterPtr CInterpreter::s_Singleton;

////////////////////////////////////////////////////////////////////////
//    MOVF_Instruction
////////////////////////////////////////////////////////////////////////

/*static*/ TAutoRegister<MOVF_Instruction> MOVF_Instruction::s_Register;

/*virtual*/ void MOVF_Instruction::Run()
{
	size_t arg = GetNextArgument();

	GetAccumulator() = GetArgument(arg);
}

/*virtual*/ size_t MOVF_Instruction::GetOpCode() const
{
	return 0;
}

/*virtual*/ size_t MOVF_Instruction::GetInstructionSize() const
{
	return 2;
}

////////////////////////////////////////////////////////////////////////
//    MOVT_Instruction
////////////////////////////////////////////////////////////////////////

/*static*/ TAutoRegister<MOVT_Instruction> MOVT_Instruction::s_Register;

/*virtual*/ void MOVT_Instruction::Run()
{
	SetArgument(GetNextArgument(), GetAccumulator());
}

/*virtual*/ size_t MOVT_Instruction::GetOpCode() const
{
	return 1;
}

/*virtual*/ size_t MOVT_Instruction::GetInstructionSize() const
{
	return 2;
}

////////////////////////////////////////////////////////////////////////
//    ADD_Instruction
////////////////////////////////////////////////////////////////////////

/*static*/ TAutoRegister<ADD_Instruction> ADD_Instruction::s_Register;

/*virtual*/ void ADD_Instruction::Run()
{
	GetAccumulator() += GetArgument(GetNextArgument());
}

/*virtual*/ size_t ADD_Instruction::GetOpCode() const
{
	return 2;
}

/*virtual*/ size_t ADD_Instruction::GetInstructionSize() const
{
	return 2;
}

////////////////////////////////////////////////////////////////////////
//    SUB_Instruction
////////////////////////////////////////////////////////////////////////

/*static*/ TAutoRegister<SUB_Instruction> SUB_Instruction::s_Register;

/*virtual*/ void SUB_Instruction::Run()
{
	GetAccumulator() -= GetArgument(GetNextArgument());
}

/*virtual*/ size_t SUB_Instruction::GetOpCode() const
{
	return 3;
}

/*virtual*/ size_t SUB_Instruction::GetInstructionSize() const
{
	return 2;
}

////////////////////////////////////////////////////////////////////////
//    MUL_Instruction
////////////////////////////////////////////////////////////////////////

/*static*/ TAutoRegister<MUL_Instruction> MUL_Instruction::s_Register;

/*virtual*/ void MUL_Instruction::Run()
{
	GetAccumulator() *= GetArgument(GetNextArgument());
}

/*virtual*/ size_t MUL_Instruction::GetOpCode() const
{
	return 4;
}

/*virtual*/ size_t MUL_Instruction::GetInstructionSize() const
{
	return 2;
}

////////////////////////////////////////////////////////////////////////
//    JZ_Instruction
////////////////////////////////////////////////////////////////////////

/*static*/ TAutoRegister<JZ_Instruction> JZ_Instruction::s_Register;

/*virtual*/ void JZ_Instruction::Run()
{
	if (GetAccumulator())
		return;

	GetInstructionCounter() = GetNextArgument();
	--GetInstructionCounter();
}

/*virtual*/ size_t JZ_Instruction::GetOpCode() const
{
	return 5;
}

/*virtual*/ size_t JZ_Instruction::GetInstructionSize() const
{
	return 2;
}

////////////////////////////////////////////////////////////////////////
//    POP_Instruction
////////////////////////////////////////////////////////////////////////

/*static*/ TAutoRegister<POP_Instruction> POP_Instruction::s_Register;

/*virtual*/ void POP_Instruction::Run()
{
	if(GetStackPointer() != GetStackBottom())
		--GetStackPointer();

	GetAccumulator() = *GetStackPointer();
}

/*virtual*/ size_t POP_Instruction::GetOpCode() const
{
	return 6;
}

/*virtual*/ size_t POP_Instruction::GetInstructionSize() const
{
	return 1;
}

////////////////////////////////////////////////////////////////////////
//    PUSH_Instruction
////////////////////////////////////////////////////////////////////////

/*static*/ TAutoRegister<PUSH_Instruction> PUSH_Instruction::s_Register;

/*virtual*/ void PUSH_Instruction::Run()
{
	*GetStackPointer() = GetAccumulator();
	
	if(GetStackPointer() != GetStackTop())
		++GetStackPointer();
}

/*virtual*/ size_t PUSH_Instruction::GetOpCode() const
{
	return 7;
}

/*virtual*/ size_t PUSH_Instruction::GetInstructionSize() const
{
	return 1;
}
