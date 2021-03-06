#include <Core/Core.h>
#include <vector>

using namespace Upp;

#define N 10000000

CONSOLE_APP_MAIN
{
	SeedRandom(0);

	Vector<String> a;
	std::vector<std::string> c;
	for(int i = 0; i < N; i++) {
		String s = AsString(Random()) + AsString(Random()) +
		           AsString(Random()) + AsString(Random()) +
		           AsString(Random()) + AsString(Random())
		;
		a.Add(s);
		c.push_back(s.ToStd());
	}

	{
		Vector<String> b = clone(a);
		{
			RTIMING("Sort");
			Sort(b);
		}
	}
	{
		Vector<String> b = clone(a);
		RTIMING("StableSort");
		StableSort(b);
	}
	{
		std::vector<std::string> d = c;
		{
			RTIMING("std::sort");
			std::sort(d.begin(), d.end());
		}
	}
	{
		std::vector<std::string> d = c;
		RTIMING("std::stable_sort");
		std::stable_sort(d.begin(), d.end());
	}
}
