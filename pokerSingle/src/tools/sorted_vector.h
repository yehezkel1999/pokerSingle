
#ifndef SORTED_VECTOR_H_
#define SORTED_VECTOR_H_

#include <vector>
#include <iostream>
#include <stdexcept>

// if there's an array implementation then insertion is already costly and needs to move
// elements to the right regardless
#define VECTOR_ARRAY_IMPLEMENTATION 1


template <typename T>
class sorted_vector {
public:
	using value_type = T;
	using vec = std::vector<value_type>;
	using size_type = typename vec::size_type;
	using iterator = typename vec::iterator;
	using const_iterator = typename vec::const_iterator;
	using reverse_iterator = typename vec::reverse_iterator;
	using const_reverse_iterator = typename vec::const_reverse_iterator;

private:
	vec m_data;

	size_type find(const value_type &value);
	size_type move_right_until(const value_type &value);
	iterator array_insert(const value_type &value);
	iterator array_insert(value_type &&value);

public:
	sorted_vector();
	sorted_vector(size_type amount);
	sorted_vector(const sorted_vector &other);
	sorted_vector(sorted_vector &&other);

	size_type capacity() const { return m_data.capacity(); }
	size_type size() const { return m_data.size(); }
	void reserve(size_type amount) { m_data.reserve(amount); }
	void shrink_to_size() { m_data.shrink_to_fit(); }
	void clear() { m_data.clear(); }
	void pop_back() const { m_data.pop_back(); }
	iterator erase(const_iterator position) { return m_data.erase(position); }
	iterator erase(const_iterator first, const_iterator last) 
	{ return m_data.erase(first, last); }
	value_type &front() { return m_data.front(); }
	const value_type &front() const { return m_data.front(); }
	value_type &back() { return m_data.back(); }
	const value_type &back() const { return m_data.back(); }

	iterator push(const value_type &value);
	iterator push(value_type &&value);
	template <typename... Args>
	void push(const value_type &value, Args&... args);
	template <typename... Args>
	void push(value_type &&value, Args&&... args);

	//template <typename... Args>
	//value_type &emplace(Args&&... args);

	value_type &operator[](size_type place) { return m_data[place]; }
	const value_type &operator[](size_type place) const { return m_data[place]; }

	sorted_vector &operator=(const sorted_vector &other);
	sorted_vector &operator=(sorted_vector &&other);

	// iterators:

	iterator begin() { return m_data.begin(); }
	iterator end() { return m_data.end(); }
	const_iterator begin() const { return m_data.cbegin(); }
	const_iterator end() const { return m_data.cend(); }
	const_iterator cbegin() const { return m_data.cbegin(); }
	const_iterator cend() const { return m_data.cend(); }
	reverse_iterator rbegin() { return m_data.rbegin(); }
	reverse_iterator rend() { return m_data.rend(); }
	const_reverse_iterator rbegin() const { return m_data.crbegin(); }
	const_reverse_iterator rend() const { return m_data.crend(); }
	const_reverse_iterator crbegin() const { return m_data.cbegin(); }
	const_reverse_iterator crend() const { return m_data.cend(); }

	template <typename U>
	friend std::ostream &operator<<(std::ostream &output, const sorted_vector &source);

	~sorted_vector() noexcept {}
};

template<typename T> inline
sorted_vector<T>::sorted_vector() : m_data() {}
template<typename T> inline
sorted_vector<T>::sorted_vector(size_type amount) : m_data(amount) {}
template<typename T> inline
sorted_vector<T>::sorted_vector(const sorted_vector &other) : m_data(other.m_data) {}
template<typename T> inline
sorted_vector<T>::sorted_vector(sorted_vector &&other) : m_data(std::move(other.m_data)) {}

