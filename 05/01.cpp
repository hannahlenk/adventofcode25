#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct range
{
	unsigned long long start, end;
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

	int count = 0;
	for(unsigned long long id; std::cin>>id; )
	{
		for(const auto& r: ranges)
		{
			if(id>=r.start && id<=r.end)
			{
				++count;
				break;
			}
		}
	}

	std::cout<<count;
	return 0;
}
