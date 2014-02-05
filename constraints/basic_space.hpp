/*
* Copyright (C) 2012-2014 - Adam Streck
*/
#pragma once

template<typename ValType>
class BasicSpace {
public:
	virtual void boundSpecie(const size_t specie_no, const int max_val) = 0;

	virtual vector<ValType> getSolution() const = 0;

	virtual void print() const = 0;
};