#include <map>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <assert.h>
#include <sstream>

template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K, V> m_map;

public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
		: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign(K const& keyBegin, K const& keyEnd, V const& val) {
		// INSERT YOUR SOLUTION HERE
		if (!(keyBegin < keyEnd)) {
			return;
		}
		auto start_range = m_map.insert_or_assign(keyBegin, val);

		auto end_range = m_map.lower_bound(keyEnd);
		if (end_range == m_map.end()) {
			// Do nothing
		}
		else if (!(end_range->first < keyEnd) && !(keyEnd < end_range->first)) {
			// keyBegin ... [keyEnd]==end_range ... end
		}
		else {
			auto copy_end_range = end_range;
			std::advance(copy_end_range, -1);
			if (!(copy_end_range->first < keyBegin) && !(keyBegin < copy_end_range->first)) {
				// keyBegin [keyEnd] end_range end
				end_range = m_map.insert({ keyEnd, end_range->second }).first;
				m_map.erase(std::next(end_range));
			}
			else {
				// keyBegin value [keyEnd] end_range ... end
				end_range = m_map.insert({ keyEnd, copy_end_range->second }).first;
			}
		}
		m_map.erase(std::next(start_range.first), end_range);
		
		// Remove begin element that has the same value as m_valBegin
		for (auto it = m_map.begin(); it != m_map.end();) {
			if (it->second == m_valBegin) {
				it = m_map.erase(it);
			}
			else {
				break;
			}
		}
		if (m_map.size() >= 2) {
			auto before_end = std::prev(m_map.end());
			auto before_before_end = std::prev(before_end);
			if (before_before_end->second == before_end->second) {
				m_map.erase(before_end);
			}
		}

	}

	// look-up of the value associated with key
	V const& operator[](K const& key) const {
		auto it = m_map.upper_bound(key);
		if (it == m_map.begin()) {
			return m_valBegin;
		}
		else {
			return (--it)->second;
		}
	}

	void clear() {
		m_map.clear();
	}

	std::string to_str() {
		std::stringstream ss;
		for (const auto& kv : m_map) {
			ss << kv.first << "->" << kv.second << "\n";
		}
		return ss.str();
	}
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.

void IntervalMapTest()
{
	std::stringstream ss;
	interval_map<int, char> imap('A');
	imap.clear();
	imap.assign(5, 5, 'C');
	assert(imap.to_str() == "");

	imap.clear();
	imap.assign(5, 6, 'A');
	assert(imap.to_str() == "");

	imap.clear();
	imap.assign(5, 6, 'C');
	assert(imap.to_str() == "5->C\n");

	imap.clear();
	imap.assign(5, 7, 'A');
	assert(imap.to_str() == "");

	imap.clear();
	imap.assign(5, 9, 'C');
	assert(imap.to_str() == "5->C\n");

	imap.clear();
	imap.assign(5, 7, 'C');
	imap.assign(4, 6, 'A');
	assert(imap.to_str() == "");

	imap.clear();
	imap.assign(5, 7, 'C');
	imap.assign(4, 6, 'B');
	assert(imap.to_str() == "4->B\n");

	imap.clear();
	imap.assign(5, 7, 'C');
	imap.assign(1, 4, 'B');
	assert(imap.to_str() == "1->B\n4->C\n");

	imap.clear();
	imap.assign(5, 7, 'C');
	imap.assign(1, 1, 'B');
	assert(imap.to_str() == "5->C\n");

	imap.clear();
	imap.assign(5, 7, 'C');
	imap.assign(1, 2, 'B');
	assert(imap.to_str() == "1->B\n2->C\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(6, 6, 'B');
	assert(imap.to_str() == "5->C\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(6, 7, 'B');
	assert(imap.to_str() == "5->C\n6->B\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(5, 7, 'B');
	assert(imap.to_str() == "5->B\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	assert(imap.to_str() == "5->C\n10->B\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(6, 7, 'D');
	assert(imap.to_str() == "5->C\n6->D\n7->B\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(6, 15, 'D');
	assert(imap.to_str() == "5->C\n6->D\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(6, 10, 'D');
	assert(imap.to_str() == "5->C\n6->D\n10->B\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(6, 11, 'D');
	assert(imap.to_str() == "5->C\n6->D\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(5, 10, 'D');
	assert(imap.to_str() == "5->D\n10->B\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(5, 11, 'D');
	assert(imap.to_str() == "5->D\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(10, 11, 'D');
	assert(imap.to_str() == "5->C\n10->D\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(11, 11, 'D');
	assert(imap.to_str() == "5->C\n10->B\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(11, 15, 'B');
	assert(imap.to_str() == "5->C\n10->B\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(11, 15, 'D');
	assert(imap.to_str() == "5->C\n10->B\n11->D\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(4, 5, 'D');
	assert(imap.to_str() == "4->D\n5->C\n10->B\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(1, 4, 'D');
	assert(imap.to_str() == "1->D\n4->C\n10->B\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(15, 20, 'D');
	imap.assign(4, 20, 'E');
	assert(imap.to_str() == "4->E\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(15, 20, 'D');
	imap.assign(4, 15, 'E');
	assert(imap.to_str() == "4->E\n15->D\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(15, 20, 'D');
	imap.assign(4, 16, 'E');
	assert(imap.to_str() == "4->E\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(15, 20, 'D');
	imap.assign(4, 10, 'E');
	assert(imap.to_str() == "4->E\n10->B\n15->D\n");

	imap.clear();
	imap.assign(5, 10, 'C');
	imap.assign(10, 15, 'B');
	imap.assign(15, 20, 'D');
	imap.assign(4, 11, 'E');
	assert(imap.to_str() == "4->E\n11->B\n15->D\n");
}
int main() {
	IntervalMapTest();
	return 0;
}


