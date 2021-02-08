
#ifndef TABLE_H_
#define TABLE_H_

#include "Deck.h"
#include <memory>


/**
 * Implamentation of a Stage enum.
 *
 * This enum represent stages of a poker game
 */
enum class Stage {
	preFlop, flop, turn, river
};

class Hand;
class HandAttempt;

/**
 * Implamentation of a Table type.
 *
 * A Table type is a representation of a poker table, it consists of a deck of cards and a 
 * sequence of visible cards that are open on the table. Memory mannagment happens within 
 * the class so there is no reason to worry about that.
 */
class Table {
public:
	using size_type = unsigned short;
	using signed_size_type = signed short;

private:
	Deck m_deck; // a deck of cards, gets shuffled at the start of the game.
	Card m_table[5]; // the cards in the game that are on the table (has up to 5 cards).
	size_type m_curAmount; // the current amout of cards that are on the table.

	void calcSixCards(const Card *cards, std::shared_ptr<HandAttempt> handAttempt) const;
	void calcSevenCards(const Card *cards, std::shared_ptr<HandAttempt> handAttempt) const;
public:
	static const size_type s_size = 5; // the maximum size of the table array that is in every object.

	/**
	* Defualt constructor for Table class, allocates a memory of 5 cards. The memory gets deleted
	* when the object goes out of scope or deleted. This constructor creates a new deck of cards
	* and shuffles them.
	*/
	Table();

	/**
	* Method for this table class, resets the table to its original state, as if the 
	* constructor was called.
	*
	* @returns the card drawn from the deck.
	*/
	void reset() noexcept;
	/**
	* Method for this table class, draws the top card from the deck that is essentially on the 
	* table and returns it.
	*
	* @returns the card drawn from the deck.
	*/
	const Card takeCardFromDeck() noexcept;
	/**
	* Method for this table class, draws cards (one by one) from the top of the deck to the 
	* table till the table is full.
	*/
	void drawAllCardsToTable() noexcept;
	/**
	* Method for this table class, draws the top card from the deck that is essentially on
	* the table and places it openly on the table (places them in the sequence of cards that
	* are open on the table).
	*/
	void drawCardToTable() noexcept;
	/**
	* Method for this table class, the first draw of every game, draws 3 cards from the top 
	* of the deck that is essentially on the table and places them openly on the table 
	* (places them in the sequence of cards that are open on the table).
	*/
	void firstDrawToTable() noexcept;
	/**
	* Method for this table class, checks if there are open cards on the table
	*
	* @returns true if there are open cards on the table, false if there aren't.
	*/
	inline bool areCardsOnTable() const noexcept { return m_curAmount > 0; }
	/**
	* Method for this table class, checks the current stage of the poker match (depending 
	* on how many cards are currently on the table).
	*
	* @returns the current stage.
	*/
	Stage currentStage() const noexcept;
	/**
	* Method for this table class, gets the sequence of cards that are open on the table 
	* and can be used by players for their hand attempt. This array's size is stored in 
	* the static member Table::size. The current amount of cards that the array contains 
	* can be checked by checking whether the card iterated is null or not.
	*
	* @returns a sequence of the cards that are open on the table.
	*/
	const Card *cardsOnTable() const noexcept { return m_table; }
	/**
	 * Method for this Table, gives a inputed hand, the best possible set of 5 cards, the 
	 * best HandAttempt, to win in a poker match, given the current cards that are open on 
	 * the table. This method stores the calculation in the given memory location.
	 * There is no need to call this method more than once a turn (or if no cards have been 
	 * opened to the table since the last turn) as the calculation is stored in the memory 
	 * location parameter.
	 *
	 * @param hand: the hand of the player that is being given his best hand attempt in 
	 * poker.
	 * @param memoryLocation: the loocation the hand attempt will be stored. This pointer 
	 * MUST be allocatted beforehand.
	 * @return the best hand attempt the hand inputed produces (allocated memory).
	 * @throw std::logic_error if an attempt to calculate a player's hand with no cards on 
	 * the table is made.
	 */
	void calculateHand(const Hand &hand, std::shared_ptr<HandAttempt> handAttempt) const;

	/**
	 * Overloaded operator << of std::ostream. Allows std::ostream to accept a Table type 
	 * to its << operator. In action, allows you to input a Table to an output stream in 
	 * this fasion: std::ostream << Table. The table outputs its members like so: 
	 * "Table, open cards: " + *the cards that are open on the table*. For example: 
	 * "Table, open cards: (5,club)(Queen,diamond)(10,heart)"
	 *
	 * @param output: the stream that the hand attempt is outputted to.
	 * @param source: the table that is outputted to the stream.
	 * @return the output stream that is recieved by the operator.
	 */
	friend std::ostream &operator<<(std::ostream &output, const Table &source) noexcept;

	inline ~Table() noexcept {}
};


#endif // !TABLE_H_
