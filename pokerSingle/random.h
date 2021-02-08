
#ifndef RANDOM_H_
#define RANDOM_H_

#include <ctime>
#include <iostream>
#include <vector>
#include <memory>


/**
 * Singleton class random, as only one instance is needed.
 */
class random {
	random();
	~random();

	inline static random &get() {
		static random instance;
		return instance;
	}

	int iRandInt(int from, int to);

public:
	random(const random &) = delete;
	random &operator=(const random &) = delete;

	/**
	 * This function draws a random number with the chosen range.
	 *
	 * @param from: the lowest number this function can draw (including the number itself). 
	 * @param to: the highest number this function can draw (including the number itself).
	 * @return a number within the given range.
	 */
	inline static int randInt(int from, int to) { return get().iRandInt(from, to); }
	/**
	 * This function returns true 50% of the time.
	 *
	 * @return true or false with an equal chance.
	 */
	inline bool randBool() { return (bool)get().iRandInt(0, 1); }
	/**
	 * This function returns true depending on the inputed parameter, the bigger it is the
	 * more likely this function is to return true.
	 *
	 * @param percentage: the likelihood of this function returning true (range: 0 - 100).
	 * @return true percentage / 100, false otherwise.
	 */
	inline static bool chance(int percentage) { return get().iRandInt(0, 99) + 1 <= percentage; }

	template <typename T> static void randomize(T *arr, int size) {
		T temp;
		int randomNum;

		for (int i = size - 1; i > 0; i--) {
			randomNum = get().iRandInt(0, i);
			temp = arr[i - 1];
			arr[i - 1] = arr[randomNum];
			arr[randomNum] = temp;
		}
	}
};

/*
	template <typename T>
	extern void randomize(std::vector<T> vec);

	// doesn't work..
	template<typename T>
	void randomize(std::vector<std::unique_ptr<T>> vec);

	template<typename T>
	void randomize(std::vector<T> vec) {
		T temp;
		int randomNum;
		for (int i = sivec.size() - 1; i > 0; i--) {
			randomNum = random::randInt(i);
			temp = vec[i - 1];
			vec[i - 1] = vec[randomNum];
			vec[randomNum] = temp;
		}
	}

	// doesn't work..
	template<typename T>
	void randomize(std::vector<std::unique_ptr<T>> vec) {
		std::unique_ptr<T> temp;
		int randomNum;
		for (int i = vec.size() - 1; i > 0; i--) {
			randomNum = random::randInt(i);
			temp = std::move(vec[i - 1]);
			vec[i - 1] = std::move(vec[randomNum]);
			vec[randomNum] = std::move(temp);
		}
	}
	*/
#endif // !RANDOM_H_
