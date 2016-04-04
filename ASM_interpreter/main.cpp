
#include "ASM_Interpreter.h"

using namespace asmi;

int main(int argc, char** argv)
{
	CInterpreterPtr i = CInterpreter::GetSingleton();

	i->Initialize();

	size_t progressReportCounter = 0;
	size_t reportIndex = 0;

	for (;;)
	{
		i->ClearStack();

		i->PushToStack(1);
		i->PushToStack(3);

		i->RunProgram();

		if (i->PopFromStack() == 4)
		{
			i->ClearStack();

			i->PushToStack(6);
			i->PushToStack(2);

			i->RunProgram();

			if (i->PopFromStack() == 8)
			{
				int sotp = 0;
				cout << "FOUND" << endl;
				i->SaveProgram("Dupa.txt");
				break;
			}
		}

		if(!i->LoadNextProgram())
			break;

		if (++progressReportCounter > 10000000)
		{
			++reportIndex;
			progressReportCounter = 0;
			cout << reportIndex << ": Next 10M programs tested!" << endl;
		}
	}

	return 0;
}