#pragma once

#include <vector>
#include <iostream> /// TODO: GTFO
#include <cassert>

using namespace std;

#define max(a, b) ((a>b)?a:b)

namespace BST
{
	/// Not actually a binary tree, but who cares
	template < class Target >
	class TwoThree
	{
		struct Node
		{
			int Degree;
			Target Value, *Max;
			Node **To;
			Node ()
			{
				Value = Target(-Target::INF, 0);
				Max = new Target[4];
				To = new Node *[4];
				Degree = 0;
				for (int i = 0; i < 4; ++i)
				{
					Max[i] = Target(Target::INF, 0);
					To[i] = 0;
				}
			}
			~Node ()
			{
				delete Max;
				delete To;
			}
		} *Root;
		
		
		void Update(Node *V)
		{
			if(V && V->Degree)
			{
				V->Value = -Target::INF;
				for(int i=0; i<V->Degree; ++i)
				{
					V->Max[i] = V->To[i]->Value;
					V->Value = max(V->Value, V->Max[i]);
				}
				for (int i = V->Degree; i < 4; ++i)
				{
					V->Max[i] = Target::INF;
					V->To[i] = 0;
				}
			}
		}
		void Append (Node *V, Node *U)
		{
			int i;
			for (i = V->Degree-1; i >= 0 && V->Max[i] > U->Value; --i)
			{
				V->Max[i+1] = V->Max[i];
				V->To[i+1] = V->To[i];
			}
			V->To[i+1] = U;
			V->Max[i+1] = U->Value;
			V->Value = max(V->Value, U->Value);
			++V->Degree;
		}
		void Reduce (Node *V, Node *P)
		{
			if (V->Degree == 4)
			{
				V->Degree = 2;
				Node *New = new Node;
				New->Degree = 2;
				for (int i = 0; i <= 1; ++i)
				{
					New->To[i] = V->To[i];
					New->Max[i] = V->Max[i];
					V->To[i] = V->To[i+2];
					V->Max[i] = V->Max[i+2];
					V->To[i+2] = 0;
					V->Max[i+2] = Target::INF;
				}
				New->Value = max(New->Max[0], New->Max[1]);
				if (!P)
				{
					Root = new Node;
					Root->To[0] = New;
					Root->To[1] = V;
					Root->Max[0] = New->Value;
					Root->Max[1] = V->Value;
					Root->Degree = 2;
					Root->Value = max(Root->Max[0], Root->Max[1]);
				}
				else
					Append(P, New);
			}
		}
		void Insert (Node *V, Node *P, Target &Object)
		{
			if (!V)
			{
				Root = new Node;
				Root->Value = Object;
				Root->Degree = 0;
			}
			else
			{
				if (V->Degree > 2 && V->Max[1] < Object && V->To[2])
				{
					Insert(V->To[2], V, Object);
					Reduce(V, P);
				}
				else if (V->Degree > 1 && V->Max[0] < Object && V->To[1])
				{
					Insert(V->To[1], V, Object);
					Reduce(V, P);
				}
				else if (V->Degree > 01 && V->To[0])
				{
					Insert(V->To[0], V, Object);
					Reduce(V, P);
				}
				else if (V->Value != Object)
				{
					Node *New = new Node;
					New->Value = Object;
					New->Degree = 0;
					if (!P)
					{
						if (New->Value > V->Value)
							swap(New, V);
						Root = new Node;
						Root->To[0] = New;
						Root->To[1] = V;
						Root->Max[0] = New->Value;
						Root->Max[1] = V->Value;
						Root->Degree = 2;
						Root->Value = max(Root->Max[0], Root->Max[1]);
					}
					else
						Append(P, New);
				}
				Update(V);
			}
		}
		bool Erase (Node *V, Node *P, Node *GP, Target &Object)
		{
			if (!V)
				return false;
			bool Ret = false;
			if (V->Degree > 2 && V->Max[1] < Object && V->To[2])
				Ret = Erase(V->To[2], V, P, Object);
			else if (V->Degree > 1 && V->Max[0] < Object && V->To[1])
				Ret = Erase(V->To[1], V, P, Object);
			else if (V->Degree > 0 && V->To[0])
				Ret = Erase(V->To[0], V, P, Object);
			else if (V->Value == Object)
				Ret = true;
			
			if (Ret && V->Degree == 0)
			{
				if (!P)
					Root = 0;
				else if (P->Degree == 3)
				{
					for (int i = 0; i < P->Degree; ++i)
						if (P->To[i] == V)
						{
							for (int j = i; j < P->Degree; ++j)
							{
								P->To[j] = P->To[j+1];
								P->Max[j] = P->Max[j+1];
							}
							break;
						}
					P->To[P->Degree-1]=0;
					P->Max[P->Degree-1] = Target(Target::INF, 0);
					--P->Degree;
				}
				else
				{
					Node *B = 0;
					for (int i = 0; i < P->Degree; ++i)
						if (P->To[i] != V)
							B = P->To[i];
					if (!GP)
					{
						Root = B;
						P->Degree = 123456789;
					}
					else
					{
						Node *NP = 0;
						for (int i = 0; i < GP->Degree; ++i)
							if (GP->To[i] == P)
							{
								if(i>0)
									NP = GP->To[i-1];
								else
									NP = GP->To[i+1];
								/*
								for (int j = i; j < GP->Degree; ++j)
								{
									GP->To[j] = GP->To[j+1];
									GP->Max[j] = GP->Max[j+1];
								}
								*/
								break;
							}
						// GP->To[GP->Degree-1] = 0;
						// GP->Max[GP->Degree-1] = Target(Target::INF, 0);
						Append(NP, B);
						Reduce(NP, GP);
						for(int i=0; i<2; ++i)
						{
							P->Max[i] = Target::INF;
							P->To[i] = 0;
						}
						P->Degree = 0;
					}
				}
				delete V;
			}
			if(V->Degree == 123456789)
				delete V;
			//Update(V);
			
			return Ret;
		}
		void Clear (Node *V)
		{
			if (!V)
				return;
			for (int i = 0; i < V->Degree; ++i)
				Clear(V->To[i]);
			delete V;
		}
		int Size (Node *V)
		{
			if (!V)
				return 0;
			int Ret = 0;
			for (int i = 0; i < V->Degree; ++i)
				Ret += Size(V->To[i]);
			return max(Ret, 1);
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
		void Clear ()
		{
			Clear(Root);
		}
		int Size ()
		{
			return Size(Root);
		}
		void Insert (Target &Object)
		{
			Insert(Root, 0, Object);
		}
		bool Erase (Target &Object)
		{
			return Erase(Root, 0, 0, Object);
		}
		bool Find (Target &Object, Target &Result)
		{
			Node *V = Root;
			while (V)
				if (V->Max[1] < Object && V->To[2])
					V = V->To[2];
				else if (V->Max[0] < Object && V->To[1])
					V = V->To[1];
				else if (V->To[0])
					V = V->To[0];
				else if (V->Value == Object)
				{
					Result = V->Value;
					return true;
				}
				else
					return false;
			return false;
		}
		void Test (Node *V)
		{
			if(!V)
				return;
			cout << "V: " << V->Degree << ' ' << V->Value.X << ' ' << V->Value.Y << endl;
			for (int i = 0; i < V->Degree; ++i)
				cout << V->Max[i].X << ' ' << V->Max[i].Y << '\t';
			cout << endl;
			for (int i = 0; i < V->Degree; ++i)
				Test(V->To[i]);
		}
		void Test () /// TODO: GTFO
		{
			cout << "Test\n";
			Test(Root);
		}
	};
}