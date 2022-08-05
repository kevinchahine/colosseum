#pragma once

#include "elo_rating.h"

class EloRatingList : public std::vector<EloRating>
{
public:
	boost::property_tree::ptree serialize() const;

	void parse(const boost::property_tree::ptree& tree);
};

