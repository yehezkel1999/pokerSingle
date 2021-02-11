
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "../Flags.h"
#include <iostream>

/**
 * Static class func.
 *
 * This Class only contains miscellaneous functions that do various actions and calculations.
 * No instances of this class can be created.
 */
class func {
private:

	static void threeDigitsToOutput(std::ostream &output, chips_t digits);

public:
	func() = delete;

	/**
	 * UNTESTED
	 *
	 * Function commas, takes a chips type and outputs it with a comma every three digits
	 * to the given output stream (###,###,###).
	 *
	 * @param output: the output stream the number is outputted to.
	 * @param number: the number that gets outputted with commas.
	 * @return the output stream that is recieved by the function.
	 */
	static std::ostream &commas(std::ostream &output, chips_t chips);

	template<typename Base, typename T>
	inline static bool instanceof(const T *) { return std::is_base_of<Base, T>::value; }

	/**
	 * Template function for an array, checks if the given array is sorted from low to high.
	 * Uses item's operator<.
	 *
	 * @param arr: a pointer to the array.
	 * @param size: the size of the array.
	 * @return true if array is sorted, false if it isnt.
	 */
	template <typename T>
	static bool isSorted(const T *arr, size_t size);

	/**
	 * Template function for an array, sorts the given array from low to high in a time
	 * complexety of (n^2+n)/2. Uses item's default constructor, assignment operator and
	 * operator<.
	 *
	 * @param arr: a pointer to the array.
	 * @param size: the size of the array.
	 */
	template <typename T>
	static void sort(T *arr, size_t size);
};

template <typename T>
bool func::isSorted(const T *arr, size_t size) {
	for (size_t i = 1; i < size; i++)
		if (arr[i] < arr[i - 1])
			return false;
	return true;
}
template <typename T>
void func::sort(T *arr, size_t size) {
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
