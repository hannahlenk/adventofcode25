#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include <cstddef>
#include <cstdint>

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

constexpr std::array<vec2d, 8> neighbouring_directions
{
	vec2d{0, 1},
	vec2d{0, -1},
	vec2d{1, 0},
	vec2d{-1, 0},
	vec2d{1, 1},
	vec2d{-1, 1},
	vec2d{1, -1},
	vec2d{-1, -1}
};

int main(int argc, char* argv[])
{
	std::vector<std::string> grid{std::istream_iterator<std::string>{std::cin}, {}};
	const int width = grid[0].size();
	const int height = grid.size();

	int result = 0;
	for(int y=0; y<height; ++y)
	{
		for(int x=0; x<width; ++x)
		{
			int sum = 0;
			for(const auto& v: neighbouring_directions)
			{
				point2d target = point2d{x, y} + v;
				if(target.x<0 || target.x>=width || target.y<0 || target.y>=height) continue;

				sum+=grid[target.y][target.x]=='@';
			}
			result+=sum<4 && grid[y][x]=='@';
		}
	}
	std::cout<<result;
	
	return 0;
}
