#include <array>
#include <bitset>
#include <iostream>
#include <vector>

#include <cstddef>

constexpr auto shape_width = 3;
constexpr auto number_of_presents = 6;
using present_grid = std::bitset<shape_width*shape_width>;

std::array<present_grid, number_of_presents> read_presents()
{
	std::array<present_grid, number_of_presents> shapes;
	for(auto& shape: shapes)
	{
		char c;
		std::cin>>c>>c;
		for(std::size_t i=0; i<shape_width*shape_width; ++i)
		{
			std::cin>>c;
			shape[i] = c=='#';
		}
	}

	return shapes;
}

present_grid rotated_right(const present_grid& shape)
{
	present_grid result;
	for(int y=0; y<shape_width; ++y)
		for(int x=0; x<shape_width; ++x)
			result[(x*shape_width)+(shape_width-1-y)] = shape[y*shape_width+x];

	return result;
}

present_grid hflipped(const present_grid& shape)
{
	present_grid result;

	for(int y=0; y<shape_width; ++y)
		for(int x=0; x<shape_width; ++x)
			result[(shape_width-1-y)*shape_width+x] = shape[y*shape_width+x];

	return result;
};

int main(int argc, char* argv[])
{
	const auto shapes = read_presents();

	const auto can_fit = [&](int width, int height, const std::array<int, number_of_presents>& counts)
	{
		int total_required = 0;
		for(int i=0; i<number_of_presents; ++i)
			total_required+=shapes[i].count()*counts[i];

		if(total_required>width*height) return false;

		std::vector<bool> grid(width*height, false);

		const auto could_fit = [&](int x, int y, const auto& shape)
		{
			if(x+shape_width>width || y+shape_width>height) return false;
			for(int sy = 0; sy<shape_width; ++sy)
				for(int sx = 0; sx<shape_width; ++sx)
					if(grid[(y+sy)*width+(x+sx)] && shape[sy*shape_width+sx])
						return false;

			return true;
		};

		const auto flip = [&](int x, int y, const auto& shape)
		{
			for(int sy = 0; sy<shape_width; ++sy)
				for(int sx = 0; sx<shape_width; ++sx)
					if(shape[sy*shape_width+sx])
						grid[(y+sy)*width+(x+sx)].flip();
		};

		const auto search = [&](this auto&& rec, int present_idx, int count)
		{
			while(present_idx<number_of_presents && count>=counts[present_idx])
				count = 0, ++present_idx;
			
			if(present_idx>=number_of_presents) return true;
			
			const auto attempt = [&](int x, int y, const auto& shape)
			{
				if(could_fit(x, y, shape))
				{
					flip(x, y, shape);
						const auto worked = rec(present_idx, count+1);
					flip(x, y, shape);

					if(worked) return true;
				}
				return false;
			};

			for(int y=0; y<height; ++y)
			{
				for(int x=0; x<width; ++x)
				{
					auto shape = shapes[present_idx];
					auto flipped_shape = hflipped(shapes[present_idx]);
					for(int rotation = 0; rotation<4; ++rotation)
					{
						if(attempt(x, y, shape)) return true;
						if(attempt(x, y, flipped_shape)) return true;
						
						shape = rotated_right(shape);
						flipped_shape = rotated_right(flipped_shape);
					}
				}
			}
			return false;
		};

		return search(0, 0);
	};

	int result = 0;
	char skip;
	for(int region_width, region_height; std::cin>>region_width>>skip>>region_height>>skip; )
	{
		std::array<int, number_of_presents> counts{};
		for(auto& count: counts) std::cin>>count;

		if(can_fit(region_width, region_height, counts))
			++result;
	}

	std::cout<<result;
	return 0;
}
