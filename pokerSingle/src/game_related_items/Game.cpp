
#include "Game.h"

#include "../Flags.h"
#include "../tools/random.h"
#include "../players/Bot.h"
#include "../players/ConsolePlayer.h"
#include "../tools/Functions.h"
#include <fstream>


Game::Game(size_type playerAmount, bool onlyBots)
	: m_output(nullptr), m_table(), m_matches(0), m_curBaseRaiseAmount(s_roundBuyIn),
	m_curCallAmount(s_roundBuyIn), m_potHandler(), m_players(), m_folded(0),
	m_broke(0) {

	if (playerAmount < 2)
		playerAmount = 2;
	else if (playerAmount > 10)
		playerAmount = 10;

	m_output = &std::cout;

	m_players.reserve(10); // the maximum amount of players allowed in a poker game

	size_type place = random::randInt(0, playerAmount - 1);

	for (size_type i = 0; i < place; i++)
		addBot(s_startingChips);

#if !DEBUG
	if (onlyBots)
		addBot(s_startingChips);
	else
		addConsolePlayer(s_startingChips);
#elif !JUST_BOTS // DEBUG
	addConsolePlayer(s_startingChips);
#else
	addBot(s_startingChips);
#endif

	for (size_type i = 0; i < playerAmount - place - 1; i++)
		addBot(s_startingChips);

	m_potHandler.create(m_output, m_players);

	*m_output << std::endl << "*A new game is starting*" << std::endl;
}

const char *Game::chooseBotName(size_type id) {
	switch (id) {
	case 0:
		return "Bob";
	case 1:
		return "Harry";
	case 2:
		return "Gal";
	case 3:
		return "Chad";
	case 4:
		return "Todd";
	case 5:
		return "Jessica";
	case 6:
		return "Emma";
	case 7:
		return "Isabella";
	case 8:
		return "Olivia";
	case 9:
		return "Alex";
	}
	return "null";
}
bool Game::addBot(chips_t chips) {
	if (m_players.size() == m_players.capacity())
		return false;
	m_players.push_back(std::make_unique<Bot>(chips, chooseBotName(m_players.size()),
		m_players.size() < 5, &m_table));
	return true;
}
bool Game::addConsolePlayer(chips_t chips) {
	if (m_players.size() == m_players.capacity())
		return false;
	m_players.push_back(std::make_unique<ConsolePlayer>(chips, &m_table));
	return true;
}
Game::p_it Game::playerExit(p_it cit) {
	*m_output << '*' << cit->get()->getName() << " has lost and has been kicked*";
	*m_output << " id: " << cit->get()->getId() << std::endl << std::endl;
	return m_players.erase(cit);
}

Game::p_it Game::addCircular(p_it it) {
	if (it != m_players.end())
		it++;
	if (it == m_players.end())
		it = m_players.begin();
	return it;
}


std::ostream &Game::showLatestDecision(const Player &player) {
	*m_output << '*' << player.getName() << ' ';
	*m_output << player.getLatestDecision() << '*' << std::endl;
	return *m_output;
}
// true if raised
bool Game::doAction(p_it it) {
	Player *player = it->get();
	Action action = player->doAction();
	bool raised = false, newPot = false;

	if (action == Action::raise || action == Action::allIn) {
		m_curCallAmount = player->getLatestDecision().getAmount();
		raised = true;
	}

	if (action == Action::fold)
		m_folded++;
	else {
		newPot = m_potHandler.addToPots(*it, player->latestChipsTaken());
		if (player->getChips() == 0)
			brokePlayer(it); // a new pot wont open since player has 0 chips
	}
	showLatestDecision(*player);

	if (newPot)
		newPotToStream();

	return raised;
}
void Game::finalAction(p_it it) {
	// will reach this method when it doesn't have enough chips to call

	if (brokePlayer(it))
		newPotToStream();
}
void Game::noAction(p_it it) {
	*m_output << '*' << it->get()->getName() << "'s turn has been skipped as the player has"
		" no chips left*" << std::endl;
}
bool Game::brokePlayer(p_it it) {
	m_broke++;

	bool newPot = false;

	chips_t chips = it->get()->finalAction();
	if (chips) {
		if (it->get()->getLatestDecision().getAction() == Action::fold) {
			m_folded++;
			showLatestDecision(*it->get());
			return false;
		}

		newPot = m_potHandler.addToPots(*it, chips, true);

		*m_output << '*' << it->get()->getName() << " has insufficient funds and has"
			" called " << it->get()->possessiveAdjective() << " remainning chips (";
		func::commas(*m_output, chips) << "$)*" << std::endl << std::endl;
	}
	return newPot;
}
Game::p_it Game::blindsAndCards(p_it firstPlayer) {
	// m_players vector iterator
	p_it it = firstPlayer;
	int count = 0;
	while (count < 2) {
		if (it->get()->isFolded()) {
			it = addCircular(it);
			continue;
		}

		if (count == 0)
			it->get()->smallBlind();
		else if (count == 1)
			it->get()->bigBlind();
#if DEBUG
		if (it->get()->getChips() == 0)
			throw std::logic_error("player should've been kicked before this match started");
		else
#endif
		// if player.chips < small/big blind
		if (it->get()->getChips() < it->get()->getLatestDecision().getAmount())
			finalAction(it);
		else
			doAction(it);

		it = addCircular(it);
		count++;
	}

	for (p_ptr &player : m_players) // new cards for all the players
		player->drawHand(m_table.takeCardFromDeck(), m_table.takeCardFromDeck());

	*m_output << std::endl << "*cards distributed to players*" << std::endl << std::endl;

	return it;
}
bool Game::stage(p_it firstBetter) {
	if (m_table.currentStage() != Stage::preFlop)
		m_curCallAmount = 0;

	p_it it = firstBetter, stop = it;
	do {
		if (it->get()->isFolded()) { // if a player has folded then continue
			it = addCircular(it);
			continue;
		}

		// set the player's call amount before his turn in case someone raised
		it->get()->setCallAmount(m_curCallAmount);

		if (!it->get()->isBroke() && it->get()->canCallAmount(m_curCallAmount)) {
			try { it->get()->doTurn(); }
			catch (std::exception &e) {
				*m_output << "player #" << it->get()->getId() << "has thrown an exception "
					"in its doTurn method: " << std::endl;
				*m_output << e.what();
				it->get()->m_latestDecision.newDecision(Action::fold);
			}
			if (doAction(it)) // true if the player has raised
				stop = it;
		}
		else if (it->get()->getChips() > 0) { // if reached then: chips < call amount
			try { it->get()->notEnoughChips(); }
			catch (std::exception &e) {
				*m_output << "player #" << it->get()->getId() << "has thrown an exception "
					"in its notEnoughChips method: " << std::endl;
				*m_output << e.what();
				it->get()->m_latestDecision.newDecision(Action::fold);
			}
			finalAction(it);
		}
		else
			noAction(it);

		// set the player's call amount after his turn in case he raised so if everyone 
		// folded after him the match would still break
		it->get()->setCallAmount(m_curCallAmount);

		it = addCircular(it); // the next player

		// break if only one player is left after this player's turn but not if someone 
		// raised since the next player's last turn (aka if the next player still has chips 
		// he needs to call)
		if (playersLeft() <= 1 && m_curCallAmount == it->get()->getCallAmount())
			return true;

	} while (it != stop);

	postStageActions();
	return false;
}
void Game::postStageActions() {
	Stage stage = m_table.currentStage();
	if (stage == Stage::preFlop)
		m_table.firstDrawToTable(); // flop
	else if (stage != Stage::river)
		m_table.drawCardToTable(); // turn and river

	m_potHandler.postStage();
	resetDecisions();

	*m_output << std::endl;
	*m_output << m_potHandler << std::endl << std::endl;
	*m_output << m_table << std::endl << std::endl;
}
void Game::resetDecisions() {
	for (p_ptr &player : m_players)
		player->resetDecision();
}

