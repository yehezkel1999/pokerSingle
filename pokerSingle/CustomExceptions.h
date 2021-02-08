
#ifndef CUSTOM_EXCEPTIONS_H_
#define CUSTOM_EXCEPTIONS_H_

#include <iostream>
#include "Flags.h"

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
	invalid_card_value(const char* message);
	virtual ~invalid_card_value();
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
	nullptr_exception(const char* message);
	virtual ~nullptr_exception();
};


#endif // DEBUG

#endif // !CUSTOM_EXCEPTIONS_H_
