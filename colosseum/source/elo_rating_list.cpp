#include "elo_rating_list.h"

#include "boost/property_tree/ptree.hpp"

using namespace std;

boost::property_tree::ptree EloRatingList::serialize() const
{
	boost::property_tree::ptree tree;

	for (const EloRating& r : *this) {
		boost::property_tree::ptree node;

		node.add_child("rating", r.serialize());

		tree.push_back(make_pair("", node));
	}

	return tree;
}

void EloRatingList::parse(const boost::property_tree::ptree& tree)
{
	this->clear();

	for (boost::property_tree::ptree::const_iterator it = tree.begin();
		it != tree.end();
		it++) {
		const string& key = it->first;
		boost::property_tree::ptree value = it->second;

		EloRating& curr = this->emplace_back();

		curr.parse(value);
	}
}
