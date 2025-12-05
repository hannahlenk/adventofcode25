#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct range
{
	unsigned long long start, end;

	friend auto operator<=>(const range&, const range&) = default; 
};

int main(int argc, char* argv[])
{
	std::vector<range> ranges;
	
	for(std::string line; std::getline(std::cin, line) && !line.empty(); )
	{
		std::stringstream strm{line};
		range r;
		char skip;
		strm>>r.start>>skip>>r.end;
		ranges.push_back(r);
	}

	std::ranges::sort(ranges);

	unsigned long long total = 0;
	for(unsigned long long end = 0; const auto& range: ranges)
	{
		const auto start = std::max(end+1, range.start);
		end = std::max(end, range.end);

		total+=std::max(0ull, end-start+1);
	}

	std::cout<<total;
	return 0;
}
