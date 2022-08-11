#include "round_robin.h"

#include "chess_match.h"
#include "views/views.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;

//	Inputs:
//		- Player:
//			- engine (as a engine or filepath to the engine application)
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
	for (; roundIndex < _nRounds; roundIndex++) {
		cout << termcolor::green << "===== Round " << roundIndex << " =====" << termcolor::white << endl;

		for (; whiteIndex < _players.size(); whiteIndex++) {
			Player& whitePlayer = _players.at(whiteIndex);

			for (; blackIndex < _players.size(); blackIndex++) {
				Player& blackPlayer = _players.at(blackIndex);

				// --- Don't let an engine play against itself ---
				if (whiteIndex != blackIndex) {
					games::ChessMatch match;

					// --- Set up engines ---
					match.launchWhiteEngine(whitePlayer.enginePath());
					match.launchBlackEngine(blackPlayer.enginePath());
					match.viewPtr() = _viewPtr->clone();

					cout << termcolor::yellow << "=== Match: "
						<< match.whiteEngine().engine_name() << " vs "
						<< match.blackEngine().engine_name() << " ===" << termcolor::white << endl;

					// --- Play game and resturn result ---
					forge::GameState gstate = match.play(_go);
					
					cout << "=== Match Ended ===" << endl
						<< gstate << endl
						<< endl;

					if (gstate.isDraw() || gstate.whiteWins() || gstate.blackWins()) {
						// --- Update Elo ---
						whitePlayer.ratings().push_back(whitePlayer.currRating());
						blackPlayer.ratings().push_back(blackPlayer.currRating());

						EloRating::update(whitePlayer.currRating(), blackPlayer.currRating(), gstate);
						cout << "=== Update Elo Ratings ===" << endl
							<< match.whiteEngine().engine_name() << ": " << whitePlayer.currRating() << " --- "
							<< match.blackEngine().engine_name() << ": " << blackPlayer.currRating() << endl;
					}

					// --- Save progress ---
					this->save();

					// *** When match goes out of scope engine processes will close ***
				}
			}
			blackIndex = 0;
		}
		whiteIndex = 0;
	}
}

boost::property_tree::ptree serializeHelper(const vector<Player>& players)
{
	boost::property_tree::ptree arr;

	for (size_t i = 0; i < players.size(); i++) {
		boost::property_tree::ptree elem;

		elem.put_child("Player", players.at(i).serialize());

		arr.push_back(
			boost::property_tree::ptree::value_type("", elem)
		);
	}

	return arr;
}

void parseHelper(vector<Player>& players, const boost::property_tree::ptree& playersTree)
{
	for (boost::property_tree::ptree::const_iterator it = playersTree.begin();
		it != playersTree.end();
		it++) {
		Player & curr = players.emplace_back();

		// TODO: Implement Player.parse(). replace with that method call.
		const string & key = it->first;
		const boost::property_tree::ptree& elem = it->second;

		// ----- Player -----
		boost::optional<const boost::property_tree::ptree &> playerTreeOp = elem.get_child_optional("Player");

		if (playerTreeOp.has_value()) {
			const boost::property_tree::ptree& playerTree = playerTreeOp.get();

			curr.parse(playerTree);
		}
	}
}

boost::property_tree::ptree RoundRobin::serialize() const
{
	// TODO: replace all trees with move operators
	boost::property_tree::ptree tree;

	// --- Go ---
	tree.add_child("RoundRobin", _go.serialize());
	
	// --- N Rounds ---
	tree.add("RoundRobin.nRounds", _nRounds);

	// --- Tournament Progress ---
	tree.add("RoundRobin.roundIndex", roundIndex);
	tree.add("RoundRobin.whiteIndex", whiteIndex);
	tree.add("RoundRobin.blackIndex", blackIndex);

	// --- Players ---
	tree.add_child("RoundRobin.Players", serializeHelper(_players));

	return tree;
}

void RoundRobin::parse(const boost::property_tree::ptree& tree)
{
	// --- Go ---
	auto goTree = tree.get_child_optional("RoundRobin.go");
	if (goTree.has_value()) {
		_go.parse(goTree.get());
	}

	// --- N Rounds ---
	auto nRoundsOp = tree.get_optional<int>("RoundRobin.nRounds");
	_nRounds = nRoundsOp.get_value_or(1);

	// --- Tournament Progress ---
	auto roundIndexOp = tree.get_optional<size_t>("RoundRobin.roundIndex");
	roundIndex = roundIndexOp.get_value_or(0);

	auto whiteIndexOp = tree.get_optional<size_t>("RoundRobin.whiteIndex");
	whiteIndex = whiteIndexOp.get_value_or(0);

	auto blackIndexOp = tree.get_optional<size_t>("RoundRobin.blackIndex");
	blackIndex = blackIndexOp.get_value_or(0);

	// --- Players ---
	auto playersTree = tree.get_child_optional("RoundRobin.Players");
	if (playersTree.has_value()) {
		parseHelper(_players, playersTree.get());
	}
}

void RoundRobin::save()
{
	boost::property_tree::ptree tree = this->serialize();

	ofstream fout(saveFile.string());
	if (fout.is_open()) {
		cout << "Tournament progress will be saved in " << saveFile << endl;
	}

	boost::property_tree::write_json(fout, tree);
	//boost::property_tree::write_json(cout, tree);	// TODO: comment this out but keep for debugging
}

void RoundRobin::load()
{
	boost::property_tree::ptree tree;

	ifstream fin;
	fin.open(saveFile.string());

	if (fin.is_open()) {
		boost::property_tree::read_json(fin, tree);

		//boost::property_tree::write_json(cout, tree);		// remove

		// remove vvv
		cout << "------------------------------------------------" << endl;

		this->parse(tree);

		cout << "go: " << _go << endl
			<< "nRounds: " << _nRounds << endl
			<< "roundIndex: " << roundIndex << endl
			<< "whiteIndex: " << whiteIndex << endl
			<< "blackIndex: " << blackIndex << endl
			<< "saveFile: " << saveFile << endl
			<< "players: " << endl;

		for (const auto& p : _players) {
			cout << '\t' << p << endl;
		}
		// remove ^^^
	}
}
