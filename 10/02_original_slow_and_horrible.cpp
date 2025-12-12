#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <vector>

#include <cstddef>

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
	for(auto& indy: indicators)
	{
		std::ranges::sort(indy.buttons, [](const auto& lhs, const auto& rhs)
		{
			return lhs.affected.size()>rhs.affected.size();
		});
	}

	const auto required_presses = [](const indicator& indy)
	{
		const auto unsolvable_from = [&](const auto& state, std::size_t first_button_idx, int limit)
		{
			if(state.max_remaining_>=limit) return true;

			for(std::size_t i=0; i<indy.requirements.values.size(); ++i)
			{
				auto missing = state.remaining_[i];
				if(missing<0) return true;

				for(std::size_t j=first_button_idx; j<indy.buttons.size(); ++j)
				{
					bool is_relevant = false;
					int max_presses = std::numeric_limits<int>::max();
					for(auto a: indy.buttons[j].affected)
					{
						is_relevant|=a==static_cast<int>(i);
						max_presses = std::min(max_presses, state.remaining_[a]);
					}
					if(is_relevant)
						missing-=max_presses;
				}
				if(missing>0)
					return true;
			}
			return false;
		};

		struct state
		{
			explicit state(const std::vector<int> values):
				remaining_{values},
				total_remaining_{std::ranges::fold_left(remaining_, 0, std::plus<>{})},
				max_remaining_{std::ranges::fold_left(remaining_, 0, [](auto lhs, auto rhs){ return std::max(lhs, rhs); })}
			{}
			
			void apply(const button& b, int times)
			{
				max_remaining_ = 0;
				for(auto a: b.affected)
				{
					max_remaining_ = std::max(max_remaining_, remaining_[a]+=times);
					total_remaining_+=times;
				}
			}
			
			std::vector<int> remaining_;
			int total_remaining_, max_remaining_;
		};

		state current_state{indy.requirements.values};
		
		const auto count = [&](this auto&& rec, std::size_t idx, int limit = std::numeric_limits<int>::max()) -> std::optional<int>
		{
			if(current_state.total_remaining_==0)
				return 0;

			if(idx>=indy.buttons.size() || unsolvable_from(current_state, idx, limit)) return std::nullopt;

			int tried = 0;
			std::optional<int> min = std::nullopt;
			while(tried<=limit && !unsolvable_from(current_state, idx, limit))
			{
				if(auto result = rec(idx+1, limit-tried); result)
				{
					min = min.transform([&](auto old)
					{
						return std::min(old, tried+*result);
					}).value_or(tried+*result);

					if(*result==0 || *result==current_state.max_remaining_) break;
					limit = std::min(limit, *min);
				}

				current_state.apply(indy.buttons[idx], -1);
				++tried;
			}

			current_state.apply(indy.buttons[idx], tried);
			return min;
		};

		for(int limit = *std::ranges::max_element(indy.requirements.values); ; limit*=2)
			if(auto result = count(0, limit)) return *result;
	};

	long total = 0;
	for(int idx = 0; const auto& indy: indicators)
		std::cerr<<idx++<<": ", total+=required_presses(indy), std::cerr<<total<<'\n';

	std::cout<<total;
	return 0;
}
