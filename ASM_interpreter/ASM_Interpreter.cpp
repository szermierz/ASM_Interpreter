
#include "Utilities.h"

#include "ASM_Interpreter.h"

using namespace asmi;

////////////////////////////////////////////////////////////////////////
//    CInterpreter
////////////////////////////////////////////////////////////////////////

/*static*/ CInterpreterPtr CInterpreter::GetSingleton()
{
	if (!s_Singleton)
		s_Singleton = make_shared<CInterpreter>();

	return s_Singleton;
}

void CInterpreter::Initialize()
{
	InitializeMemory();
	InitializeInstructions();
}

void CInterpreter::RunProgram()
{
	m_InstructionCounter = 0;
	--m_InstructionCounter;
	m_Accumulator = 0;

	for (;;)
	{
		if (++m_InstructionCounter == m_Memory.size())
			break;

		MemoryUnit& currentMemory = m_Memory[m_InstructionCounter];
		m_CurrentOpCode = currentMemory.first;
		m_CurrentArg = currentMemory.second;

		m_InstructionList[m_CurrentOpCode]->Run();	
	}
}

void CInterpreter::InitializeMemory()
{
	m_Memory.resize(PROGRAM_SIZE);

	m_Stack.resize(STACK_SIZE);
	m_StackPointer = &m_Stack.front();
}

void CInterpreter::InitializeInstructions()
{
	for (auto it : m_InstructionList)
		it->Initialize(m_Accumulator, m_Memory, m_InstructionCounter, m_CurrentOpCode, m_CurrentArg, m_StackPointer);
}

////////////////////////////////////////////////////////////////////////
//    CInstructionFactory
////////////////////////////////////////////////////////////////////////

/*static*/ void CInstructionFactory::AddInstruction(vector<CInstructionBasePtr>& destInstructionList, CInstructionBasePtr instruction)
{
	if (!instruction)
		return;

	size_t opCode = instruction->GetOpCode();
	if (destInstructionList.size() <= opCode)
		destInstructionList.resize(opCode + 1);

	//todo wywalic to nizej albo cokolwiek
	if (destInstructionList[opCode])
	{
		for (size_t i = 0; i < destInstructionList.size(); ++i)
		{
			if (!destInstructionList[i])
			{
				opCode = i;
				break;
			}
		}
	}
	if (destInstructionList[opCode])
	{
		destInstructionList.resize(destInstructionList.size() + 1);
		opCode = destInstructionList.size() - 1;
	}

	destInstructionList[opCode] = instruction;
}

////////////////////////////////////////////////////////////////////////
//    CInstructionBase
////////////////////////////////////////////////////////////////////////

/*virtual*/ void CInstructionBase::Initialize(size_t& accumulator, CInterpreter::Memory& memoryModel, size_t& instructionCounter, size_t& opCode, size_t& argument, size_t*& stackPointer)
{
	m_Accumulator = &accumulator;
	m_Memory = &memoryModel;
	m_InstructionCounter = &instructionCounter;
	m_OpCode = &opCode;
	m_Argument = &argument;
	m_StackPointer = &stackPointer;
}

/*virtual*/ void CInstructionBase::Run()
{ }

/*virtual*/ size_t CInstructionBase::GetOpCode() const
{
	return 0;
}

size_t& CInstructionBase::GetArgument()
{
	return (*m_Memory)[*m_Argument].second;
}

size_t& CInstructionBase::GetCurrentOpCode()
{
	return (*m_Memory)[*m_Argument].first;
}

size_t& CInstructionBase::GetAccumulator()
{
	return *m_Accumulator;
}

size_t& CInstructionBase::GetInstructionCounter()
{
	return *m_InstructionCounter;
}

size_t*& CInstructionBase::GetStackPointer()
{
	return *m_StackPointer;
}

size_t& CInstructionBase::GetStackTop()
{
	return **m_StackPointer;
}
