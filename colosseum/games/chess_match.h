#pragma once

#include "views/view_base.h"
#include "views/no_view.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <type_traits>

#include <boost/filesystem/path.hpp>

#include <uci/uci_client.h>
#include <uci/go.h>

#include <forge/core/Move.h>
#include <forge/core/game_history.h>
#include <forge/time/clock.h>

namespace games
{
	class ChessMatch
	{
	public:
		ChessMatch() = default;
		ChessMatch(const ChessMatch&) = default;
		ChessMatch(ChessMatch&&) noexcept = default;
		~ChessMatch() noexcept = default;
		ChessMatch& operator=(const ChessMatch&) = default;
		ChessMatch& operator=(ChessMatch&&) noexcept = default;

		void launchWhiteEngine(const boost::filesystem::path& filename);
		const uci::UciClient& whiteEngine() const { return _whiteEngine; }
		uci::UciClient& whiteEngine() { return _whiteEngine; }

		void launchBlackEngine(const boost::filesystem::path& filename);
		const uci::UciClient& blackEngine() const { return _blackEngine; }
		uci::UciClient& blackEngine() { return _blackEngine; }

		template<class VIEW_T>
		void makeView();

		// Plays a game between white and black engines. 
		// Search parameters of each engine can be controlled using the uci::go parameters
		void play(const uci::go& whitesParams, const uci::go& blacksParams);

		// Plays a game between white and black engines. 
		// Search parameters of both engines can be controlled using the uci::go parameters
		void play(const uci::go& params = uci::go());

	private:
		uci::UciClient _whiteEngine;
		uci::UciClient _blackEngine;

		forge::game_history _history;

		forge::clock _clock;

		std::unique_ptr<views::ViewBase> _viewPtr = std::make_unique<views::NoView>();
	};

	template<class VIEW_T>
	void ChessMatch::makeView()
	{
		static_assert(std::is_base_of<views::ViewBase, VIEW_T>(), "Error: VIEW_T must derive from ViewBase");

		_viewPtr = std::make_unique<VIEW_T>();
	}
} // namespace games
