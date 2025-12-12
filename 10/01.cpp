#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <set>
#include <vector>

struct button
{
	std::vector<int> affected;
};

std::istream& operator>>(std::istream& in, button& b)
{
	char skip;
	b.affected.clear();

	in>>skip;
	do
	{
		int value;
		in>>value>>skip;
		b.affected.push_back(value);
	}
	while(skip!=')');

	return in;
}

struct indicator_state
{
	std::vector<bool> lights;
};

std::istream& operator>>(std::istream& in, indicator_state& s)
{
	s.lights.clear();

	char skip;
	std::cin>>skip;
	for(char next; in>>next && next!=']';)
		s.lights.push_back(next=='#');

	return in;
}

struct joltage_requirements
{
	std::vector<int> values;
};

std::istream& operator>>(std::istream& in, joltage_requirements& r)
{
	r.values.clear();

	char skip;
	std::cin>>skip;
	for(int next; skip!='}' && in>>next; std::cin>>skip)
		r.values.push_back(next);

	return in;
}

struct indicator
{
	indicator_state desired_state;
	std::vector<button> buttons;
	joltage_requirements requirements;
};


std::istream& operator>>(std::istream& in, indicator& i)
{
	i.buttons.clear();
	
	in>>i.desired_state>>std::ws;
	while(in.peek()=='(')
	{
		button b;
		in>>b>>std::ws;
		i.buttons.push_back(b);
	}

	return in>>i.requirements;
}

int main(int argc, char* argv[])
{
	std::vector<indicator> indicators{std::istream_iterator<indicator>{std::cin},{}};

	const auto required_presses = [](const indicator& indy)
	{
		std::set<std::vector<bool>> seen;
		struct waypoint
		{
			std::vector<bool> state;
			int presses;
		};

		std::deque<waypoint> to_visit;
		to_visit.push_back({std::vector<bool>(indy.desired_state.lights.size(), false), 0});

		while(!to_visit.empty())
		{
			const auto next = to_visit.front();
			to_visit.pop_front();

			if(std::ranges::equal(next.state, indy.desired_state.lights))
				return next.presses;

			for(const auto& b: indy.buttons)
			{
				auto updated_state = next.state;
				for(auto affected: b.affected)
					updated_state[affected] = !updated_state[affected];

				if(seen.contains(updated_state)) continue;
				seen.insert(updated_state);
				to_visit.push_back({std::move(updated_state), next.presses+1});
			}
		}

		return 0;
	};

	long total = 0;
	for(const auto& indy: indicators)
		total+=required_presses(indy);

	std::cout<<total;
	return 0;
}
