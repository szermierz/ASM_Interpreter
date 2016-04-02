#pragma once

namespace
{
	template<typename DataType>
	class TBitVariableImpl
	{
	public:

		operator DataType&()
		{
			return m_Data;
		}

	private:

		DataType m_Data;

	};
}

class TBitVariable
{
public:

	TBitVariable(int bitsCount);
};



