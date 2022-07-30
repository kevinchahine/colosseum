#pragma once

#include <iostream>
#include <vector>

#include <uci/go.h>

#include "source/player.h"
#include "views/view_base.h"

// Plays a tournament in the round robin format. 
// Each player plays each other player an equal number of times. 
// After each game, the elo rating of each participant is updated and saved.
// 
// https://en.wikipedia.org/wiki/Chess_tournament#Formats
class RoundRobin /* : public ???ChessTournament??? */
{
public:
	uci::go& go() { return _go; }
	const uci::go& go() const { return _go; }

	std::vector<Player>& players() { return _players; }
	const std::vector<Player>& players() const { return _players; }

	void setNGames(int nGames) { _nGames = nGames; }
	int getNGames() const { return _nGames; }

	template<class VIEW_T>
	void makeView()
	{
		static_assert(std::is_base_of<views::ViewBase, VIEW_T>(), "Error: VIEW_T must derive from views::ViewBase");

		_viewPtr = std::make_unique<VIEW_T>();
	}

	void play();

private:
	uci::go _go;
	std::vector<Player> _players;
	int _nGames;
	std::unique_ptr<views::ViewBase> _viewPtr;
};
