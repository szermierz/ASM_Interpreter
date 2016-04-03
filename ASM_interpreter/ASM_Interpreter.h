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
		const size_t STACK_SIZE = 500;
		const size_t PROGRAM_SIZE = 128; //todo

	public:

		/* Typedefs */
		typedef pair<size_t, size_t> MemoryUnit;
		typedef vector<MemoryUnit> Memory;

	public:

		void Initialize();

		void RunProgram();

	private:

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

		virtual void Initialize(size_t& accumulator, CInterpreter::Memory& memoryModel, size_t& instructionCounter, size_t& opCode, size_t& argument, size_t*& stackPointer);
		
		virtual void Run();

		virtual size_t GetOpCode() const;

	protected:

		/* Interpreter Access */
		size_t&                    GetArgument();
		size_t&                    GetCurrentOpCode();
		size_t&                    GetAccumulator();
		size_t&                    GetInstructionCounter();
		size_t*&                   GetStackPointer();
		size_t&                    GetStackTop();
		size_t*                    m_Accumulator;
		CInterpreter::Memory*      m_Memory;
		size_t*                    m_InstructionCounter;
		size_t*                    m_OpCode;
		size_t*                    m_Argument;
		size_t**                   m_StackPointer;

	};

}