template<typename T> inline typename
sorted_vector<T>::size_type sorted_vector<T>::move_right_until(const value_type &value) {
	size_type i = size();
	if (value < m_data[i - 1]) {
		m_data.push_back(std::move(m_data[i - 1]));
		i--;
	}
	else
		return i;

	for (; i > 0 && value < m_data[i - 1]; i--)
		m_data[i] = std::move(m_data[i - 1]);
	return i;
}
template<typename T> inline typename
sorted_vector<T>::iterator sorted_vector<T>::array_insert(const value_type &value) {
	size_type place = move_right_until(value);
	if (place == size())
		m_data.push_back(value);
	else
		m_data[place] = value;
	return begin() + place;
}
template<typename T> inline typename
sorted_vector<T>::iterator sorted_vector<T>::array_insert(value_type &&value) {
	size_type place = move_right_until(value);
	if (place == size())
		m_data.push_back(std::move(value));
	else
		m_data[place] = std::move(value);
	return begin() + place;
}
template<typename T> inline typename
sorted_vector<T>::size_type sorted_vector<T>::find(const value_type &value) {
	size_type size = m_data.size();

	size_type inc = size / 2, it = inc, jump;
	while (inc && it >= 0 && it < size) {
		if (value == m_data[it]) // found and should be placed at [it]
			break;

		jump = (size_t) (inc / 2.0 + 0.5);
		if (value > m_data[it]) { // move right --->
			if (it < size - 1 && value <= m_data[it + 1]) // found and should be placed at [it]
				break;
			it += inc == 1 ? 1 : jump;
		}
		else { // value < m_data[it], move left <---
			if (it > 0 && value >= m_data[it - 1]) // found and should be placed at [it]
				break;
			it -= inc == 1 ? 1 : jump;
		}
		inc /= 2;
	}

	if (it > size)
		it = size;

	if (!inc) // if not found the it has to be in one of the edges
		if (it > 1)
			it = size;
		else
			it = 0;

	if (it < size && value > m_data[it])
		it++;

	return it;
}
template<typename T> inline typename
sorted_vector<T>::iterator sorted_vector<T>::push(const value_type &value) {
	if (!size()) {
		m_data.push_back(value);
		return begin();
}
#if VECTOR_ARRAY_IMPLEMENTATION
	return array_insert(value);
#else
	size_type place = find(value);

	if (place == size())
		m_data.push_back(value);
	else
		m_data.insert(begin() + place, value);

	return begin() + place;
#endif
}
template<typename T> inline typename
sorted_vector<T>::iterator sorted_vector<T>::push(value_type &&value) {
	if (!size()) {
		m_data.push_back(std::move(value));
		return begin();
	}
#if VECTOR_ARRAY_IMPLEMENTATION
	return array_insert(std::move(value));
#else
	size_type place = find(value);

	if (place == size())
		m_data.push_back(std::move(value));
	else
		m_data.insert(begin() + place, std::move(value));

	return begin() + place;
#endif
}

template<typename T> template<typename ...Args> inline
void sorted_vector<T>::push(const value_type &value, Args &... args) {
	push(value);
	push(args...);
}
template<typename T> template<typename ...Args> inline
void sorted_vector<T>::push(value_type &&value, Args &&... args) {
	push(std::move(value));
	push(std::forward<Args &&>(args)...);
}
/*
template<typename T> template<typename... Args> inline typename
sorted_vector<T>::value_type &sorted_vector<T>::emplace(Args &&... args) {
	find_rv find_data = find(value);

	if (!find_data.found) {
		if (find_data.place > 1) {
			m_data.emplace_back(std::forward<Args>(args)...);
			return;
		}
		find_data.place = 0;
	}

	if (value < m_data[find_data.place])
		m_data.emplace(begin() + find_data.place, std::forward<Args>(args)...);
	else
		m_data.emplace(begin() + find_data.place + 1, std::forward<Args>(args)...);
}
*/

template<typename T> inline
sorted_vector<T> &sorted_vector<T>::operator=(const sorted_vector &other) {
	if (this == &other)
		return *this;

	m_data = other.m_data;

	return *this;
}
template<typename T> inline
sorted_vector<T> &sorted_vector<T>::operator=(sorted_vector &&other) {
	if (this == &other)
		return *this;

	m_data = std::move(other.m_data);

	return *this;
}

template<typename T> inline
std::ostream &operator<<(std::ostream &output, const sorted_vector<T> &source) {
	output << "ordered vector: " << std::endl;
	const char *separator = std::is_fundamental<T>::value ? ", " : "\n";

	auto it = source.cbegin();
	for (; it + 1 != source.cend(); it++)
		output << *it << separator;

	return output << *it << std::endl;
}

#endif // !SORTED_VECTOR_H_
