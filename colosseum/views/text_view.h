#pragma once

#include <iostream>

#include "view_base.h"

namespace views
{
	class TextView : public ViewBase
	{
	public:
		virtual void show(const forge::Position& position) override;

	private:

	};
} // namespace views