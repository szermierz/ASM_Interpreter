#pragma once

#include "Utilities.h"
#include "Bitvariable.h"

namespace asmi
{
	CLASS(CInterpreter);
	CLASS(CInstructionFactory);
	CLASS(CInstructionBase);
}

namespace asmi
{
	class CInterpreter
	{
	private:

		static CInterpreterPtr s_Singleton;

	private:

		friend class CInstructionFactory;

	public:

		static CInterpreterPtr GetSingleton();

	public:

		/* Constants */
		static const size_t STACK_SIZE = 500;
		static const size_t PROGRAM_SIZE = 16; //todo
		static const size_t INSTRUCTIONS_COUNT = 8; //todo
		static const size_t ARGUMENTS_COUNT = 10; //todo
		static const size_t MAX_CLOCK_CYCLES = 100; //todo

	public:

		/* Typedefs */
		typedef pair<size_t, size_t> MemoryUnit;
		typedef vector<MemoryUnit> Memory;

	public:

		/* Initialization */
		void Initialize();

		/* Program loading */
		bool LoadNextProgram();
		
		/* Program saving */
		void SaveProgram(const string& filename);

		/* Interpretation */
		void RunProgram();
		void ClearStack();
		void PushToStack(size_t arg);
		size_t PopFromStack();

	private:

		/* Program loading */
		void LoadInitialProgram();
		bool IncrementProgramMemory();
		void LoadProgramMemory();
		Memory         m_ProgramMemory;

		/* Memory model */
		void InitializeMemory();
		Memory         m_Memory;
		size_t*        m_StackPointer;
		vector<size_t> m_Stack;

		/* Instructions list */
		void InitializeInstructions();
		vector<CInstructionBasePtr> m_InstructionList;

		/* Interpretation */
		size_t      m_InstructionCounter;
		size_t      m_Accumulator;
		size_t      m_CurrentOpCode;
		size_t      m_CurrentArg;
	};

	class CInstructionFactory
	{
	private:

		friend class CInstructionRegistration;

	public:

		template<class InstructionType>
		static void RegisterInstruction()
		{
			auto interpreter = CInterpreter::GetSingleton();
			auto newInstruction = make_shared<InstructionType>();
			if (!interpreter || !newInstruction)
				return;

			AddInstruction(interpreter->m_InstructionList, newInstruction);
		}

	private:

		static void AddInstruction(vector<CInstructionBasePtr>& destInstructionList, CInstructionBasePtr instruction);

	};

	class CInstructionBase
	{
	public:

		virtual void Initialize(size_t& accumulator, CInterpreter::Memory& memoryModel, size_t& instructionCounter, size_t& opCode, size_t& argument, size_t* stackBottom, size_t* stackTop, size_t*& stackPointer);
		
		virtual void Run();

		virtual size_t GetOpCode() const;
		virtual const char* GetOpPneumonic() const;

	protected:

		/* Interpreter Access */
		size_t                     GetArgument();
		size_t                     GetArgument(size_t addr);
		void                       SetArgument(size_t arg);
		void                       SetArgument(size_t addr, size_t arg);
		size_t&                    GetCurrentOpCode();
		size_t&                    GetAccumulator();
		size_t&                    GetInstructionCounter();
		size_t*&                   GetStackPointer();
		const size_t*              GetStackTop();
		const size_t*              GetStackBottom();
		size_t*                    m_Accumulator;
		CInterpreter::Memory*      m_Memory;
		size_t*                    m_InstructionCounter;
		size_t*                    m_OpCode;
		size_t*                    m_Argument;
		size_t*                    m_StackBottom;
		size_t*                    m_StackTop;
		size_t**                   m_StackPointer;

	};

}
