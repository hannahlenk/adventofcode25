#include <iostream>
#include <string>
#include <string_view>

#include <cstddef>

int main(int argc, char* argv[])
{
	const auto is_repeated = [](std::string_view str, std::size_t length)
	{
		if(str.size()%length!=0) return false;;
		
		for(std::size_t start = length; start<str.size(); start+=length)
		{
			if(str.substr(0, length)!=str.substr(start, length))
				return false;
		}
		return true;
	};
	
	const auto is_invalid = [&](long long id)
	{
		const auto as_str = std::to_string(id);

		for(std::size_t length = 1; length<=as_str.size()/2; ++length)
		{
			if(is_repeated(as_str, length))
				return true;
		}

		return false;
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
