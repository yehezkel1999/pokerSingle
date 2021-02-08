
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <iostream>

/**
 * Namespace func.
 *
 * This namespace contains miscellaneous functions do various actions and calculations.
 */
namespace func {
	/**
	 * UNTESTED
	 * 
	 * Function commas, takes a const unsigned int and outputs it with a comma every three
	 * digits to the given output stream (###,###,###).
	 * 
	 * @param output: the output stream the number is outputted to.
	 * @param number: the number that gets outputted with commas.
	 * @return the output stream that is recieved by the function.
	 * @exceptsafe This method does not throw exceptions.
	 */
	std::ostream &commas(std::ostream &output, const unsigned int number);
	/**
	 * UNTESTED
	 * 
	 * Function commas, takes a const int and outputs it with a comma every three
	 * digits to the given output stream (###,###,###).
	 *
	 * @param output: the output stream the number is outputted to.
	 * @param number: the number that gets outputted with commas.
	 * @return the output stream that is recieved by the function.
	 * @exceptsafe This method does not throw exceptions.
	 */
	std::ostream &commas(std::ostream &output, const int number);

	template<typename Base, typename T>
	inline bool instanceof(const T *) {
		return std::is_base_of<Base, T>::value;
	}
}




#endif // !FUNCTIONS_H_
