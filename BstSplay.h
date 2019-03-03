#pragma once

#include <utility>
#include "Bst.h"

#include <iostream>

using namespace std;

namespace BST
{
template < class T >
class Splay : public Base < T >
{
private:
	struct Node
	{
		T v;
		Node *l = 0, *r = 0, *p = 0;
		Node(T v) : v(v) {}
		void set_l(Node *n)
		{
			l = n;
			if (n)
				n->p = this;
		}
		void set_r(Node *n)
		{
			r = n;
			if (n)
				n->p = this;
		}
	} *root = 0;
	static void lr(Node *n) // left rotation
	{
		Node *r = n->r, *p = n->p;
		n->set_r(n->r->l);
		r->set_l(n);
		if (p)
		{
			if (p->l == n)
				p->set_l(r);
			else
				p->set_r(r);
		}
		else
			r->p = 0;
	}
	static void rr(Node *n) // right rotation
	{
		Node *l = n->l, *p = n->p;
		n->set_l(n->l->r);
		l->set_r(n);
		if (p)
		{
			if (p->l == n)
				p->set_l(l);
			else
				p->set_r(l);
		}
		else
			l->p = 0;
	}
	static void splay(Node *n)
	{
		while (n->p)
		{
			if (n->p->l == n)
			{
				if (n->p->p)
				{
					if (n->p->p->l == n->p)
					{
						rr(n->p->p);
						rr(n->p);
					}
					else
					{
						rr(n->p);
						lr(n->p);
					}
				}
				else
					rr(n->p);
			}
			else
			{
				if (n->p->p)
				{
					if (n->p->p->r == n->p)
					{
						lr(n->p->p);
						lr(n->p);
					}
					else
					{
						lr(n->p);
						rr(n->p);
					}
				}
				else
					lr(n->p);
			}
		}
	}
	static Node *merge(Node *u, Node *v)
	{
		if (!u)
			return v;
		if (!v)
			return u;
		Node *t = u;
		while (t->r)
			t = t->r;
		splay(t);
		t->set_r(v);
		return t;
	}
	static pair < Node *, Node * > split(Node *n, T &value)
	{
		Node *t = n, *f = 0;
		while (t)
		{
			if (t->v <= value)
				f = t;
			if (t->v > value)
				t = t->l;
			else
				t = t->r;
		}
		pair < Node *, Node * > ret;
		if (f)
		{
			splay(f);
			if (f->r)
				f->r->p = 0;
			ret = {f, f->r};
			f->r = 0;
		}
		else
			ret = {0, n};
		return ret;
	}
	void clear(Node *v)
	{
		if (v)
		{
			clear(v->l);
			clear(v->r);
			delete v;
		}
	}
	int size(Node *v)
	{
		if (v)
			return size(v->l) + size(v->r) + 1;
		else
			return 0;
	}
public:
	Splay() {}
	~Splay() {}
	void insert(T &value)
	{
		pair < Node *, Node * > ret = split(root, value);
		root = new Node(value);
		if (ret.first && ret.first->v == value)
			root = merge(ret.first, ret.second);
		else
		{
			root->set_l(ret.first);
			root->set_r(ret.second);
		}
	}
	bool erase(T &value)
	{
		Node *n = root;
		while (true)
		{
			if (!n)
				return false;
			else if (n->v == value)
			{
				splay(n);
				if (n->l)
					n->l->p = 0;
				if (n->r)
					n->r->p = 0;
				root = merge(n->l, n->r);
				delete n;
				return true;
			}
			else if (value < n->v)
				n = n->l;
			else
				n = n->r;
		}
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
				splay(n);
				root = n;
				return true;
			}
			else if (value < n->v)
				n = n->l;
			else
				n = n->r;
		}
	}
};
}