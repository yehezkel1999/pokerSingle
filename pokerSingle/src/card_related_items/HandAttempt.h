
#ifndef HAND_ATTEMPT_H_
#define HAND_ATTEMPT_H_

#include "Card.h"

/**
 * Implamentation of a value's relevance enum. 
 *
 * This enum decides the relevance of a value in a hand, whether its a card's value or 
 * the type of the hand. This enum's value is used in powers of 14, and multiplies the 
 * outcome with its respective value (wether it be a card value or the type of hand). 
 * - pow(14, Relevence) * value.
 */
enum class Relevance {
	least, // the least most relevent value, for example, the 9 in the hand: 9JQKA
	fourth, // the fourth most relevent value, for example, the 5 in the hand: 566JQ
	third, // the third most relevent value, for example, the 8 in the hand: 2558A
	second, // the second most relevent value, for example, the 3 in the hand: 3355K
	first, // the most relevent value, for example, 4 in the hand: 2344A
	rank // the hand's rank (the type of poker hand)
};

/**
 * Implamentation of a HandRank enum.
 *
 * This enum ranks the types of possible poker hands from lowest to highest and uses its 
 * values to determine the strength of the hand. Hand ranks from lowest to highest: 
 * high card, pair, two pair, three of a kind, straight, flush, full house, four of a kind, 
 * straight flush, royal flush.
 */
enum class HandRank {
	highCard, pair, twoPair, threeOfaKind, straight, flush, fullHouse, fourOfaKind,
	straightFlush, royalFlush
};

/**
 * Implamentation of a HandAttempt type.
 *
 * A HandAttempt type consists of a sequence of 5 cards (and only 5) and calculates what 
 * type of poker hand it is, ranging from high card to royal flush, using the HandRank enum. 
 * The card sequence will be sorted (if it's not already) from highest (Ace) to lowest (2), 
 * with the exception being the sequence 2345A which will be reorginized to: A2345. 
 * In addition, A HandAttempt gives each object a hashcode accourding to its hand type in 
 * poker, and accourding to each card value in the hand, and its relevance to its respective 
 * hand type in poker. For example, given the hand 223AA (with non consistent symbols), the 
 * hashcode is calculated as a two pair, then adds the hashcode for an Ace, then the 2, 
 * then the 3, all being multiplyed in a formula accourding to its respectful relevance, 
 * the two pair being most relevant and the 3 being least relevant.
 */
class HandAttempt {
public:
	using size_type = unsigned short;
	using signed_size_type = signed short;

private:
	Card m_cards[5]; // The sequence of cards that is being calculated and ranked.

	HandRank m_handRank; // The rank given to the hand attempt (the type of poker hand).
	// The hashcode given to the hand attempt accourding to each card and its relevance.
	unsigned int m_hashcode; // the hashcode given to the object depending on the given hand.

	void highCardHandle();
	void twoPairHandle();
	bool twoPairSequence();
	void timesOfaKindHandle();
	bool fullHouseSequence();
	void fullHouseHandle();
	size_type timesOfaKind();
	bool straightSequence();
	bool sameSymbolSequence();

	void addHash(unsigned int amount, Relevance relevance) noexcept;
	inline void addHash(const Card &card, Relevance relevance) noexcept
	{ addHash(Card::getOpposingValue(card.getValue()), relevance) ;}
public:
	// A hand attempt can only have 5 cards, no more no less (standard poker rules).
	static const size_type s_size = 5;

	/**
	 * Defualt constructor for HandAttempt class, allocates 5 cards, but leaves the cells 
	 * blank. The memory gets deleted when the object goes out of scope or deleted. all 
	 * other values are set to their defualt state.
	 */
	HandAttempt() noexcept;
	/**
	 * Copy constructor for HandAttempt class, allocates 5 cards, and copies the values 
	 * from other as well as for the other members. The memory gets deleted when the object 
	 * goes out of scope or deleted.
	 *
	 * @parm other: the object being copied.
	 */
	HandAttempt(const HandAttempt &other) noexcept;
	/**
	 * Constructor for HandAttempt class, allocates 5 cards, and copies the values from the 
	 * inputed sequence of cards. The memory gets deleted when the object goes out of scope 
	 * or deleted. This constructor calculates the hand using the calcFiveCards method. If 
	 * you input an array of less than 5 cells then bad things will happen.
	 *
	 * @parm cards: the sequence of cards. CAN ONLY BE 5 CELLS!!!
	 * @throws bad things will happen if cards is less than 5 cells long.
	 */
	HandAttempt(const Card *cards) noexcept;

