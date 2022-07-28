#include "view_base.h"

using namespace std;

namespace views
{
	void ViewBase::highlight(const forge::BoardSquare& square)
	{
		this->_highlights[square] = 1;
	}

	void ViewBase::highlight(const std::bitset<64>& squares)
	{
		this->_highlights |= squares;
	}

	void ViewBase::highlight(const forge::BitBoard& squares)
	{
		this->_highlights |= squares;
	}
} // anmespace views