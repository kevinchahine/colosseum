#include "elo_rating.h"

using namespace std;

void EloRating::update(
	EloRating& playerA, 
	double scoreA,
	EloRating& playerB, 
	double scoreB,
	double kFactor)
{
	double qA = pow(10, playerA.rating / 400);
	double qB = pow(10, playerB.rating / 400);

	double eA = qA / (qA + qB);
	double eB = qB / (qA + qB);

	double newRatingA = playerA.rating + kFactor * (scoreA - eA);
	double newRatingB = playerB.rating + kFactor * (scoreB - eB);

	playerA.rating = newRatingA;
	playerB.rating = newRatingB;
}

void EloRating::update(EloRating& whitePlayer, EloRating& blackPlayer, const forge::GameState& gstate, double kFactor)
{
	double whiteScore = 0.0;
	double blackScore = 0.0;

	if (gstate.isDraw()) {
		whiteScore = 0.5;
		blackScore = 0.5;
	}
	else if (gstate.whiteWins()) {
		whiteScore = 1.0;
		blackScore = 0.0;
	}
	else if (gstate.blackWins()) {
		whiteScore = 0.0;
		blackScore = 1.0;
	}
	else {
		// somethings wrong
		cout << "Error: game state is not win loss draw" << endl;

		return;
	}

	EloRating::update(
		whitePlayer, whiteScore, 
		blackPlayer, blackScore,
		kFactor);
}

