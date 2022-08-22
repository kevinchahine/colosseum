#pragma once

#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>

namespace globals
{
	extern const boost::filesystem::path stockfishPath;
	extern const boost::filesystem::path lc0Path;
	extern const boost::filesystem::path berserkPath;
	extern const boost::filesystem::path etherealPath;
	extern const boost::filesystem::path koivistoPath;
	extern const boost::filesystem::path komodoPath;
	extern const boost::filesystem::path slowChessPath;
	
	extern const std::vector<boost::filesystem::path> enginePaths;
} // namespace globals