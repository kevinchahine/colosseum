#pragma once

#include <vector>
#include <bitset>

#include <forge/core/Position.h>
#include <forge/time/clock.h>

namespace views
{
	class ViewBase
	{
	public:
		// Clears highlighs 
		virtual void show(const forge::Position& position) = 0;

		virtual std::unique_ptr<ViewBase> clone() const = 0;

		void highlight(const forge::BoardSquare& square);
		void highlight(const std::bitset<64>& squares);
		void highlight(const forge::BitBoard& squares);
		//template<typename CONTAINER_T>
		//void highlight(const CONTAINER_T::const_iterator& begin, const CONTAINER_T::const_iterator& end);

	protected:
		forge::BitBoard _highlights;

		forge::clock _clock;
	};
} // namespace views