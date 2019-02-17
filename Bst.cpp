#include "BstAvl.h"
#include "BstTreap.h"
#include "BstTwoThree.h"
#include "BstRbt.h"

#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace BST;

struct Pair
{
	int X, Y;
	static const int INF = 1e9 + 2;
	Pair(int x = 0, int y = 0) : X(x), Y(y) {}
	bool operator>(const Pair &P) const
	{
		return X > P.X;
	}
	bool operator<(const Pair &P) const
	{
		return X < P.X;
	}
	bool operator==(const Pair &P) const
	{
		return X == P.X;
	}
	bool operator!=(const Pair &P) const
	{
		return X != P.X;
	}
	bool operator>=(const Pair &P) const
	{
		return X >= P.X;
	}
	bool operator<=(const Pair &P) const
	{
		return X <= P.X;
	}
	Pair operator+(int N)
	{
		return Pair(X + N, Y);
	}
};

struct Query
{
	int Type, Response;
	Pair P;
	Query(int t = 0, int x = 0, int y = 0) : Type(t), P(x, y) {}
};

typedef long double ld;

#define random() ( rand()*RAND_MAX + rand() )

int main()
{
	srand(time(0));
	set < Pair > Set;
	AVL < Pair > Avl;
	Treap < Pair > Treap;
	TwoThree < Pair > TwoThree;
	RBT < Pair > RBT;

	cout << "Enter the amount of queries and maximum element value: \n";
	int Size, Value;
	// cin >> Size >> Value;
	Size = 100000, Value = 100000;
	vector < Query > Queries;
	for (int i = 0; i < Size; ++i)
		Queries.push_back(Query((random()%2)*2, random()%Value, random()%Value));
	cout << "Generation complete!\n";

	clock_t Time = clock();
	for (int i = 0; i < Size; ++i)
	{
		switch (Queries[i].Type)
		{
			case 0: Set.insert(Queries[i].P);
				break;
			case 1: Set.erase(Queries[i].P);
				break;
			case 2:
			{
				auto it = Set.find(Queries[i].P);
				if (it == Set.end())
					Queries[i].Response = -1;
				else
					Queries[i].Response = it->Y;
				break;
			}
		}
	}
	cout << "Set processing complete!\n";
	cout << (clock() - Time + 0.0)/ld(CLOCKS_PER_SEC) << endl;

	Time = clock();
	bool Check = true;
	for (int i = 0; i < Size; ++i)
	{
		switch (Queries[i].Type)
		{
			case 0: RBT.insert(Queries[i].P);
				break;
			case 1: return 1;
			case 2:
			{
				Pair Response;
				if (RBT.find(Queries[i].P, Response))
				{
					if (Response.Y != Queries[i].Response)
					{
						cout << "Attention: " << Response.Y << ' ' << Queries[i].Response << endl;
						Check = false;
					}
				}
				else
				{
					if (Queries[i].Response != -1)
					{
						cout << "Attention: " << Queries[i].Response << endl;
						Check = false;
					}
				}
				break;
			}
		}
	}
	cout << "RBT processing complete!\n";
	cout << (clock() - Time + 0.0)/ld(CLOCKS_PER_SEC) << endl;
	if (Check)
		cout << "All answers are matching!\n";
	else
		cout << "Sorry, there are some mistakes...\n";

	Time = clock();
	Check = true;
	for (int i = 0; i < Size; ++i)
	{
		switch (Queries[i].Type)
		{
			case 0: Avl.Insert(Queries[i].P);
				break;
			case 1: Avl.Erase(Queries[i].P);
				break;
			case 2:
			{
				Pair Response;
				if (Avl.Find(Queries[i].P, Response))
				{
					if (Response.Y != Queries[i].Response)
					{
						cout << "Attention: " << Response.Y << ' ' << Queries[i].Response << endl;
						Check = false;
					}
				}
				else
				{
					if (Queries[i].Response != -1)
					{
						cout << "Attention: " << Queries[i].Response << endl;
						Check = false;
					}
				}
				break;
			}
		}
	}
	cout << "Avl processing complete!\n";
	cout << (clock() - Time + 0.0)/ld(CLOCKS_PER_SEC) << endl;
	if (Check)
		cout << "All answers are matching!\n";
	else
		cout << "Sorry, there are some mistakes...\n";

	Time = clock();
	Check = true;
	for (int i = 0; i < Size; ++i)
	{
		switch (Queries[i].Type)
		{
			case 0: Treap.Insert(Queries[i].P, random()%Value);
				break;
			case 1: Treap.Erase(Queries[i].P);
				break;
			case 2: Pair Response;
				if (Treap.Find(Queries[i].P, Response))
				{
					if (Response.Y != Queries[i].Response)
					{
						cout << "Attention: " << Response.Y << ' ' << Queries[i].Response << endl;
						Check = false;
					}
				}
				else
				{
					if (Queries[i].Response != -1)
					{
						cout << "Attention: " << Queries[i].Response << endl;
						Check = false;
					}
				}
				break;
		}
	}
	cout << "Treap processing complete!\n";
	cout << (clock() - Time + 0.0)/ld(CLOCKS_PER_SEC) << endl;
	if (Check)
		cout << "All answers are matching!\n";
	else
		cout << "Sorry, there are some mistakes...\n";

	Time = clock();
	Check = true;
	for (int i = 0; i < Size; ++i)
	{
		switch (Queries[i].Type)
		{
			case 0: TwoThree.Insert(Queries[i].P);
				break;
			case 1: TwoThree.Erase(Queries[i].P);
				break;
			case 2:
			{
				Pair Response;
				if (TwoThree.Find(Queries[i].P, Response))
				{
					if (Response.Y != Queries[i].Response)
					{
						cout << "Attention: " << Response.Y << ' ' << Queries[i].Response << endl;
						Check = false;
					}
				}
				else
				{
					if (Queries[i].Response != -1)
					{
						cout << "Attention: " << Queries[i].Response << endl;
						Check = false;
					}
				}
				break;
			}
		}
	}
	cout << "TwoThree processing complete!\n";
	cout << (clock() - Time + 0.0)/ld(CLOCKS_PER_SEC) << endl;
	cout << "Sizes are: " << Set.size() << ", " << RBT.size() << ", " << Avl.Size() << ", " << Treap.Size() << " and " << TwoThree.Size() << endl;
	if (Check && Set.size() == RBT.size() && Set.size() == Avl.Size() && Set.size() == Treap.Size() && Set.size() == TwoThree.Size())
		cout << "All answers are matching!\n";
	else
		cout << "Sorry, there are some mistakes...\n";
	return 0;
}