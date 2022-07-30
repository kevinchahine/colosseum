#pragma once

#include "elo_rating.h"

#include <boost/filesystem/path.hpp>

class Player
{
public:
    Player() = default;
    Player(const boost::filesystem::path& pathToEngine, const EloRating& currRating = EloRating{});
    Player(const Player&) = default;
    Player(Player&&) noexcept = default;
    ~Player() noexcept = default;
    Player& operator=(const Player&) = default;
    Player& operator=(Player&&) noexcept = default;

    const boost::filesystem::path& enginePath() const { return _enginePath; }
    boost::filesystem::path& enginePath() { return _enginePath; }

    const EloRating& rating() const { return _rating; }
    EloRating& rating() { return _rating; }

private:
	boost::filesystem::path _enginePath;
	EloRating _rating;
};