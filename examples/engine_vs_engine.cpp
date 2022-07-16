#include "examples.h"

#include <iostream>

#include <boost/filesystem/path.hpp>

#include <uci/uci_client.h>

using namespace std;

void engine_vs_engine()
{
	// 1.) --- Launch Each Engine ---
	boost::filesystem::path engine1_path = "/usr/games/stockfish";
	boost::filesystem::path engine2_path = "/usr/games/ethereal-chess";
	
	cout << engine1_path << endl
		<< engine2_path << endl;

	uci::UciClient engine1(engine1_path.string());
	uci::UciClient engine2(engine2_path.string());

	
}