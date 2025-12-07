#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<long long> previous, next{};
	for(std::string line; std::getline(std::cin, line); )
	{
		previous.resize(line.size());
		next.resize(line.size());

		for(std::size_t i=0; i<line.size(); ++i)
		{
			switch(line[i])
			{
				case 'S': next[i] = 1; break;
				case '.': next[i]+=previous[i]; break;
				case '^': next[i-1]+=previous[i]; next[i+1]+=previous[i]; break;
			}
		}

		previous = std::move(next);
		next.clear();
	}

	std::cout<<std::accumulate(previous.begin(), previous.end(), 0ll);

	return 0;
}
