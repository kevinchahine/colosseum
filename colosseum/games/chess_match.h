#pragma once

#include "views/view_base.h"
#include "views/no_view.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <type_traits>

#include <boost/filesystem/path.hpp>

#include <uci/engine.h>
#include <uci/commands/go.h>

#include <forge/core/Move.h>
#include <forge/core/game_history.h>
#include <forge/time/clock.h>
#include <forge/core/GameState.h>

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
		const uci::engine& whiteEngine() const { return _whiteEngine; }
		uci::engine& whiteEngine() { return _whiteEngine; }

		void launchBlackEngine(const boost::filesystem::path& filename);
		const uci::engine& blackEngine() const { return _blackEngine; }
		uci::engine& blackEngine() { return _blackEngine; }

		template<class VIEW_T>
		void makeView();

		std::unique_ptr<views::ViewBase>& viewPtr() { return _viewPtr; }
		const std::unique_ptr<views::ViewBase>& viewPtr() const { return _viewPtr; }

		// Plays a game between white and black engines. 
		// Search parameters of each engine can be controlled using the uci::commands::go parameters
		// TODO: Return GameState
		forge::GameState play(const uci::commands::go& whitesParams, const uci::commands::go& blacksParams);

		// Plays a game between white and black engines. 
		// Search parameters of both engines can be controlled using the uci::commands::go parameters
		forge::GameState play(const uci::commands::go& params = uci::commands::go());

	private:
		uci::engine _whiteEngine;
		uci::engine _blackEngine;

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
