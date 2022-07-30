#include "globals.h"

using namespace std;

namespace globals
{
	std::vector<boost::filesystem::path> enginePaths = {
		R"dil(C:\Users\kchah\Saved Games\stockfish_15\stockfish_15.exe)dil",				// Stockfish 15
		R"dil(C:\Users\kchah\Saved Games\lc0\lc0.exe)dil",									// Leela Chess Zero
		R"dil(C:\Users\kchah\Saved Games\Berserk-9_Windows\berserk-9-x64.exe)dil",			// Berserk 9
		R"dil(C:\Users\kchah\Saved Games\ethereal\Ethereal-pext-avx2.exe)dil",				// Ethereal
		R"dil(C:\Users\kchah\Saved Games\koivisto\Koivisto_8.0-x64-windows-avx2.exe)dil",	// Koivisto
		R"dil(C:\Users\kchah\Saved Games\komodo-13\komodo-13_201fd6\Windows\komodo-13.02-64bit.exe)dil",	// Komodo
		R"dil(C:\Users\kchah\Saved Games\SlowChess-2.9\SlowChess-2.9\slow64-avx2.exe)dil",	// Slow Chess
	};

} // namespace globals