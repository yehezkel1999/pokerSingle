
#ifndef HAND_H_
#define HAND_H_

#include "Card.h"

/**
 * Implamentation of a Hand type.
 *
 * A Hand is a class with two members of Card type that represnt the hand of the player.
 * This class does not have a defualt constructor as a player cannot have a hand with no
 * cards.
 */
class Hand
{
	Card m_one; // the fist card.
	Card m_two; // the second card.

	/**
	 * Default constructor for Hand class, creates 2 nulled cards.
	 */
	Hand() noexcept;
	/**
	 * Constructor for Hand class, recieves the two cards that the hand will consist of.
	 * The actual call of this constructor will most likely consist of cards drawn from
	 * a deck, NOT random cards that get created.
	 *
	 * @param one: the first card of the hand.
	 * @param two: the second card of the hand.
	 */
	Hand(const Card &one, const Card &two) noexcept;
	/**
	 * Copy constructor for Hand class, copies the cards from the other hand.
	 *
	 * @param other: the other hand.
	 */
	Hand(const Hand &other) noexcept;

	/**
	 * Operator for this card, copies the cards from the other hand.
	 *
	 * @param other: the hand that is being copied.
	 * @return this hand.
	 */
	Hand &operator=(const Hand &other) noexcept;

	/**
	 * Method for this hand, sets the cards of the hand, can only call this method if this
	 * hand is empty (this object was constructed with the default constructor or cleared).
	 *
	 * @param one: the first card of the hand.
	 * @param two: the second card of the hand.
	 * @throw std::logic_error if this method is called while the hand already contains cards.
	 */
	void drawHand(const Card &one, const Card &two);
	/**
	 * Method for this hand, clears the hand from cards.
	 */
	void clear() noexcept;

public:

	/**
	 * Inline method for this hand, gets the first card of the hand.
	 *
	 * @return the first card.
	 */
	inline const Card &getFirstCard() const noexcept { return m_one; }
	/**
	 * Inline method for this hand, gets the second card of the hand.
	 *
	 * @return the second card.
	 */
	inline const Card &getSecondCard() const noexcept { return m_two; }

	/**
	 * Operator for this card, checks if the other hand is equal to this hand. For this to
	 * be true the cards in the hand have to be exactly the same.
	 *
	 * @param other: the hand that is being compared.
	 * @return true if the other hand is a different hand, false if it isn't.
	 * @throws nullptr_exception thrown if one of the cards is null.
	 */
	inline bool operator==(const Hand &other) { return m_one == other.m_one && m_two == other.m_two; }
	/**
	 * Operator for this card, checks if the other hand is a different hand.
	 *
	 * @param other: the hand that is being compared.
	 * @return true if the other hand is a different hand, false if it isn't.
	 * @throws nullptr_exception thrown if one of the cards is null.
	 */
	inline bool operator!=(const Hand &other) { return !(*this == other); }

	/**
	 * Overloaded operator << of std::ostream. Allows std::ostream to accept a Hand
	 * type to its << operator. In action, allows you to input a hand to an output stream in
	 * this fasion: std::ostream << Hand. The hand outputs its members like so:
	 * (card #1)(card #2)
	 *
	 * @param output: the stream that the hand is outputted to.
	 * @param source: the hand that is outputted to the stream.
	 * @return the output stream that is recieved by the operator.
	 */
	friend std::ostream &operator<<(std::ostream &output, const Hand &source) noexcept;

	friend class Player;

	inline ~Hand() noexcept {}
};

#endif // !HAND_H_
