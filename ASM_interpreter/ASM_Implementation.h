#pragma once

#include "ASM_Interpreter.h"

#define DEFINE_INSTRUCTION(pneumonic) \
class pneumonic##_Instruction : public CInstructionBase \
{ \
private: \
	static TAutoRegister<pneumonic##_Instruction> s_Register; \
 \
public: \
	virtual void Run(); \
	virtual size_t GetOpCode() const; \
}

namespace asmi
{
	template<class InstructionType>
	class TAutoRegister
	{
	public:
		TAutoRegister()
		{
			CInstructionFactory::RegisterInstruction<InstructionType>();
		}
	};

	DEFINE_INSTRUCTION(MOVF);

	DEFINE_INSTRUCTION(MOVT);

	DEFINE_INSTRUCTION(ADD);

	DEFINE_INSTRUCTION(SUB);

	DEFINE_INSTRUCTION(MUL);

	DEFINE_INSTRUCTION(JZ);

	DEFINE_INSTRUCTION(POP);

	DEFINE_INSTRUCTION(PUSH);

}