
#include "ASM_Interpreter.h"

using namespace asmi;

int main(int argc, char** argv)
{
	CInterpreterPtr i = CInterpreter::GetSingleton();

	i->Initialize();

	i->RunProgram();

	return 0;
}