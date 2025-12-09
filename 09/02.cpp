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

	const auto is_inside = [&](point2d a, point2d b)
	{
		const auto min = point2d{std::min(a.x, b.x), std::min(a.y, b.y)};
		const auto max = point2d{std::max(a.x, b.x), std::max(a.y, b.y)};

		for(std::size_t i=0; i<points.size(); ++i)
		{
			const auto first = points[i];
			const auto second = points[(i+1)%points.size()];

			const auto line_min = point2d{std::min(first.x, second.x), std::min(first.y, second.y)};
			const auto line_max = point2d{std::max(first.x, second.x), std::max(first.y, second.y)};

			if(max.x<line_min.x || min.x>line_max.x || max.y<line_min.y || min.y>line_max.y) continue;

			if(line_min.x<=min.x && line_max.x>min.x && line_min.y>min.y && line_max.y<max.y) return false;
			if(line_max.x>=max.x && line_min.x<max.x && line_min.y>min.y && line_max.y<max.y) return false;

			if(line_min.y<=min.y && line_max.y>min.y && line_min.x>min.x && line_max.x<max.x) return false;
			if(line_max.y<=max.y && line_min.y<max.y && line_min.x>min.x && line_max.x<max.x) return false;
		}

		return true;
	};

	long long result = 0;
	for(std::size_t i=0; i<points.size(); ++i)
	{
		for(std::size_t j=i+1; j<points.size(); ++j)
		{
			if(points[i].x==points[j].x || points[i].y==points[j].y) continue;

			if(is_inside(points[i], points[j]))
				result = std::max(result, (std::abs(points[i].x-points[j].x)+1)*(std::abs(points[i].y-points[j].y)+1));
		}
	}
	
	std::cout<<result;
	return 0;
}
