#pragma once

#include "Utilities.h"
#include "Bitvariable.h"

namespace asmi
{
	CLASS(CInterpreter);
	STRUCT(SMemory);
	CLASS(CInstructionFactory);
	CLASS(CInstructionBase)

	class CInterpreter
	{
	public:

		void Initialize();

	private:

		/* Memory model */
		void InitializeMemory();
		SMemoryPtr m_Memory;

		/* Instructions map */
		void InitializeInstructionsMap();
		unordered_map<TBitVariable<3>, CInstructionBasePtr> m_InstructionsMap;
		

	};

	struct SMemory
	{

		vector<TBitVariable<3>> Instructions;
		vector<TBitVariable<5>> Arguments;

	};

	class CInstructionFactory
	{
	private:

		static CInstructionFactoryPtr s_Singleton;

	public:

		static CInstructionFactoryPtr GetSingleton();

	};

	class CInstructionBase
	{
	public:

		void Initialize(SMemoryPtr memoryModel);

	protected:

		/* Memory access */
	};

}
