
#include "Bot.h"
#include "../tools/random.h"


chips_t Bot::s_call = 0;
chips_t Raise::s_base = 0;


Bot::Bot()
	: Player(), m_turns(0), m_standards() {}
Bot::Bot(const Bot &other)
	: Player(other), m_turns(other.m_turns), m_standards(other.m_standards) {}
Bot::Bot(chips_t chips, const char *name, const Table *table)
	: Player(chips, name, table), m_turns(0), m_standards() {}


const Decision &Bot::raiseCall(const Raise &me, const Raise &enemy) {
	if (me > enemy)
		return m_latestDecision.newDecision(Action::raise, me.m_amount);
	if (me + m_standards < enemy && me.m_amount + m_standards.m_amount < Player::getChips())
		return m_latestDecision.newDecision(Action::fold);
	return m_latestDecision.newDecision(Action::call, s_call);
}

void Bot::setStandards() {
	int max = (Player::getChips() - s_call) / Raise::s_base;
	m_standards = max < 15 ? 5 : 4 * random::randInt(1, 3);
}

int Bot::rateHand() {
	int rate = 1;
	const Card &one = getHand().getFirstCard();
	const Card &two = getHand().getSecondCard();

	if (one == two)
		rate = 100;
	else if (one.getSymbol() == two.getSymbol())
		rate = 25;
	else if (Card::isSubsequent(one, two))
		rate = 10;

	rate *= (one.getValue() == 1 ? 14 : one.getValue()) +
		(two.getValue() == 1 ? 14 : two.getValue());

	if (rate == 2800)
		return 6;
	if (rate >= 2000)
		return 5;
	if (rate >= 1000)
		return 4;
	if (rate >= 400)
		return 4;
	if (rate >= 150)
		return 3;
	if (rate >= 50)
		return 2;
	if (rate >= 22)
		return 3;
	if (rate >= 19)
		return 2;
	return 1;
}
const Decision &Bot::preFlop() {
	if (m_turns > 4)
		m_turns = 1;
	if (m_turns == 1)
		setStandards();

	// max m_amount of raises accounting for my chips
	Raise max((Player::getChips() - s_call) / Raise::s_base);
	Raise emptyTable(rateHand()); // recommended raises depending on hand

	Raise enemy((s_call - Raise::s_base) / Raise::s_base); // the enemy raise this turn
	Raise me(emptyTable > max ? 0 : emptyTable.m_times); // how much I can raise this turn 

	// enemy raised too much for my standards at the beginning
	if (me.m_times + (int)((float)m_standards.m_times / 2.5) < enemy.m_times)
		return m_latestDecision.newDecision(Action::fold);

	if (enemy) // enemy has raised and Im willing to call or raise more
		return random::chance(40) && enemy <= me ?
			m_latestDecision.newDecision(Action::raise, me.m_amount) :
			m_latestDecision.newDecision(Action::call, s_call);

	// enemy didn't raise
	return random::chance(20)? /* random self raise */
		m_latestDecision.newDecision(Action::raise, me.m_amount) :
		m_latestDecision.newDecision(Action::call, s_call);
}


const Decision &Bot::doTurn() {
	/**
 	 * Simplest "human like" bot without spending too much time on it, that makes somewhat 
	 * random decisions to give it a human like effect. 
	 * Doesnt take note to what hands the other players may have based on the open cards on 
	 * the table. 
	 * Doesnt take note of the possible hands it could have based on probabilities of having 
	 * a good hand the naxt stage
	 */

	m_turns++;

	s_call = Player::getCallAmount();
	Raise::s_base = Player::getBaseRaise();

	// max m_amount of raises accounting for my chips
	Raise max((Player::getChips() - s_call) / Raise::s_base);
	const Stage stage = Player::getCurrentStage();

	if (s_call > max.m_amount) // phisically can't call
		return m_latestDecision.newDecision(Action::fold);
	bool shouldnt = false; // true if the bot shouldnt call
	if (m_standards.m_times > max.m_amount)
		shouldnt = true;

	if (stage == Stage::preFlop) // pre flop satge
		return preFlop();

	// post flop stages...
	Player::checkHand();
	const HandAttempt &best = Player::getBestHand();

	// the enemy raise this turn
	Raise enemy(s_call ? (s_call - Raise::s_base) / Raise::s_base : 0); 

	HandRank rank = best.getHandRank();

	Raise high(((int)rank + m_standards.m_times) * random::randInt(1, 2));

	if (rank >= HandRank::fullHouse && random::chance(80)) {
		high = high > max ? max : high;
		return raiseCall(high, enemy);
	}

	// the chance to raise randomly, paired with the actual raise
	std::pair<bool, Raise> rand = std::make_pair(random::chance((int)rank * 10),
		Raise(((int)rank + 1) * random::randInt(1, 3)));

	// a ranking of whether I sould raise or not
	Raise me((int)rank * 3 + (rank != HandRank::highCard ? (int)stage : 0) + 1);

	if (enemy > max)
		if (me.m_times < 15)
			return m_latestDecision.newDecision(Action::fold);
		else
			return m_latestDecision.newDecision(Action::call, max.m_amount);

	if (enemy && shouldnt && me.m_times < 8)
		return m_latestDecision.newDecision(Action::fold);

	if (rand.first)
		return raiseCall(rand.second, enemy);

	if (me.m_times > 12)
		return raiseCall(me, enemy);

	if (me + m_standards < enemy)
		return m_latestDecision.newDecision(Action::fold);


	return m_latestDecision.newDecision(Action::call, getCallAmount());
}

const Decision &Bot::notEnoughChips() {
	return m_latestDecision.newDecision(Action::call, Player::getChips());
}


Bot &Bot::operator=(const Bot &other) {
	if (this == &other)
		return *this;

	Player::operator=(other);
	m_turns = other.m_turns;
	m_standards = other.m_standards;

	return *this;
}

Raise &Raise::operator=(const Raise &other) {
	if (this == &other)
		return *this;

	m_times = other.m_times;
	m_amount = other.m_amount;

	return *this;
}
