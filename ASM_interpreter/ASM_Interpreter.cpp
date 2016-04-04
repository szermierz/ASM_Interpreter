
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
		file << i << ":" << m_InstructionList[unit.first]->GetOpPneumonic() << " " << unit.second << endl;
	}

	file.close();
}

void CInterpreter::RunProgram()
{
	LoadProgramMemory();

	size_t clockCycles = 0;
	m_InstructionCounter = 0;
	--m_InstructionCounter;
	m_Accumulator = 0;

	for (;;)
	{
		++clockCycles;
		if (clockCycles > MAX_CLOCK_CYCLES)
			break;

		if (++m_InstructionCounter == m_Memory.size())
			break;

		MemoryUnit& currentMemory = m_Memory[m_InstructionCounter];
		m_CurrentOpCode = currentMemory.first;
		m_CurrentArg = currentMemory.second;

		m_InstructionList[m_CurrentOpCode]->Run();	
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

	m_ProgramMemory.front().second += 1;

	for (size_t i = 0; m_ProgramMemory[i].second >= ARGUMENTS_COUNT; ++i)
	{
		m_ProgramMemory[i].second -= ARGUMENTS_COUNT;
		++m_ProgramMemory[i].first;

		if (m_ProgramMemory[i].first < INSTRUCTIONS_COUNT)
			break;

		m_ProgramMemory[i].first -= INSTRUCTIONS_COUNT;

		if (i < m_ProgramMemory.size() - 1)
			++m_ProgramMemory[i + 1].second;
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
	for (auto it : m_InstructionList)
		it->Initialize(m_Accumulator, m_Memory, m_InstructionCounter, m_CurrentOpCode, m_CurrentArg, &m_Stack.front(), &m_Stack.back(),  m_StackPointer);
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

/*virtual*/ void CInstructionBase::Initialize(size_t& accumulator, CInterpreter::Memory& memoryModel, size_t& instructionCounter, 
	                                          size_t& opCode, size_t& argument, size_t* stackBottom, size_t* stackTop, size_t*& stackPointer)
{
	m_Accumulator = &accumulator;
	m_Memory = &memoryModel;
	m_InstructionCounter = &instructionCounter;
	m_OpCode = &opCode;
	m_Argument = &argument;
	m_StackBottom = stackBottom;
	m_StackTop = stackTop;
	m_StackPointer = &stackPointer;
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

size_t CInstructionBase::GetArgument()
{
	return math::clampCeil(*m_Argument, CInterpreter::ARGUMENTS_COUNT);
}

size_t CInstructionBase::GetArgument(size_t addr)
{
	CInterpreter::Memory& memory = *m_Memory;
	addr = math::clampCeil(addr, memory.size());

	return math::clampCeil(memory[addr].second, CInterpreter::ARGUMENTS_COUNT);
}

void CInstructionBase::SetArgument(size_t arg)
{
	*m_Argument = arg;
}

void CInstructionBase::SetArgument(size_t addr, size_t arg)
{
	CInterpreter::Memory& memory = *m_Memory;
	addr = math::clampCeil(addr, memory.size());
	
	memory[addr].second = arg;
}

size_t& CInstructionBase::GetCurrentOpCode()
{
	size_t arg = *m_Argument;
	CInterpreter::Memory& memory = *m_Memory;

	if (arg >= memory.size())
		return memory.back().first;

	return memory[arg].first;
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

const size_t* CInstructionBase::GetStackTop()
{
	return m_StackTop;
}

const size_t* CInstructionBase::GetStackBottom()
{
	return m_StackBottom;
}