// Things that need to happen at the start of every match.
void Game::preMatch() {
	m_matches++;

	// reset match dependant variables
	m_folded = 0;
	m_broke = 0;

	m_curBaseRaiseAmount = m_curCallAmount;
	m_table.reset();
	for (p_ptr &player : m_players)
		player->reset(m_curBaseRaiseAmount, m_curCallAmount);

	*m_output << "*A new match has started! (match #" << m_matches << ")*";
	*m_output << std::endl << std::endl << std::endl;
}
// Things that need to happen at the end of every match.
// @returns if the game should keep going or not.
void Game::postMatch(bool interrupted) {
	if (interrupted)
		interruptedMatch();

	m_potHandler.calcWinners(interrupted);

	playersToStream(*m_output);

	m_curCallAmount = m_curBaseRaiseAmount;
	if (m_matches % 3 == 0)
		m_curCallAmount *= 2;

	m_potHandler.reset(m_players);
}
void Game::interruptedMatch() {
	m_table.drawAllCardsToTable();
	postStageActions();
}

int Game::start() {
	p_it firstBetter = m_players.begin();
	bool interrupted; // true if a match didn't finish naturally
	bool loop = true;
	while (loop) { // game loop
		preMatch();

		if (playersLeft() == 1)
			break;

		interrupted = stage(blindsAndCards(firstBetter)); // pre flop

		for (int i = 0; i < 3 && !interrupted && playersLeft() > 1; i++)
			interrupted = stage(firstBetter);

		postMatch(interrupted);

		firstBetter = checkChips() ? /* true if someone was kicked */
			m_players.begin() : addCircular(firstBetter);

		loop = m_players.size() > 1;
	}

	ultimateWinner();

	*m_output << "*The game has ended*" << std::endl << std::endl;

	return 0;
}
bool Game::checkChips() {
	p_it it = m_players.begin();
	bool erased = false;
	while (it != m_players.end())
		if (it->get()->getChips() == 0) { // can't call next round
			it = playerExit(it);
			erased = true;
		}
		else
			it++;

	return erased;
}

void Game::ultimateWinner() {
	p_it winner;
	for (p_it it = m_players.begin(); it != m_players.end(); it++)
		if (!it->get()->isFolded())
			winner = it;
	*m_output << std::endl << std::endl;
	*m_output << "*The ultimate winner of the game is: " << winner->get()->getName();
	*m_output << '*' << std::endl << std::endl;
}

std::ostream &Game::playersToStream(std::ostream &output) const {
	output << "Players:" << std::endl;
	for (auto &player : m_players)
		output << *player << std::endl << std::endl;
	output << std::endl;
	return output;
}
std::ostream &Game::newPotToStream() const {
	return *m_output << "new pot added due to a broke player (" 
		<< m_potHandler.latestReason().getName() << "):"<< std::endl
		<< m_potHandler.latestPotAdded() << std::endl << std::endl;
}
std::ostream &operator<<(std::ostream &output, const Game &source) {
	output << "Poker Game, ";
	output << source.m_potHandler << " | table: " << source.m_table << std::endl;
	source.playersToStream(output);
	return output;
}
