#include "examples.h"

#include <iostream>

#include <boost/filesystem/path.hpp>

using namespace std;

void engine_vs_engine()
{
	// 1.) --- Launch Each Engine ---
	boost::filesystem::path engine1_path = "/usr/games/stockfish";
	boost::filesystem::path engine2_path = "/usr/games/ethereal-chess";
	
	cout << engine1_path << endl
	<< engine2_path << endl;
	
	//uci::client engine1()

}