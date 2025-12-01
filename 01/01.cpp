#include <iostream>

int main(int argc, char* argv[])
{
	int position = 50, step, zeros_seen = 0;
	for(char direction; std::cin>>direction>>step;)
	{
		if(direction=='L')
			position-=step;
		else
			position+=step;

		position = (position%100+100)%100;

		zeros_seen+=position==0;
	}

	std::cout<<zeros_seen;
	
	return 0;
}
