#include "round_robin.h"

#include "chess_match.h"
#include "views/views.hpp"

using namespace std;

//	Inputs:
//		- Player:
//			- engine (as a UciClient or filepath to the engine application)
//			- rating 
//			- record file (as a filepath)
//		- nGamesPlayed (may times will each engine play each other engine?)
//			ex: with 4 engines and nGames = 2, each engine will play every other engine 2 times
//				resulting in (4 - 1) * 4 * 2 = 24 games.
//		- go: uci go parameter which specifies game parameters like:
//			clock
//			search times
//			node limits
//			etc.	
//			* See uci_description.txt
// 
//		+ totalGames:
//			(nEngines - 1) * (nEngines) * (nGamesPlayed)
// 
//		+ play()
//	

void RoundRobin::play()
{
	for (size_t gameIndex = 0; gameIndex < _nGames; gameIndex++) {
		cout << "=== Round " << gameIndex << " ===" << endl;

		for (size_t whiteIndex = 0; whiteIndex < _players.size(); whiteIndex++) {
			Player& whitePlayer = _players.at(whiteIndex);

			for (size_t blackIndex = 0; blackIndex < _players.size(); blackIndex++) {
				Player& blackPlayer = _players.at(blackIndex);

				// --- Don't let an engine play against itself ---
				if (whiteIndex != blackIndex) {
					games::ChessMatch match;

					// --- Set up engines ---
					match.launchWhiteEngine(whitePlayer.enginePath());
					match.launchBlackEngine(blackPlayer.enginePath());
					match.viewPtr() = _viewPtr->clone();

					cout << "\t=== Match: "
						<< match.whiteEngine().engine_name() << " vs "
						<< match.blackEngine().engine_name() << " ===" << endl;

					// --- Play game and resturn result ---
					forge::GameState gstate = match.play(_go);
					
					cout << "=== Match Ended ===" << endl
						<< "\t" << gstate << endl;

					// --- Update Elo ---
					EloRating::update(whitePlayer.rating(), blackPlayer.rating(), gstate);

					cout << match.whiteEngine().engine_name() << ": " << whitePlayer.rating() 
						<< match.blackEngine().engine_name() << ": " << blackPlayer.rating() << endl;
					// *** When match goes out of scope engine processes will close ***
				}
			}
		}
	}
}
