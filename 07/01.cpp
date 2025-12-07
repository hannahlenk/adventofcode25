#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct point2d
{
	int x, y;
};

struct vec2d
{
	int dx, dy;
};

point2d operator+(const point2d& lhs, const vec2d& rhs)
{
	return {lhs.x+rhs.dx, lhs.y+rhs.dy};
}

int main(int argc, char* argv[])
{
	std::vector<std::string> grid{std::istream_iterator<std::string>{std::cin}, {}};
	const int width = grid[0].size();
	const int height = grid.size();

	point2d start{};
	for(int y=0; y<height; ++y)
	{
		for(int x=0; x<width; ++x)
		{
			if(grid[y][x]=='S')
				start = {x, y};
		}
	}

	std::vector<point2d> to_visit;
	to_visit.push_back(start);

	std::vector<bool> seen(width*height, false);
	
	int splits = 0;
	while(!to_visit.empty())
	{
		const auto next = to_visit.back();
		to_visit.pop_back();

		if(next.y>=height || seen[next.y*width+next.x]) continue;
		seen[next.y*width+next.x] = true;

		if(grid[next.y][next.x]=='^')
		{
			++splits;
			to_visit.push_back({next.x-1, next.y+1});
			to_visit.push_back({next.x+1, next.y+1});
		}
		else
			to_visit.push_back({next.x, next.y+1});
	}

	std::cout<<splits;
	return 0;
}
