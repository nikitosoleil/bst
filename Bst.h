#pragma once

namespace BST
{
template < class T >
class Base
{
public:
	virtual void clear() = 0;
	virtual int size() = 0;
	virtual void insert(T &obj) = 0;
	virtual bool erase(T &obj) = 0;
	virtual bool find(T &obj, T &res) = 0;
};
}
