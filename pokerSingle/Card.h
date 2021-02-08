#ifndef CARD_H_
#define CARD_H_

#include <iostream>

/**
 * Implamentation of a card symbol enum.
 * 
 * The int value of the enum is irrelevent, what matters is the actual symbol
 * name in order to determine the symbol of the card.
 */
enum class Symbol {
	club, heart, spade, diamond, null
};

/**
 * Implamentation of a Card type.
 * 
 * A Card type consists of 2 members, the Card's symbol and its value.
 * The Card's symbol is an enum that has 5 values: club, heart, spade, diamond, null.
 * The Card's is an int that can range from 1-13 (including 1 and 13).
 * For a card to be null, the defualt constructor will have to be called,
 * its symbol would be null and its value would be 0. Cannot operate on a nulled Card!
 */
class Card
{
	int m_value; // The value of the card, an int ranging from 1-13 (including 1 and 13).
	Symbol m_symbol; // The symbol of the card, could be club, heart, spade, diamond or null.

public:
	/**
	 * Defualt constructor for Card class, sets values to null.
	 *
	 * @exceptsafe This constructor does not throw exceptions.
	 */
	Card();
	/**
	 * Copy constructor for Card class, simply copies member values from the other card.
	 *
	 * @param other: the Card that is being copied.
	 * @exceptsafe This constructor does not throw exceptions.
	 */
	Card(const Card &other);
	/**
	 * Constructor for Card class, initializes a Card with the inputed values for its members.
	 *
	 * @param value: the value of the Card that is being created.
	 * @param symbol: the symbol of the Card that is being created.
	 * @throws invalid_card_value if the value inputed is lower than 1 or greater than 13.
	 */
	Card(int value, Symbol symbol);

	/**
	 * Inline method for this card, gets the value of the card.
	 *
	 * @return the value of the card.
	 * @exceptsafe This method does not throw exceptions.
	 */
	inline int getValue() const { return m_value; }
	/**
	 * Inline method for this card, gets the symbol of the card.
	 *
	 * @return the symbol of the card.
	 * @exceptsafe This method does not throw exceptions.
	 */
	inline Symbol getSymbol() const { return m_symbol; }
	/**
	 * Inline method for this card, checks if the card is null.
	 *
	 * @return true if the card is null, false if it isn't.
	 * @exceptsafe This method does not throw exceptions.
	 */
	inline bool isNull() const { return m_symbol == Symbol::null || m_value == 0; }
	/**
	 * Method for this card, checks if the other card is the same colour.
	 *
	 * @param other: the Card that is being compared.
	 * @return true if the cards have the same colour, false if they don't.
	 * @throws nullptr_exception thrown if this or the other card is null.
	 */
	bool sameColour(const Card &other) const;
	/**
	 * Method for this card, checks if the other card is the same symbol.
	 *
	 * @param other: the Card that is being compared.
	 * @return true if the cards have the same symbol, false if they don't.
	 * @throws nullptr_exception thrown if this or the other card is null.
	 */
	bool sameSymbol(const Card &other) const;
	/**
	 * Method for this card, checks if the other card has the same value.
	 *
	 * @param other: the Card that is being compared.
	 * @return true if the cards have the same values, false if they don't.
	 * @throws nullptr_exception thrown if this or the other card is null.
	 */
	bool sameValue(const Card &other) const;
	/**
	 * Method for this card, translate the card's value to a c string type format.
	 * The card value gets translated to the real card name asosiating it. For example
	 * the value of 1 will be translated to "Ace" and 13 will be tanslated to "King".
	 *
	 * @return a null charcter terminated c string, describing the symbol of the card.
	 * @throws nullptr_exception thrown if this card is null.
	 */
	const char *valueToString() const;
	/**
	 * Method for this card, translate the card's symbol to a c string type format.
	 *
	 * @return a null charcter termenated c string, describing the symbol of the card.
	 * @throws nullptr_exception thrown if this card is null.
	 */
	const char *symbolToString() const;

	/**
	 * Operator for this card, adds the cards' values.
	 *
	 * @param other: the Card's value that is being added.
	 * @return the sum of the cards' values.
	 * @throws nullptr_exception thrown if this or the other card is null.
	 */
	int operator+(const Card &other) const;
	/**
	 * Operator for this card, substracts the other card's value from this value.
	 *
	 * @param other: the Card's value that is being subtracted.
	 * @return the differance of the cards' values.
	 * @throws nullptr_exception thrown if this or the other card is null.
	 */
	int operator-(const Card &other) const;
	/**
	 * Operator for this card, adds the a number to the card's value.
	 *
	 * @param num: the number that is being added.
	 * @return the sum of the card's and the number's value.
	 * @throws nullptr_exception thrown if this card is null.
	 */
	int operator+(int num) const;
	/**
	 * Operator for this card, substracts the number from the card's value.
	 *
	 * @param num: the number that is being subtracted.
	 * @return the differance of the card's and number's values.
	 * @throws nullptr_exception thrown if this card is null.
	 */
	int operator-(int num) const;

