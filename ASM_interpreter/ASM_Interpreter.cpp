
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
	LoadInitialProgram();
	InitializeMemory();
	InitializeInstructions();
}

bool CInterpreter::LoadNextProgram()
{
	return IncrementProgramMemory();
}

void CInterpreter::SaveProgram(const string& filename)
{
	ofstream file(filename);

	for (size_t i = 0; i < m_ProgramMemory.size(); ++i)
	{
		MemoryUnit& unit = m_ProgramMemory[i];
		file << i << ": (" << unit << ") " << m_InstructionList[unit & INSTRUCTION_MASK]->GetOpPneumonic() << endl;
	}

	file.close();
}

void CInterpreter::RunProgram()
{
	LoadProgramMemory();

	size_t clockCycles = 0;
	m_InstructionCounter = 0;
	m_Accumulator = 0;

	for (;;)
	{
		++clockCycles;
		if (clockCycles > MAX_CLOCK_CYCLES)
			break;

		if (m_InstructionCounter >= m_Memory.size() - 1)
			break;

		MemoryUnit& currentMemory = m_Memory[m_InstructionCounter];
		m_CurrentOpCode = currentMemory & INSTRUCTION_MASK;

		m_InstructionList[m_CurrentOpCode]->Run();
		m_InstructionCounter += m_InstructionList[m_CurrentOpCode]->GetInstructionSize();
	}
}

void CInterpreter::ClearStack()
{
	m_StackPointer = &m_Stack.front();
}

void CInterpreter::PushToStack(size_t arg)
{
	*m_StackPointer = arg;

	if(m_StackPointer != &m_Stack.back())
		++m_StackPointer;
}

size_t CInterpreter::PopFromStack()
{
	if(m_StackPointer != &m_Stack.front())
		--m_StackPointer;

	return *m_StackPointer;
}

void CInterpreter::LoadInitialProgram()
{
	m_ProgramMemory.resize(PROGRAM_SIZE);
	m_Memory = m_ProgramMemory;
}

bool CInterpreter::IncrementProgramMemory()
{
	bool result = true;

	m_ProgramMemory.front() += 1;

	for (size_t i = 0; m_ProgramMemory[i] >= ARGUMENTS_COUNT; ++i)
	{
		m_ProgramMemory[i] -= ARGUMENTS_COUNT;

		if (i < m_ProgramMemory.size() - 1)
			++m_ProgramMemory[i + 1];
		else
			result = false;
	}

	return result;
}

void CInterpreter::LoadProgramMemory()
{
	m_Memory = m_ProgramMemory;
}

void CInterpreter::InitializeMemory()
{
	m_Stack.resize(STACK_SIZE);
	m_StackPointer = &m_Stack.front();
}

void CInterpreter::InitializeInstructions()
{
	CInstructionBase::Initialize(m_Accumulator, m_Memory, m_InstructionCounter, m_CurrentOpCode, &m_Stack.front(), &m_Stack.back(),  m_StackPointer);
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

/*static*/ size_t*               CInstructionBase::s_Accumulator;
/*static*/ CInterpreter::Memory* CInstructionBase::s_Memory;
/*static*/ size_t*               CInstructionBase::s_InstructionCounter;
/*static*/ size_t*               CInstructionBase::s_OpCode;
/*static*/ size_t*               CInstructionBase::s_StackBottom;
/*static*/ size_t*               CInstructionBase::s_StackTop;
/*static*/ size_t**              CInstructionBase::s_StackPointer;

/*static*/ void CInstructionBase::Initialize(size_t& accumulator, CInterpreter::Memory& memoryModel, size_t& instructionCounter, 
	                                         size_t& opCode, size_t* stackBottom, size_t* stackTop, size_t*& stackPointer)
{
	s_Accumulator = &accumulator;
	s_Memory = &memoryModel;
	s_InstructionCounter = &instructionCounter;
	s_OpCode = &opCode;
	s_StackBottom = stackBottom;
	s_StackTop = stackTop;
	s_StackPointer = &stackPointer;
}

/*virtual*/ void CInstructionBase::Run()
{ }

/*virtual*/ size_t CInstructionBase::GetOpCode() const
{
	return 0;
}

/*virtual*/ const char* CInstructionBase::GetOpPneumonic() const
{
	return "NOP";
}

/*virtual*/ size_t CInstructionBase::GetInstructionSize() const
{
	return 1;
}

size_t CInstructionBase::GetArgument()
{
	return GetArgument(GetInstructionCounter());
}

size_t CInstructionBase::GetNextArgument()
{
	return GetArgument(GetInstructionCounter() + 1);
}

size_t CInstructionBase::GetArgument(size_t addr)
{
	CInterpreter::Memory& memory = *s_Memory;
	addr = math::clampCeil(addr, memory.size());

	return math::clampCeil(memory[addr], CInterpreter::ARGUMENTS_COUNT);
}

void CInstructionBase::SetArgument(size_t arg)
{
	arg = math::clampCeil(arg, CInterpreter::ARGUMENTS_COUNT);
	SetArgument(GetInstructionCounter(), arg);
}

void CInstructionBase::SetArgument(size_t addr, size_t arg)
{
	CInterpreter::Memory& memory = *s_Memory;
	addr = math::clampCeil(addr, memory.size());
	
	memory[addr] = arg;
}

size_t& CInstructionBase::GetAccumulator()
{
	return *s_Accumulator;
}

size_t& CInstructionBase::GetInstructionCounter()
{
	return *s_InstructionCounter;
}

size_t*& CInstructionBase::GetStackPointer()
{
	return *s_StackPointer;
}

const size_t* CInstructionBase::GetStackTop()
{
	return s_StackTop;
}

const size_t* CInstructionBase::GetStackBottom()
{
	return s_StackBottom;
}
