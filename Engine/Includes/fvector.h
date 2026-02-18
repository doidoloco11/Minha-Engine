#pragma once

#include <vector>
#include <stack>
#include <algorithm>

template<typename T>
class fvector {
public:
	std::vector<T> data;
	T& operator[](size_t index) { return data[index]; }
	size_t size() const { return data.size(); }
	void push_back(const T& value) { data.push_back(value); }
	void pop_back() { data.pop_back(); }
	void clear() { data.clear(); }
	void remove(const T& value) {
		removingElements.push(value);
	}
	void processRemovals() {
		while (!removingElements.empty()) {
			T value = removingElements.top();
			removingElements.pop();
			data.erase(std::remove(data.begin(), data.end(), value), data.end());
		}
	}
	T& back() { return data.back(); }
private:
	std::stack<T> removingElements;
};