#pragma once

#include <vector>
#include "Bst.h"

using namespace std;

namespace BST
{
	/// Not actually a binary tree, but who cares
	template < class Target >
	class TwoThree : public Base < Target >
	{
	protected:
		struct Node
		{
			static const int MAX_DEGREE = 5;
			int Degree;
			Target Value, *Max;
			Node **To, *P;
			Node (Target Input = -Target::INF)
			{
				Value = Input;
				P = 0;
				Max = new Target[MAX_DEGREE];
				To = new Node *[MAX_DEGREE];
				Degree = 0;
				for (int i = 0; i < MAX_DEGREE; ++i)
				{
					Max[i] = Target(Target::INF, 0);
					To[i] = 0;
				}
			}
			void Insert (Node *V)
			{
				V->P = this;
				int Pos = Degree;
				while (Pos && Max[Pos-1] > V->Value)
				{
					Max[Pos] = Max[Pos-1];
					To[Pos] = To[Pos-1];
					--Pos;
				}
				Max[Pos] = V->Value;
				To[Pos] = V;
				++Degree;
				Single_Update();
			}
			void Erase (Node *V)
			{
				for (int i = 0; i < Degree; ++i)
				{
					if (To[i] == V)
					{
						for (int j = i; j < Degree; ++j)
						{
							Max[j] = Max[j+1];
							To[j] = To[j+1];
						}
						--Degree;
						break;
					}
				}
				Single_Update();
			}
			void Single_Update ()
			{
				Value = -Target::INF;
				for (int i = 0; i < Degree; ++i)
				{
					Max[i] = To[i]->Value;
					Value = max(Value, Max[i]);
				}
			}
			void Update ()
			{
				Single_Update();
				if (P)
					P->Update();
			}
			void Split (Node *&Root)
			{
				if (Degree == 4)
				{
					Node *New = new Node();
					New->Insert(To[3]);
					Erase(To[3]);
					New->Insert(To[2]);
					Erase(To[2]);
					if (P)
					{
						P->Insert(New);
						P->Split(Root);
					}
					else
					{
						Root = new Node();
						Root->Insert(this);
						Root->Insert(New);
					}
				}
			}
			~Node ()
			{
				delete Max;
				delete To;
			}
		} *Root;
		
		void Clear (Node *V)
		{
			if (V)
			{
				for (int i = 0; i < V->Degree; ++i)
					Clear(V->To[i]);
				delete V;
			}
		}
		int Size (Node *V)
		{
			int Ret = 0;
			if (V)
			{
				for (int i = 0; i < V->Degree; ++i)
				{
					auto Tmp = V->To[i];
					Ret += Size(V->To[i]);
				}
				if (Ret == 0)
					++Ret;
			}
			return Ret;
		}
		Node *Find (Target &Object)
		{
			Node *V = Root;
			while (V && V->Degree)
			{
				if (V->Degree == 3 && Object > V->Max[1])
					V = V->To[2];
				else if (Object > V->Max[0])
					V = V->To[1];
				else
					V = V->To[0];
			}
			return V;
		}
	public:
		TwoThree ()
		{
			Root = 0;
		}
		~TwoThree ()
		{
			Clear();
		}
		void insert(Target &Object)
		{
			Node *New = new Node(Object);
			if (Root)
			{
				Node *V = Find(Object);
				if (V->Value != Object)
				{
					if (V->P)
					{
						V->P->Insert(New);
						V->P->Update();
						V->P->Split(Root);
					}
					else
					{
						Root = new Node();
						Root->Insert(V);
						Root->Insert(New);
					}
					V->P->Update();
				}
			}
			else
				Root = New;
		}
		void Erase (Node *V)
		{
			if (V->P)
			{
				if (V->P->Degree > 2)
				{
					V->P->Erase(V);
					V->P->Update();
					delete V;
				}
				else
				{
					Node *U;
					for (int i = 0; i < V->P->Degree; ++i)
						if (V->P->To[i] == V)
						{
							if (i > 0)
								U = V->P->To[i-1];
							else
								U = V->P->To[i+1];
						}
					if (!V->P->P)
					{
						delete V->P;
						delete V;
						Root = U;
						U->P = 0;
					}
					else
					{
						Target Tmp = V->P->Value;
						Node *NP;
						for (int i = 0; i < V->P->P->Degree; ++i)
							if (V->P->P->To[i] == V->P)
							{
								if (i > 0)
									NP = V->P->P->To[i-1];
								else
									NP = V->P->P->To[i+1];
							}
						V->P->Erase(V);
						NP->Insert(U);
						V->P->Erase(U);
						V->P->Value = Tmp;
						NP->Split(Root);
						NP->Update();
						Erase(V->P);
						delete V;
					}
				}
			}
			else
			{
				delete V;
				Root = 0;
			}
		}
		bool erase(Target &Object)
		{
			Node *V = Find(Object);
			if (V && V->Value == Object)
			{
				Erase(V);
				return true;
			}
			else
				return false;
		}
		void clear()
		{
			Clear(Root);
		}
		int size()
		{
			return Size(Root);
		}
		bool find(Target &Object, Target &Result)
		{
			Node *V = Find(Object);
			if (V && V->Value == Object)
			{
				Result = V->Value;
				return true;
			}
			else
				return false;
		}
	};
}