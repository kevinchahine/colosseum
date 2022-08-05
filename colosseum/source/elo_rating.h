#pragma once

#include <iostream>
#include <vector>
#include <math.h>

#include "forge/core/GameState.h"

#include <boost/property_tree/ptree_fwd.hpp>

class EloRating
{
public:
	boost::property_tree::ptree serialize() const;

	void parse(const boost::property_tree::ptree& tree);

	// Let scoreA and scoreB denote the result of a game played
	// 1.0 means win
	// 0.5 means draw
	// 0.0 means loss
	static void update(EloRating& playerA, double scoreA, EloRating& playerB, double scoreB, double kFactor = 20);

	static void update(EloRating& whitePlayer, EloRating& blackPlayer, const forge::GameState & gstate, double kFactor = 20);

	friend std::ostream& operator<<(std::ostream& os, const EloRating& elo)
	{
		os << elo.rating;

		return os;
	}

	double rating = 1400.0;
};
