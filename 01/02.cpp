#include <iostream>

int main(int argc, char* argv[])
{
	int position = 50, step, zeros_seen = 0;
	for(char direction; std::cin>>direction>>step;)
	{
		const auto was_positive = position>0;

		if(direction=='L')
			position-=step;
		else
			position+=step;

		if(was_positive && position<=0)
			++zeros_seen;
		
		zeros_seen+=std::abs(position)/100;
		position = (position%100+100)%100;
	}
	std::cout<<zeros_seen;
	
	return 0;
}
