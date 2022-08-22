#pragma once

#include <iostream>
#include <vector>

#include <uci/commands/go.h>

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
	uci::commands::go& go() { return _go; }
	const uci::commands::go& go() const { return _go; }

	std::vector<Player>& players() { return _players; }
	const std::vector<Player>& players() const { return _players; }

	void setNRounds(int nGames) { _nRounds = nGames; }
	int getNRounds() const { return _nRounds; }

	template<class VIEW_T>
	void makeView()
	{
		static_assert(std::is_base_of<views::ViewBase, VIEW_T>(), "Error: VIEW_T must derive from views::ViewBase");

		_viewPtr = std::make_unique<VIEW_T>();
	}

	void play();

	boost::property_tree::ptree serialize() const;

	void parse(const boost::property_tree::ptree& tree);

	void save();

	void load();

private:
	uci::commands::go _go;
	int _nRounds;			// How many games will each engine play.
	std::vector<Player> _players;
	std::unique_ptr<views::ViewBase> _viewPtr;

	// --- Game Progress ---
	size_t roundIndex = 0;	// which round are we playing?
	size_t whiteIndex = 0;	// which engine is playing as white?
	size_t blackIndex = 0;	// which engine is playing as black?

public:
	boost::filesystem::path	saveFile = DATA_DIR"roundRobin.json";
};
