
#include "CustomExceptions.h"


invalid_card_value::invalid_card_value(const char *message) :out_of_range(message) {
}
invalid_card_value::~invalid_card_value() {}

nullptr_exception::nullptr_exception(const char *message) :invalid_argument(message) {
}
nullptr_exception::~nullptr_exception() {}
