
#ifndef DECK_H_
#define DECK_H_

#include "Card.h"

/**
 * Implamentation of a Deck type.
 *
 * A Deck is a squence of objects of type Card the represnts a deck of cards in poker.
 * You cannot operate on this clas as if it were an array, bet rather do operations on the
 * deck that are relevent in a game of poker. Memory mannagment happens within the class
 * so there is no reason to worry about that.
 */
class Deck {
public:
	using size_type = unsigned short;
	using signed_size_type = signed short;

private:
	Card *m_deck; // A dynamic array of cards.
	size_type m_curAmount; // The amount of cards in the array at a certain time and point.

public:
	// The max size that the deck array can be (a standerd poker deck size w/o jokers).
	static const size_type s_maxSize = 52;
	/**
	 * Defualt constructor for Deck class, allocates memory according to the MAX_SIZE macro.
	 * The memory gets deleted when the object goes out of scope or deleted. This constructor
	 * calls the newDeck() function (creating a new deck).
	 * 
	 * @throws std::bad_alloc as ne operator is used
	 */
	Deck();
	/**
	 * Copy constructor for Deck class, allocates memory according to the MAX_SIZE macro.
	 * The memory gets deleted when the object goes out of scope or deleted. This constructor
	 * copies the contents of the other deck.
	 * 
	 * @param other: the deck being copied.
	 * @throws std::bad_alloc as ne operator is used
	 */
	Deck(const Deck &other);
	/**
	 * Move constructor for Deck class, moves the other object's resources to this object 
	 * and nullifies the other object.
	 *
	 * @param other: the deck being moved.
	 * @throws std::bad_alloc as ne operator is used
	 */
	Deck(Deck &&other) noexcept;

	/**
	 * Method on this deck, creates a new deck of cards in the symbol order of: club, heart, spade,
	 * and diamond, and in the value order of Ace to King.
	 * This method does not reallocate new memory, it overrides the memory's previos contents.
	 */
	void newDeck() noexcept;
	/**
	 * Inline method for this deck, checks if the deck is full.
	 *
	 * @return true if the deck is full, false if it isn't.
	 */
	inline bool isFull() noexcept { return m_curAmount == s_maxSize; }
	/**
	 * Inline method for this deck, checks if an amount of cards can be added to the deck.
	 *
	 * @param add: the amount of cards to check if they can be added.
	 * @return true if the amount can be added, false if it can't.
	 */
	inline bool canAdd(int add) noexcept { return m_curAmount + add <= s_maxSize; }
	/**
	 * Inline method for this deck, checks if the deck is empty.
	 *
	 * @return true if the deck is empty, false if it isn't.
	 */
	inline bool isEmpty() noexcept { return m_curAmount == 0; }
	/**
	 * Inline method for this deck, checks if an amount of cards can be taken from the deck.
	 *
	 * @param take: the amount of cards to check if they can be taken.
	 * @return true if the amount can be taken, false if  it can't.
	 */
	inline bool canTake(int take) noexcept { return m_curAmount - take >= 0; }
	/**
	 * Method on this deck, shuffles the deck to a completely random sequence of cards.
	 * This method does not reallocate new memory, it simply switches the cards of the deck.
	 */
	void shuffle();

	/**
	 * Method on this deck, takes the top card of the deck.
	 *
	 * @return the card that is being taken from the deck.
	 * @throws out_of_range if you try to take a card from an empty deck.
	 */
	const Card takeTopCard();
	/**
	 * Method on this deck, takes a random card from the deck.
	 *
	 * @return the card that is being taken from the deck.
	 * @throws out_of_range if you try to take a card from an empty deck.
	 */
	const Card takeRandomCard();
	/**
	 * Method on this deck, places a card in the bottom of the deck.
	 *
	 * @param the card that is being placed in the deck.
	 * @throws out_of_range if you try to place a card in a full deck.
	 */
	void placeCardBottom(const Card &card);
	/**
	 * Method on this deck, places a card in a random position in the deck.
	 *
	 * @param the card that is being placed in the deck.
	 * @throws out_of_range if you try to place a card in a full deck.
	 */
	void placeCardRandom(const Card &card);

	/**
	 * Assignment operator for this deck, simply copies the contents of the other deck. 
	 *
	 * @param other: the deck that is being copied.
	 * @return this deck.
	 */
	Deck &operator=(const Deck &other) noexcept;
	/**
	 * Move assignment operator for this deck, simply moves the contents of the other deck 
	 * to this deck.
	 *
	 * @param other: the deck that is being moved.
	 * @return this deck.
	 */
	Deck &operator=(Deck &&other) noexcept;
	/**
	 * Operator for this deck, throws cards from the deck (cards thrown cannot be used).
	 *
	 * @param num: the cards that are being thrown.
	 * @return this deck.
	 * @throws out_of_range if you try to throw cards from an empty deck or if you try to throw
	 * more cards than the deck has.
	 */
	Deck &operator-=(size_type num);
	/**
	 * Operator for this deck, checks if the other deck is the same as this. for the decks to be the
	 * same they have to be exactly identical (evrey card in each deck has to be in the same place 
	 * in both decks).
	 *
	 * @param other: the deck that is being compared.
	 * @return true if the decks are identical, false if they aren't.
	 */
	bool operator==(const Deck &other) const noexcept;

	/**
	 * Overloaded operator << of std::ostream. Allows std::ostream to accept a Deck
	 * type to its << operator. In action, allows you to input a deck to an output stream in 
	 * this fasion: std::ostream << Deck. The deck outputs its members like so: 
	 * "Deck: *num* cards; contents: " + all its cards where as num is the amount of cards in the deck.
	 *
	 * @param output: the stream that the deck is outputted to.
	 * @param source: the deck that is outputted to the stream.
	 * @return the output stream that is recieved by the operator.
	 */
	friend std::ostream &operator<<(std::ostream &output, const Deck &source) noexcept;

	~Deck() noexcept;
};

#endif // !DECK_H_