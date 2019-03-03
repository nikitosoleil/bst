#include "Bst.h"
#include "BstAvl.h"
#include "BstTreap.h"
#include "BstTwoThree.h"
#include "BstSplay.h"

#include <set>

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

	vector < Base < Pair > * > trees{new AVL < Pair >(), new TwoThree < Pair >(), new Treap < Pair >(), new Splay < Pair >()};
	vector < string > tree_names{"AVL", "2-3", "Treap", "Splay"};

	cout << "Enter the amount of queries and maximum element value: \n";
	int Size, Value;
	// cin >> Size >> Value;
	Size = 1000000, Value = 1000000;
	vector < Query > Queries;
	for (int i = 0; i < Size; ++i)
		Queries.push_back(Query(random()%3, random()%Value, random()%Value));
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
	cout << "Size is: " << Set.size() << endl;

	for (int i = 0; i < trees.size(); ++i)
	{
		Base < Pair > *tree = trees[i];
		Time = clock();
		bool Check = true;
		for (int i = 0; i < Size; ++i)
		{
			switch (Queries[i].Type)
			{
				case 0: tree->insert(Queries[i].P);
					break;
				case 1: tree->erase(Queries[i].P);
					break;
				case 2:
				{
					Pair Response;
					if (tree->find(Queries[i].P, Response))
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
		cout << tree_names[i] << " processing complete!\n";
		cout << (clock() - Time + 0.0)/ld(CLOCKS_PER_SEC) << endl;
		if (Check)
			cout << "All answers are matching!\n";
		else
			cout << "Sorry, there are some mistakes...\n";
		cout << "Size is: " << tree->size() << endl;
		cout << endl;
	}
	return 0;
}