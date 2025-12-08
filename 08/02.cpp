#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
#include <vector>
#include <utility>

#include <cstddef>

struct point3d
{
	long long x, y, z;
};

std::istream& operator>>(std::istream& in, point3d& p)
{
	char skip;
	return in>>p.x>>skip>>p.y>>skip>>p.z;
}

long long distance(const point3d& lhs, const point3d& rhs)
{
	return (lhs.x-rhs.x)*(lhs.x-rhs.x) + (lhs.y-rhs.y)*(lhs.y-rhs.y) + (lhs.z-rhs.z)*(lhs.z-rhs.z);
}

int main(int argc, char* argv[])
{
	std::vector<point3d> points{std::istream_iterator<point3d>{std::cin}, {}};

	std::vector<int> groups(points.size());
	std::iota(groups.begin(), groups.end(), 0);

	const auto find = [&](int idx)
	{
		int target = idx;
		while(groups[target]!=target)
			target = groups[target];

		while(groups[idx]!=idx)
			idx = std::exchange(groups[idx], target);

		return target;
	};

	const auto merge = [&](int idx0, int idx1)
	{
		idx0 = find(idx0);
		idx1 = find(idx1);

		groups[idx0] = idx1;
	};

	struct connection
	{
		std::size_t idx0, idx1;
	};

	const auto compare = [&](const connection& lhs, const connection& rhs)
	{
		const auto lhs_dist = distance(points[lhs.idx0], points[lhs.idx1]);
		const auto rhs_dist = distance(points[rhs.idx0], points[rhs.idx1]);

		return rhs_dist<lhs_dist;
	};

	std::priority_queue<connection, std::vector<connection>, decltype(compare)> connections{compare};

	for(std::size_t idx0 = 0; idx0<points.size(); ++idx0)
	{
		for(std::size_t idx1 = idx0+1; idx1<points.size(); ++idx1)
			connections.push({idx0, idx1});
	}

	long long result = 0;
	while(!connections.empty())
	{
		const auto to_connect = connections.top();
		connections.pop();
		if(find(to_connect.idx0)!=find(to_connect.idx1))
		{
			merge(to_connect.idx0, to_connect.idx1);
			result = points[to_connect.idx0].x*points[to_connect.idx1].x;
		}
	}

	std::cout<<result;
	return 0;
}
