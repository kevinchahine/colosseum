#include "globals.h"

using namespace std;

namespace globals
{
	const boost::filesystem::path stockfishPath = R"dil(C:\Users\kchah\Saved Games\stockfish_15\stockfish_15.exe)dil";			
	const boost::filesystem::path lc0Path = R"dil(C:\Users\kchah\Saved Games\lc0\lc0.exe)dil";									
	const boost::filesystem::path berserkPath = R"dil(C:\Users\kchah\Saved Games\Berserk-9_Windows\berserk-9-x64.exe)dil";			
	const boost::filesystem::path etherealPath = R"dil(C:\Users\kchah\Saved Games\ethereal\Ethereal-pext-avx2.exe)dil";				
	const boost::filesystem::path koivistoPath = R"dil(C:\Users\kchah\Saved Games\koivisto\Koivisto_8.0-x64-windows-avx2.exe)dil";	
	const boost::filesystem::path komodoPath = R"dil(C:\Users\kchah\Saved Games\komodo-13\komodo-13_201fd6\Windows\komodo-13.02-64bit.exe)dil";
	const boost::filesystem::path slowChessPath = R"dil(C:\Users\kchah\Saved Games\SlowChess-2.9\SlowChess-2.9\slow64-avx2.exe)dil";

	const std::vector<boost::filesystem::path> enginePaths = {
		stockfishPath,
		lc0Path,
		berserkPath,
		etherealPath,
		koivistoPath,
		komodoPath,
		slowChessPath,
	};
} // namespace globals