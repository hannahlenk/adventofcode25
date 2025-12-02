#include <iostream>
#include <string>

#include <cstddef>

int main(int argc, char* argv[])
{
	const auto is_invalid = [](long long id)
	{
		const auto as_str = std::to_string(id);
		if(as_str.size()%2!=0) return false;

		for(std::size_t i=0; i<as_str.size()/2; ++i)
			if(as_str[i]!=as_str[i+as_str.size()/2])
				return false;

		return true;
	};
	
	char skip;
	long long invalid = 0;
	for(long long start, end; std::cin>>start>>skip>>end; std::cin>>skip)
	{
		for(long long id=start; id<=end; ++id)
		{
			if(is_invalid(id))
				invalid+=id;
		}
	}

	std::cout<<invalid;
	return 0;
}
