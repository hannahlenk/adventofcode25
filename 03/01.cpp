#include <iostream>
#include <string>

#include <cstddef>

int main(int argc, char* argv[])
{
	long long sum = 0;
	for(std::string line; std::getline(std::cin, line);)
	{
		long long max = 0, max_last = 0;
		for(auto c: line)
		{
			max = std::max(max_last*10+c-'0', max);
			max_last = std::max<long long>(max_last, c-'0');
		}
		sum+=max;
	}

	std::cout<<sum;
	
	return 0;
}
