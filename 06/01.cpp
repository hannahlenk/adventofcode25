#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<std::vector<int>> inputs;

	for(std::string line; std::isdigit(std::cin.peek()) && std::getline(std::cin, line); std::cin>>std::ws)
	{
		std::stringstream strm{line};
		inputs.push_back({std::istream_iterator<int>{strm}, {}});
	}

	long long result = 0;
	for(std::size_t i=0; i<inputs[0].size(); ++i)
	{
		char op;
		std::cin>>op;

		if(op=='*')
		{
			long long product = 1;
			for(std::size_t j=0; j<inputs.size(); ++j)
				product*=inputs[j][i];

			result+=product;
		}
		else
		{
			long long sum = 0;
			for(std::size_t j=0; j<inputs.size(); ++j)
				sum+=inputs[j][i];

			result+=sum;
		}
	}
	std::cout<<result;
	return 0;
}
