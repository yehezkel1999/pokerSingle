
#include "Functions.h"

std::ostream &commas(std::ostream &output, const unsigned int number) {
	if (number == 0)
		return output << number;

	int numbers[5], count = 0, size = 5;
	for (int i = 0; i < size; i++)
		numbers[i] = 0;

	unsigned int num = number;
	while (num != 0) {
		numbers[count] = num % 1000;
		num /= 1000;
		count++;
	}

	char str[3];
	for (int i = size - 1; i >= 0; i--) {
		for (int j = 0; j < 3; j++)
			str[i] = (char)((int)'0' + ((numbers[i] / (int)pow(10, 2 - j)) % 10));
		if (i == 0 || (i != 0 && numbers[i - 1] == 0))
			output << str;
		else
			output << str << ',';
	}
	return output;
}
std::ostream &commas(std::ostream &output, const int number) {
	if (number == 0)
		return output << number;

	int numbers[5], count = 0, size = 5;
	for (int i = 0; i < size; i++)
		numbers[i] = 0;

	unsigned int num = number;
	while (num != 0) {
		numbers[count] = num % 1000;
		num /= 1000;
		count++;
	}

	char str[3];
	for (int i = size - 1; i >= 0; i--) {
		for (int j = 0; j < 3; j++)
			str[i] = (char)((int)'0' + ((numbers[i] / (int)pow(10, 2 - j)) % 10));
		if (i == 0 || (i != 0 && numbers[i - 1] == 0))
			output << str;
		else
			output << str << ',';
	}
	return output;
}
