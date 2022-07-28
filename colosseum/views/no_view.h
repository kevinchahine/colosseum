#pragma once

#include "view_base.h"

namespace views
{
	class NoView : public ViewBase
	{
	public:
		virtual void show(const forge::Position& position) override { /* no-op */ }

	private:

	};
} // namespace views