
#ifndef CONSOLE_PLAYER_H_
#define CONSOLE_PLAYER_H_

#include "../game_related_items/Player.h"


class ConsolePlayer : public Player {
public:
	ConsolePlayer(const ConsolePlayer &other);
	ConsolePlayer(ConsolePlayer &&other) noexcept;
	ConsolePlayer(chips_t chips, const Table *table);

	virtual const Decision &doTurn();
	virtual const Decision &notEnoughChips();
	inline virtual const char *possessiveAdjective() { return "his/her"; }

	virtual ConsolePlayer &operator=(ConsolePlayer &&other) noexcept;
	virtual ConsolePlayer &operator=(const ConsolePlayer &other);

	inline virtual ~ConsolePlayer() noexcept {}
};

#endif // !CONSOLE_PLAYER_H_
