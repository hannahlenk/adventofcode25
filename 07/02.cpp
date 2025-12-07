#include <iostream>
#include <iterator>
#include <queue>
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

	struct waypoint
	{
		point2d current, came_from;

		bool operator<(const waypoint& other) const
		{
			return other.current.y<current.y;
		}
	};
	
	std::priority_queue<waypoint> to_visit;
	to_visit.push({start, {-1,-1}});

	std::vector<unsigned long long> seen(width*height, 0);

	unsigned long long timelines = 0;
	while(!to_visit.empty())
	{
		const auto next = to_visit.top();
		to_visit.pop();

		if(next.current.y>=height)
		{
			timelines+=seen[(next.came_from.y)*width+next.came_from.x];
			continue;
		}

		const auto is_first = seen[next.current.y*width+next.current.x]==0;
		const auto to_add = next.current.y==start.y?1:seen[(next.came_from.y)*width+next.came_from.x];

		seen[next.current.y*width+next.current.x]+=to_add;
		    
		if(!is_first) continue;

		if(grid[next.current.y][next.current.x]=='^')
		{
			to_visit.push({{next.current.x-1, next.current.y}, next.current});
			to_visit.push({{next.current.x+1, next.current.y}, next.current});
		}
		else
			to_visit.push({{next.current.x, next.current.y+1}, next.current});
	}

	std::cout<<timelines;
	return 0;
}
