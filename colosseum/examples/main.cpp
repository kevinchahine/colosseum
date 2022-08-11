#include <iostream>
#include <string>
#include <sstream>

#include "examples/examples.h"
#include "games/chess_match.h"
#include "uci/go.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "source/elo_rating.h"

using namespace std;

int main(int argc, const char** argv)
{
	cout << "=== Colosseum ===" << endl;

	for (int i = 0; i < argc; i++) {
		cout << '\t' << argv[i] << endl;
	}

	engine_vs_engine();
	//chess_match();
	//elo_rating();
	//round_robin();
	
	return 0;
}