#include "player.h"

#include <boost/property_tree/ptree.hpp>

using namespace std;

Player::Player()
{
}

Player::Player(const boost::filesystem::path& pathToEngine, const EloRating& currRating) :
	_enginePath(pathToEngine)
{
	_ratings.emplace_back(currRating);
}

boost::property_tree::ptree Player::serialize() const
{
	boost::property_tree::ptree tree;

	// --- Engine Path ---
	tree.put("enginePath", _enginePath.string());
	
	// --- Ratings ---
	tree.add_child("ratings", _ratings.serialize());

	return tree;
}

void Player::parse(const boost::property_tree::ptree& tree)
{
	// --- Engine Path ---
	boost::optional<string> op = tree.get_optional<string>("enginePath");

	if (op.has_value()) {
		_enginePath = op.get();
	}

	// --- Ratings ---
	boost::optional<const boost::property_tree::ptree &> ratingsTreeOp = tree.get_child_optional("ratings");
	if (ratingsTreeOp.has_value()) {
		_ratings.parse(ratingsTreeOp.get());
	}
}


