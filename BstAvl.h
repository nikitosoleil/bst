#pragma once

#include <vector>
#include "Bst.h"

using namespace std;

namespace BST
{
template < class Target >
class AVL : public Base < Target >
{
private:
	struct Node
	{
		Target Value;
		int Height;
		Node *L = nullptr, *R = nullptr;
		int LHeight()
		{
			return ((L == nullptr) ? 0 : L->Height);
		}
		int RHeight()
		{
			return ((R == nullptr) ? 0 : R->Height);
		}
		int Balance()
		{
			return this->LHeight() - this->RHeight();
		}
		void UpdateHeight()
		{
			Height = max(this->LHeight(), this->RHeight()) + 1;
		}
	} *Root;

	void Clear(Node *&N)
	{
		if (N == nullptr)
			return;
		Clear(N->L);
		Clear(N->R);
		delete N;
	}
	void SmallLeft(Node *&N)
	{
		Node *B = N->R;
		N->R = B->L;
		B->L = N;
		N->UpdateHeight();
		B->UpdateHeight();
		N = B;
	}
	void SmallRight(Node *&N)
	{
		Node *B = N->L;
		N->L = B->R;
		B->R = N;
		N->UpdateHeight();
		B->UpdateHeight();
		N = B;
	}
	void BigLeft(Node *&N)
	{
		SmallRight(N->R);
		SmallLeft(N);
	}
	void BigRight(Node *&N)
	{
		SmallLeft(N->L);
		SmallRight(N);
	}
	void Balance(Node *&N)
	{
		N->UpdateHeight();
		if (N->Balance() == 2)
		{
			if (N->L->Balance() == -1)
				BigRight(N);
			else
				SmallRight(N);
		}
		else if (N->Balance() == -2)
		{
			if (N->R->Balance() == 1)
				BigLeft(N);
			else
				SmallLeft(N);
		}
	}
	bool Insert(Node *&N, Target &T)
	{
		if (N == nullptr)
		{
			N = new Node();
			N->Value = T;
			N->Height = 1;
			return true;
		}
		else if (N->Value != T)
		{
			bool Tmp;
			if (T > N->Value)
				Tmp = Insert(N->R, T);
			else
				Tmp = Insert(N->L, T);
			Balance(N);
			return Tmp;
		}
		else
			return false;
	}
	int Size(Node *&N)
	{
		if (N == nullptr)
			return 0;
		return Size(N->L) + Size(N->R) + 1;
	}
	Target EraseTwo(Node *&N)
	{
		if (N->L == nullptr)
		{
			if (N->R == nullptr)
			{
				Target Tmp = N->Value;
				delete N;
				N = nullptr;
				return Tmp;
			}
			else
			{
				Node *TmpN = N;
				N = N->R;
				Target TmpValue = TmpN->Value;
				delete TmpN;
				return TmpValue;
			}
		}
		else
		{
			Target Tmp = EraseTwo(N->L);
			Balance(N);
			return Tmp;
		}
	}
	bool EraseOne(Node *&N, Target &Object)
	{
		if (N == nullptr)
			return false;
		else if (N->Value == Object)
		{
			if (N->R == nullptr)
			{
				if (N->L == nullptr)
				{
					delete N;
					N = nullptr;
				}
				else
				{
					Node *Tmp = N;
					N = N->L;
					delete Tmp;
				}
			}
			else
			{
				N->Value = EraseTwo(N->R);
				Balance(N);
			}
			return true;
		}
		else if (Object > N->Value)
		{
			if (EraseOne(N->R, Object))
			{
				Balance(N);
				return true;
			}
			return false;
		}
		else
		{
			if (EraseOne(N->L, Object))
			{
				Balance(N);
				return true;
			}
			return false;
		}
	}
public:
	AVL() : Root(nullptr) {}
	~AVL()
	{
		Clear();
	}
	void clear()
	{
		Clear(Root);
	}
	int size()
	{
		return Size(Root);
	}
	void insert(Target &Object)
	{
		Insert(Root, Object);
	}
	bool erase(Target &Object)
	{
		return EraseOne(Root, Object);
	}
	bool find(Target &Object, Target &Result)
	{
		Node *N = Root;
		while (true)
		{
			if (N == nullptr)
				return false;
			else if (N->Value == Object)
			{
				Result = N->Value;
				return true;
			}
			else if (Object > N->Value)
				N = N->R;
			else
				N = N->L;
		}
	}
};
}