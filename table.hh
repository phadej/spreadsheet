/** \file Helper table. */
#ifndef TABLE_HH
#define TABLE_HH

#include <map>

#include "cellindex.hh"
#include "definitions.hh"

/** Helper table class. */
template <typename T>
class table {
public:
	typedef typename std::map<cellindex, T>::const_iterator const_iterator;

	const_iterator begin() const { return data.begin(); }
	const_iterator end() const { return data.end(); }

	void set(const cellindex &i, const T &t) {
		data.erase(i);
		data.insert(std::make_pair(i, t));
	}

	void set(unsigned int col, unsigned int row, const T &t) {
		set(cellindex(col, row), t);
	}

	const_iterator find(const cellindex &index) const {
		return data.find(index);
	}

	void erase(const cellindex &i) {
		data.erase(i);
	}

private:
	std::map<cellindex, T> data;
};

#endif
