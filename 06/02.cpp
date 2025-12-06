#include <iostream>
#include <string>
#include <vector>

#include <cstddef>

int main(int argc, char* argv[])
{
	std::vector<std::string> inputs;
	for(std::string line; std::getline(std::cin, line);)
		inputs.push_back(line);

	long long result = 0;
	for(std::size_t i=1, last_op = 0; i<=inputs.back().size(); ++i)
	{
		if(i==inputs.back().size() || !std::isspace(inputs.back()[i]))
		{
			const auto op = inputs.back()[last_op];

			long long v = op=='*'?1:0;
			for(int j=i==inputs.back().size()?i-1:(i-2); j>=static_cast<int>(last_op); --j)
			{
				long long value = 0;
				for(std::size_t k=0; k+1<inputs.size(); ++k)
				{
					if(!std::isspace(inputs[k][j]))
						value = value*10+inputs[k][j]-'0';
				}
				v = op=='*'?(v*value):(v+value);
			}

			result+=v;
			last_op = i;
		}
	}

	std::cout<<result;
	return 0;
}
