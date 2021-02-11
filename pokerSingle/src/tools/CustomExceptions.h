
#ifndef CUSTOM_EXCEPTIONS_H_
#define CUSTOM_EXCEPTIONS_H_

#include <iostream>
#include "../Flags.h"

#if DEBUG

/**
 * Implamentation of an invalid card value.
 * 
 * Throw this exception if the card value is smaller than 1 or bigger than 13.
 * This exception inherits std::out_of_range exception.
 */
class invalid_card_value :std::out_of_range {
public:
	/**
	 * Throw this exception if the card value is smaller than 1 or bigger than 13.
	 * This exception inherits std::out_of_range exception.
	 */
	inline invalid_card_value(const char *message) noexcept : std::out_of_range(message) {}
	inline virtual ~invalid_card_value() noexcept {}
};

/**
 * Implamentation of a null pointer exception.
 * 
 * Throw this exception if a method is attempted to be used on an object that is essentially null.
 * This exception inherits std::out_of_range exception.
 */
class nullptr_exception :std::invalid_argument {
public:
	/**
	 * Throw this exception if a method is attempted to be used on an object that is essentially null.
	 * This exception inherits std::invalid_argument exception.
	 */
	inline nullptr_exception(const char *message) noexcept : std::invalid_argument(message) {}
	inline virtual ~nullptr_exception() noexcept {}
};

#endif // DEBUG

#endif // !CUSTOM_EXCEPTIONS_H_
