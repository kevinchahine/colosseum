#include "elo_rating.h"

#include <boost/property_tree/ptree.hpp>

using namespace std;

boost::property_tree::ptree EloRating::serialize() const
{
	boost::property_tree::ptree tree;
	
	tree.put("", rating);

	return tree;
}

void EloRating::parse(const boost::property_tree::ptree& tree)
{
	boost::optional<double> op = tree.get_optional<double>("rating");

	if (op.has_value()) {
		rating = op.value();
	}
	else {
		cout << "Error: property_tree does not have an entry for 'rating'" << endl;
		rating = 1400;
	}
}

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
		cout << "Error: game state is not win loss draw. Elo ratings will not be updated." << endl;

		return;
	}

	EloRating::update(
		whitePlayer, whiteScore, 
		blackPlayer, blackScore,
		kFactor);
}
