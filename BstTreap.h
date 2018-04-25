#pragma once

#include <vector>
#include <utility>

using namespace std;

#define max(a, b) ((a>b)?a:b)

namespace BST
{
	template < class Target >
	class Treap
	{
	private:
		struct Node
		{
			Target X;
			int Y;
			Node *L, *R;
			Node (Target A = Target(), int B = 0): X(A), Y(B), L(0), R(0) {}
		} *Root = 0;
		
		pair < Node *, Node * > Split (Node *V, Target Key)
		{
			if (!V)
				return {0, 0};
			else if (Key > V->X)
			{
				auto Ret = Split(V->R, Key);
				V->R = Ret.first;
				return {V, Ret.second};
			}
			else
			{
				auto Ret = Split(V->L, Key);
				V->L = Ret.second;
				return {Ret.first, V};
			}
		}
		Node *Merge (Node *V, Node *U)
		{
			if ((!V) || (!U))
				return max(U, V);
			if (V->Y > U->Y)
			{
				Node *Tmp = Merge(V->R, U);
				V->R = Tmp;
				return V;
			}
			else
			{
				Node *Tmp = Merge(V, U->L);
				U->L = Tmp;
				return U;
			}
		}
		void Clear (Node *V)
		{
			if (V)
			{
				Clear(V->L);
				Clear(V->R);
				delete V;
			}
		}
		int Size (Node *V)
		{
			if (V)
				return Size(V->L)+Size(V->R)+1;
			else
				return 0;
		}
	public:
		Treap (): Root(0) {}
		~Treap ()
		{
			Clear();
		}
		void Clear ()
		{
			Clear(Root);
		}
		int Size ()
		{
			return Size(Root);
		}
		void Insert (Target X, int Y)
		{
			Target Tmp;
			if (Find(X, Tmp)) return;
			auto One = Split(Root, X);
			auto Now = new Node(X, Y);
			auto Two = Merge(One.first, Now);
			Root = Merge(Two, One.second);
		}
		void Erase (Target X)
		{
			auto One = Split(Root, X+1);
			auto Two = Split(One.first, X);
			delete Two.second;
			Root = Merge(Two.first, One.second);
		}
		bool Find (Target &Object, Target &Result)
		{
			Node *V = Root;
			while (true)
			{
				if (!V)
					return false;
				else if (V->X == Object)
				{
					Result = V->X;
					return true;
				}
				else if (Object > V->X)
					V = V->R;
				else
					V = V->L;
			}
		}
	};
}
