#include <array>
#include <iostream>
#include <string>

#include <cstddef>

int main(int argc, char* argv[])
{
	long long sum = 0;
	for(std::string line; std::getline(std::cin, line);)
	{
		std::array<long long, 12> max_per_length{};
		for(auto c: line)
		{
			const long long digit = c - '0';
			std::array<long long, 12> new_max_per_length{};

			new_max_per_length[0] = std::max(max_per_length[0], digit);
			for(std::size_t length = 1; length<max_per_length.size(); ++length)
				new_max_per_length[length] = std::max(max_per_length[length], max_per_length[length-1]*10+digit);

			max_per_length = new_max_per_length;
		}
		sum+=max_per_length.back();
	}

	std::cout<<sum;
	
	return 0;
}