	/**
	* Operator for this card, simply copies member values from the other card.
	*
	* @param other: the Card that is being copied.
	* @exceptsafe This operator does not throw exceptions.
	*/
	Card &operator=(const Card &other);
	/**
	 * Operator for this card, checks if the other card is the same card. for the card
	 * to be the same both the card's symbol and its value have to be the same.
	 *
	 * @param other: the Card that is being compared.
	 * @return true if the other card is the same card, false if it isn't.
	 * @throws nullptr_exception thrown if this or the other card is null.
	 */
	bool operator==(const Card &other) const;
	/**
	 * Operator for this card, checks if the other card is a different card.
	 * 
	 * @param other: the Card that is being compared.
	 * @return true if the other card is a different card, false if it isn't.
	 * @throws nullptr_exception thrown if this or the other card is null.
	 */
	inline bool operator!=(const Card &other) const { return !(*this == other); }
	/**
	 * Operator for this card, checks if this card is worse in poker than the other.
	 * For example an Ace (value: 1) is better than a King (value: 12).
	 * @param other: the Card that is being compared.
	 *
	 * @return true if the other card is worse in poker, false if it isn't.
	 * @throws nullptr_exception thrown if this or the other card is null.
	 */
	bool operator<(const Card &other) const;
	/**
	 * Operator for this card, checks if this card is better in poker than the other.
	 * For example an Ace (value: 1) is better than a King (value: 12).
	 *
	 * @param other: the Card that is being compared.
	 * @return true if this card is better in poker than the other, false if it isn't.
	 * @throws nullptr_exception thrown if this or the other card is null.
	 */
	inline bool operator>(const Card &other) const { return other < *this; }
	/**
	 * Operator for this card, checks if this card has a worse or equal value in
	 * poker than the other.
	 * For example an Ace (value: 1) is better than a King (value: 12).
	 *
	 * @param other: the Card that is being compared.
	 * @return true if this card has a worse or equal value in poker than the other,
	 * false if it doesn't.
	 * @throws nullptr_exception thrown if this or the other card is null.
	 */
	inline bool operator<=(const Card &other) const { return !(other < *this); }
	/**
	 * Operator for this card, checks if this card has a better or equal value in 
	 * poker than the other.
	 * For example an Ace (value: 1) is better than a King (value: 12).
	 *
	 * @param other: the Card that is being compared.
	 * @return true if this card has a better or equal value in poker than the other,
	 * false if it doesn't.
	 * @throws nullptr_exception thrown if this or the other card is null.
	 */
	inline bool operator>=(const Card &other) const { return !(*this < other); }

	/**
	 * Overloaded operator << of std::ostream. Allows std::ostream to accept a Card
	 * type to its << operator. In action, allows you to input a card to an output stream in 
	 * this fasion: std::ostream << Card. The card outputs its members like so:
	 * '(' + value + ',' + symbol + ')', some examples: (Ace,diamond) (King,club) (7,spade)
	 *
	 * @param output: the stream that the card is outputted to.
	 * @param source: the card that is outputted to the stream.
	 * @return the output stream that is recieved by the operator.
	 * @exceptsafe This operator does not throw exceptions.
	 */
	friend std::ostream &operator<<(std::ostream &output, const Card &source);
	/**
	 * Static function for card class, sorts the given sequence of cards by value frow low to high
	 * in a time complexety of (n^2+n)/2 (does not consider card symbols).
	 *
	 * @param cards: a pointer to the array of cards to sort.
	 * @param size: the size of the array.
	 * @exceptsafe This function does not throw exceptions.
	 */
	static void sortCards(Card *cards, int size);
	/**
	 * Static function for card class, checks if the given array of cards is sorted by the 
	 * cards' values from low to high.
	 *
	 * @param cards: a pointer to the array of cards to sort.
	 * @param size: the size of the array.
	 * @return true if cards is sorted, false if it isnt
	 * @exceptsafe This function does not throw exceptions.
	 */
	static bool isSorted(const Card *cards, int size);
	/**
	 * Static inline function for card class, returns the oposing value of an ace.
	 * if value is not an ace value this function returns the parameter.
	 *
	 * @param value: an ace value (1 or 14).
	 * @return 1 if value is 14 or, 14 if the value is 1.
	 * @exceptsafe This function does not throw exceptions.
	 */
	static inline int getOpposingValue(int value) { return value == 1 ? 14 : (value == 14 ? 1 : value); }
	/**
	 * Static function for card class, checks if the cards inputted are subsequent of each
	 * other.
	 *
	 * @param one: the first card.
	 * @param two: the second card.
	 * @return true if the cards are subsequent of eachother, false if they aren't.
	 * @exceptsafe This function does not throw exceptions.
	 */
	static bool isSubsequent(const Card &one, const Card &two);

	~Card();
};

#endif // !CARD_H_