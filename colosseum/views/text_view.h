#pragma once

#include <iostream>

#include "view_base.h"

namespace views
{
	class TextView : public ViewBase
	{
	public:
		virtual void show(const forge::Position& position) override;
		
		virtual std::unique_ptr<ViewBase> clone() const override { return std::make_unique<TextView>(*this); };

	private:

	};
} // namespace views