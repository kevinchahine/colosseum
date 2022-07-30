#include "player.h"

using namespace std;

Player::Player(const boost::filesystem::path& pathToEngine, const EloRating& currRating) :
	_enginePath(pathToEngine),
	_rating(currRating)
{
}


