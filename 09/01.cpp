#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include <cstddef>

struct point2d
{
	long long x, y;
};

std::istream& operator>>(std::istream& in, point2d& p)
{
	char skip;
	return in>>p.x>>skip>>p.y;
}

int main(int argc, char* argv[])
{
	std::vector<point2d> points{std::istream_iterator<point2d>{std::cin}, {}};

	long long result = 0;
	for(std::size_t i=0; i<points.size(); ++i)
		for(std::size_t j=i+1; j<points.size(); ++j)
			result = std::max(result, (std::abs(points[i].x-points[j].x)+1)*(std::abs(points[i].y-points[j].y)+1));

	std::cout<<result;
	return 0;
}