	/**
	 * Method for this HandAttempt, check if the five cards given are of any special hand 
	 * pattern in poker. This function only works for a sorted sequence of cards! 
	 * The sequence needs to be sorted by the value of the cards, from smallest to highest. 
	 * The order of the card symbols doesn't need to be in order, to determine the poker 
	 * hand. If the sequence is not sorted it will be sorted in the method. 
	 * In addition, this method give each and every single hand a hashcode. If two 
	 * HandAttempt objects have the same hashcode, then they have the same card. 
	 * The hashcode is determined by the type of poker hand (the better the bigger hashcode) 
	 * and by the values of the cards (the bigger the values the bigger hashcode, ace being 
	 * the biggest).
	 */
	void calcFiveCards();
	/**
	 * Method for this HandAttempt, wipes out all data, allowing you to treat this object 
	 * as if it was just constructed (with the same cards as before). Does not free memory.
	 */
	void wipe() noexcept;
	/**
	 * Method for this hand attempt, translate the hand's rank in poker to a c string type 
	 * format.
	 *
	 * @return a null charcter termenated c string, describing the hand rank of the hand 
	 * attempt.
	 */
	const char *handRankToString() const noexcept;
	/**
	 * Inline method for this hand attempt, gets the hand rank in poker of the hand attempt. 
	 *
	 * @return the hand rank in poker of this hand attempt.
	 */
	inline HandRank getHandRank() const noexcept { return m_handRank; }
	/**
	 * Inline method for this hand attempt, gets the hash code of this specific hand 
	 * attempt. 
	 *
	 * @return the hash code of this hand attempt.
	 */
	inline unsigned int getHashCode() const noexcept { return m_hashcode; }

	/**
	 * Inline operator for this hand attempt, checks if the other hand attempt has the same 
	 * cards.
	 *
	 * @param other: the HandAttempt that is being compared.
	 * @return true if the other HandAttempt is the same, false if it isn't.
	 */
	inline bool operator==(const HandAttempt &other) const noexcept { return m_hashcode == other.m_hashcode; }
	/**
	 * Inline operator for this hand attempt, checks if the other hand attempt has different 
	 * cards.
	 *
	 * @param other: the HandAttempt that is being compared.
	 * @return true if the other HandAttempt is different, false if it isn't.
	 */
	inline bool operator!=(const HandAttempt &other) const noexcept { return !(*this == other); }
	/**
	 * Inline operator for this hand attempt, checks if the this hand attempt has worse
	 * cards than the other (would lose in poker).
	 *
	 * @param other: the HandAttempt that is being compared.
	 * @return true if the this HandAttempt is worse, false if it isn't.
	 */
	inline bool operator<(const HandAttempt &other) const noexcept { return m_hashcode < other.m_hashcode; }
	/**
	 * Inline operator for this hand attempt, checks if the this hand attempt has better 
	 * cards than the other (would win in poker).
	 *
	 * @param other: the HandAttempt that is being compared.
	 * @return true if the this HandAttempt is better, false if it isn't.
	 */
	inline bool operator>(const HandAttempt &other) const noexcept { return other < *this; }
	/**
	 * Inline operator for this hand attempt, checks if the this hand attempt has worse,
	 * or the same, cards than the other (would lose or tie in poker).
	 *
	 * @param other: the HandAttempt that is being compared.
	 * @return true if the this HandAttempt is worse or the same, false if it isn't.
	 */
	inline bool operator<=(const HandAttempt &other) const noexcept { return !(other < *this); }
	/**
	 * Inline operator for this hand attempt, checks if the this hand attempt has better,
	 * or the same, cards than the other (would win or tie in poker).
	 *
	 * @param other: the HandAttempt that is being compared.
	 * @return true if the this HandAttempt is better or the same, false if it isn't.
	 */
	inline bool operator>=(const HandAttempt &other) const noexcept { return !(*this < other); }

	/**
	 * Operator for this hand attempt, gives the card that is in the place of the given 
	 * parameter. The card given is modifiable. 
	 * This operator may only be called if the cards have not yet been calculated (if the 
	 * method calcFiveCards hasn't been called yet (meaning only if you constructed this 
	 * object with the default constructor, or wiped the object before this).
	 *
	 * @param place: the place of the card in the sequence of cards (ranging from 0 to 4). 
	 * @return the card from the given place in the sequence. 
	 * @throw std::out_of_range exception if place is smaller than 0 or bigger than 4. 
	 * @throw std::logic_error exception if this operator is called on an already calculated 
	 * HandAttempt. 
	 */
	Card &operator[](size_type place);
	/**
	* Operator for this hand attempt, simply copies member values from the other card.
	*
	* @param other: the HandAttempt that is being copied.
	*/
	HandAttempt &operator=(const HandAttempt &other) noexcept;

	/**
	 * Overloaded operator << of std::ostream. Allows std::ostream to accept a HandAttempt 
	 * type to its << operator. In action, allows you to input a HandAttempt to an output 
	 * stream in this fasion: std::ostream << HandAttempt. The hand attempt outputs its 
	 * members like so: "hand: " + hand rank + " | hashcode: " + hash code + " | cards:" + 
	 * *the cards that are in the hand attempt*. For example: 
	 * hand: straight flush | hashcode: 4802000 | cards: 
	 * (9,diamond)(10,diamond)(Jack,diamond)(Queen,diamond)(King,diamond)
	 * 
	 * @param output: the stream that the hand attempt is outputted to.
	 * @param source: the hand attempt that is outputted to the stream.
	 * @return the output stream that is recieved by the operator.
	 */
	friend std::ostream &operator<<(std::ostream &output, const HandAttempt &source) noexcept;

	inline ~HandAttempt() noexcept {}
};



#endif // !HAND_ATTEMPT_H_
