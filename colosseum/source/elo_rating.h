#pragma once

#include <iostream>
#include <math.h>

#include "forge/core/GameState.h"

class EloRating
{
public:
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

private:
	double rating = 1400.0;
};

