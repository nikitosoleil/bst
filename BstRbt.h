#pragma once

#include "Bst.h"

using namespace std;

namespace BST
{
template < class T >
class RBT : public Base < T >
{
private:
	enum Color { red, black };
	struct Node
	{
		T v;
		Color c;
		Node *l = nullptr, *r = nullptr;
		Node(T v, Color c) : v(v), c(c) {}
	} *root = nullptr;
	void lr(Node *&n) // left rotation
	{
		Node *t = n;
		n = n->r;
		t->r = n->l;
		n->l = t;
	}
	void rr(Node *&n) // right rotation
	{
		Node *t = n;
		n = n->l;
		t->l = n->r;
		n->r = t;
	}
	Color c(Node *&n)
	{
		return n ? n->c : black;
	}
	void insert(Node *&n, Node *p, Node *b, T v)
	{
		if (!n)
			n = new Node(v, red);
		else if (v < n->v)
		{
			insert(n->l, n, n->r, v);
			if (n->c == red && n->l->c == red)
			{
				if (c(b) == red)
				{
					n->c = black;
					b->c = black;
					p->c = red;
				}
				else
				{
					if (p && p->r == n)
						rr(n);
				}
			}
			if (c(n->l) == red && c(n->l->l) == red)
			{
				rr(n);
				n->c = black;
				n->r->c = red;
			}
		}
		else if (v > n->v)
		{
			insert(n->r, n, n->l, v);
			if (n->c == red && n->r->c == red)
			{
				if (c(b) == red)
				{
					n->c = black;
					b->c = black;
					p->c = red;
				}
				else
				{
					if (p && p->l == n)
						lr(n);
				}
			}
			if (c(n->r) == red && c(n->r->r) == red)
			{
				lr(n);
				n->c = black;
				n->l->c = red;
			}

		}
	}
	void clear(Node *n)
	{
		if (n)
		{
			clear(n->l);
			clear(n->r);
			delete n;
		}
	}
	int size(Node *n)
	{
		if (n)
			return size(n->l) + size(n->r) + 1;
		else
			return 0;
	}
public:
	RBT() {}
	~RBT()
	{
		clear();
	}
	void insert(T &value)
	{
		insert(root, nullptr, nullptr, value);
	}
	bool erase(T &value)
	{
		// TODO
	}
	void clear()
	{
		clear(root);
	}
	int size()
	{
		return size(root);
	}
	bool find(T &value, T &result)
	{
		Node *n = root;
		while (true)
		{
			if (!n)
				return false;
			else if (n->v == value)
			{
				result = n->v;
				return true;
			}
			else if (value <= n->v)
				n = n->l;
			else
				n = n->r;
		}
	}
};
}