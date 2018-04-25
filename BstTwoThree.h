#pragma once

#include <vector>

using namespace std;

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
				Value = -Target::INF;
				Max = new Target[4];
				To = new Node *[4];
				for (int i = 0; i < 4; ++i)
				{
					Max[3] = Target::INF;
					To[3] = 0;
				}
			}
			~Node ()
			{
				delete Max;
			}
		} *Root;
		
		void Update (Node *V)
		{
			if (V->Degree == 4)
			{
				V->Degree = 2;
				Node **N = new Node *[2];
				for (int i = 0; i <= 1; ++i)
				{
					N[i] = new Node;
					N[i]->Degree = 2;
					for (int j = 0; j <= 1; ++j)
					{
						N[i]->Max[j] = V->Max[i*2+j];
						N[i]->To[j] = V->To[i*2+j];
						N[i]->Value = max(N[i]->Value, N[i]->Max[j]);
					}
					V->Max[i] = N[i]->Value;
					V->To[i] = N[i];
				}
			}
		}
		void Insert (Node *V, Node *P, Target &Object)
		{
			if (V->Max[1] < Object)
			{
				Insert(V->To[2], V, Object);
				Update(V);
			}
			else if (V->Max[0] < Object)
			{
				Insert(V->To[1], V, Object);
				Update(V);
			}
			else if (V->To[0])
			{
				Insert(V->To[0], V, Object);
				Update(V);
			}
			else if (V->Max[0] == Object)
			{
				if (!P)
					P = new Node;
				int i;
				for (i = P->Degree-1; i >= 0 && P->Max[i] > Object; --i)
					P->Max[i+1] = P->Max[i];
				P->To[i+1] = P->To[i];
				P->To[i+1] = new Node;
				P->Max[i+1] = P->To[i+1]->Value = Object;
				P->Value = max(P->Value, Object);
				++P->Degree;
				P->To[i+1]->Degree = 1;
			}
		}
	public:
		TwoThree ()
		{
			Root = 0;
		}
		~TwoThree ()
		{
			/// FUCK IT
		}
		bool Find (Target &Object, Target &Result)
		{
			Node *V = Root;
			while (true)
				if (V->Max[1] < Object)
					V = V->To[2];
				else if (V->Max[0] < Object)
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
		}
	};
}