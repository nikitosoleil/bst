#pragma once

#include <vector>
#include "Bst.h"

using namespace std;

namespace BST
{
template < class T, int _DEG >
class BPlus : public Base < T >
{
	static const int DEG = _DEG;
protected:
	template < int DEG >
	struct Node
	{
		int degree;
		T value, *max;
		Node **to, *p;
		Node(T input = -T::INF)
		{
			value = input;
			p = 0;
			max = new T[DEG + 2];
			to = new Node *[DEG + 2];
			degree = 0;
			for (int i = 0; i < DEG; ++i)
			{
				max[i] = T(T::INF, 0);
				to[i] = 0;
			}
		}
		void insert(Node *v)
		{
			v->p = this;
			int pos = degree;
			while (pos && max[pos - 1] > v->value)
			{
				max[pos] = max[pos - 1];
				to[pos] = to[pos - 1];
				--pos;
			}
			max[pos] = v->value;
			to[pos] = v;
			++degree;
			single_update();
		}
		void erase(Node *v)
		{
			for (int i = 0; i < degree; ++i)
			{
				if (to[i] == v)
				{
					for (int j = i; j < degree; ++j)
					{
						max[j] = max[j + 1];
						to[j] = to[j + 1];
					}
					--degree;
					break;
				}
			}
			single_update();
		}
		void single_update()
		{
			value = -T::INF;
			for (int i = 0; i < degree; ++i)
			{
				max[i] = to[i]->value;
				if (max[i] > value)
					value = max[i];
			}
		}
		void update()
		{
			single_update();
			if (p)
				p->update();
		}
		void split(Node *&v)
		{
			if (degree == DEG + 1)
			{
				Node *nw = new Node();
				for (int i = DEG; i >= DEG/2 + 1; --i)
				{
					nw->insert(to[i]);
					erase(to[i]);
				}
				if (p)
				{
					p->insert(nw);
					p->split(v);
				}
				else
				{
					v = new Node();
					v->insert(this);
					v->insert(nw);
				}
			}
		}
		~Node()
		{
			delete max;
			delete to;
		}
	};
	Node < DEG > *root;

	void clear(Node < DEG > *v)
	{
		if (v)
		{
			for (int i = 0; i < v->degree; ++i)
				clear(v->to[i]);
			delete v;
		}
	}
	int size(Node < DEG > *v)
	{
		int ret = 0;
		if (v)
		{
			for (int i = 0; i < v->degree; ++i)
				ret += size(v->to[i]);
			if (ret == 0)
				++ret;
		}
		return ret;
	}
	Node < DEG > *find(T &obj)
	{
		Node < DEG > *v = root;
		while (v && v->degree)
		{
			bool found = false;
			for (int i = v->degree - 1; i >= 1 && !found; --i)
				if (obj > v->max[i - 1])
				{
					v = v->to[i];
					found = true;
				}
			if (!found)
				v = v->to[0];
		}
		return v;
	}
public:
	BPlus()
	{
		root = 0;
	}
	~BPlus()
	{
		clear();
	}
	void insert(T &obj)
	{
		Node < DEG > *nw = new Node < DEG >(obj);
		if (root)
		{
			Node < DEG > *v = find(obj);
			if (v->value != obj)
			{
				if (v->p)
				{
					v->p->insert(nw);
					v->p->update();
					v->p->split(root);
				}
				else
				{
					root = new Node < DEG >();
					root->insert(v);
					root->insert(nw);
				}
				v->p->update();
			}
		}
		else
			root = nw;
	}
	void erase(Node < DEG > *v)
	{
		if (v->p)
		{
			if (v->p->degree > 2)
			{
				v->p->erase(v);
				v->p->update();
				delete v;
			}
			else
			{
				Node < DEG > *u;
				for (int i = 0; i < v->p->degree; ++i)
					if (v->p->to[i] == v)
					{
						if (i > 0)
							u = v->p->to[i - 1];
						else
							u = v->p->to[i + 1];
					}
				if (!v->p->p)
				{
					delete v->p;
					delete v;
					root = u;
					u->p = 0;
				}
				else
				{
					T tmp = v->p->value;
					Node < DEG > *np;
					for (int i = 0; i < v->p->p->degree; ++i)
						if (v->p->p->to[i] == v->p)
						{
							if (i > 0)
								np = v->p->p->to[i - 1];
							else
								np = v->p->p->to[i + 1];
						}
					v->p->erase(v);
					np->insert(u);
					v->p->erase(u);
					v->p->value = tmp;
					np->split(root);
					np->update();
					erase(v->p);
					delete v;
				}
			}
		}
		else
		{
			delete v;
			root = 0;
		}
	}
	bool erase(T &obj)
	{
		Node < DEG > *v = find(obj);
		if (v && v->value == obj)
		{
			erase(v);
			return true;
		}
		else
			return false;
	}
	void clear()
	{
		clear(root);
	}
	int size()
	{
		return size(root);
	}
	bool find(T &obj, T &res)
	{
		Node < DEG > *v = find(obj);
		if (v && v->value == obj)
		{
			res = v->value;
			return true;
		}
		else
			return false;
	}
};
}