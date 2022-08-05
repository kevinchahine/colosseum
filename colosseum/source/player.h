#pragma once

#include "elo_rating_list.h"

#include <vector>

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

class Player
{
public:
    Player();
    Player(const boost::filesystem::path& pathToEngine, const EloRating& currRating = EloRating{});
    Player(const Player&) = default;
    Player(Player&&) noexcept = default;
    ~Player() noexcept = default;
    Player& operator=(const Player&) = default;
    Player& operator=(Player&&) noexcept = default;

    const boost::filesystem::path& enginePath() const { return _enginePath; }
    boost::filesystem::path& enginePath() { return _enginePath; }

    const EloRating& currRating() const { return _ratings.back(); }
    EloRating& currRating() { return _ratings.back(); }

    const EloRatingList& ratings() const { return _ratings; }
    EloRatingList& ratings() { return _ratings; }

    boost::property_tree::ptree serialize() const;
    void parse(const boost::property_tree::ptree& tree);

    friend std::ostream& operator<<(std::ostream& os, const Player& player)
    {
        os << player._enginePath << '\t' << "ratings:";

        for (const EloRating& r : player._ratings) {
            os << ' ' << r;
        }

        return os;
    }

private:
	boost::filesystem::path _enginePath;
    EloRatingList _ratings;
};