
#include "pch.h"
#include "Functions.h"

void func::threeDigitsToOutput(std::ostream &output, chips_t digits) {
	if (digits > 99) {
		output << digits;
		return;
	}

	int zeros;
	if (digits == 0)
		zeros = 3;
	else if (digits < 10)
		zeros = 2;
	else // digits < 100
		zeros = 1;

	for (int i = 0; i < zeros; i++)
		output << '0';
	if (zeros != 3)
		output << digits;
}
std::ostream &func::commas(std::ostream &output, chips_t number) {
	if (number <= 999)
		return output << number;

	if (number > 1000000000)
		int x = 1;

	unsigned long long ceiling = 1000;
	int count = 0;
	while (number >= ceiling) {
		ceiling *= 1000;
		count++;
	}
	count++;
	ceiling /= 1000;

	for (int i = 0; i < count; i++) {

		if (i == 0) // first should be outputed without zeros before it
			output << number / ceiling;
		else
			threeDigitsToOutput(output, (chips_t) ((unsigned long long) number / ceiling));

		if (i != count - 1) // last shouldnt have a comma after it
			output << ',';

		number %= ceiling;
		ceiling /= 1000;
	}

	return output;
}
