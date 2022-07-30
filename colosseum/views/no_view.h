#pragma once

#include "view_base.h"

namespace views
{
	class NoView : public ViewBase
	{
	public:
		virtual void show(const forge::Position& position) override { /* no-op */ }

		virtual std::unique_ptr<ViewBase> clone() const override { return std::make_unique<NoView>(*this); };

	private:

	};
} // namespace views