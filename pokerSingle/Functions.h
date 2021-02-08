
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
	inline bool instanceof(const T *) {	return std::is_base_of<Base, T>::value; }

	/**
	 * Template function for an array, checks if the given array is sorted from low to high. 
	 * Uses item's operator<.
	 *
	 * @param arr: a pointer to the array.
	 * @param size: the size of the array.
	 * @return true if array is sorted, false if it isnt.
	 */
	template <typename T>
	bool isSorted(const T *arr, size_t size) noexcept;

	/**
	 * Template function for an array, sorts the given array from low to high in a time 
	 * complexety of (n^2+n)/2. Uses item's default constructor, assignment operator and 
	 * operator<.
	 *
	 * @param arr: a pointer to the array.
	 * @param size: the size of the array.
	 */
	template <typename T>
	void sort(T *arr, size_t size) noexcept;
}

template <typename T>
bool func::isSorted(const T *arr, size_t size) noexcept {
	for (size_t i = 1; i < size; i++)
		if (arr[i] < arr[i - 1])
			return false;
	return true;
}
template <typename T>
void func::sort(T *arr, size_t size) noexcept {
	T temp;
	for (size_t i = 1; i < size; i++) {
		if (i == 0)
			i++;
		if (arr[i] < arr[i - 1]) {
			temp = arr[i];
			arr[i] = arr[i - 1];
			arr[i - 1] = temp;
			i -= 2;
		}
	}
}


#endif // !FUNCTIONS_H_
