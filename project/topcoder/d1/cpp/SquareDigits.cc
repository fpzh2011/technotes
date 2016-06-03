/* 163.html */

#include <set>
#include <limits>
#include "SquareDigits.h"

unsigned SquareDigits::smallestResult (unsigned n) {
	auto maxv = std::numeric_limits<unsigned>::max();
	for(std::size_t i = 0; i < maxv; ++i) {
		if(contain(n, i)) {
			return i;
		}
	}
	return maxv;
}

unsigned SquareDigits::sod (unsigned i) {
	unsigned sum = 0, d;
	while(i != 0) {
		d = i % 10;
		sum += d*d;
		i /= 10;
	}
	return sum;
}

bool SquareDigits::contain (unsigned n, unsigned s) {
	std::set<unsigned> rs;
	s = sod(s);
	while(s != n && rs.find(s) == rs.end()) {
		rs.insert(s);
		s = sod(s);
	}
	return s == n;
}
