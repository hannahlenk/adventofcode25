#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <ranges>
#include <unordered_set>
#include <vector>

#include <cstddef>

struct button
{
	std::vector<long> affected;
};

std::istream& operator>>(std::istream& in, button& b)
{
	char skip;
	b.affected.clear();

	in>>skip;
	do
	{
		long value;
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
	std::vector<long> values;
};

std::istream& operator>>(std::istream& in, joltage_requirements& r)
{
	r.values.clear();

	char skip;
	std::cin>>skip;
	for(long next; skip!='}' && in>>next; std::cin>>skip)
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

struct equation
{
	std::vector<long> coefficients;
	long sum;
};

std::vector<equation> to_equations(const indicator& indy)
{
	std::vector<equation> result;

	for(std::size_t i=0; i<indy.requirements.values.size(); ++i)
	{
		equation eq;
		eq.sum = indy.requirements.values[i];
		for(std::size_t button_idx=0; button_idx<indy.buttons.size(); ++button_idx)
		{
			const auto found = std::ranges::find(indy.buttons[button_idx].affected, i)!=indy.buttons[button_idx].affected.end();
			eq.coefficients.push_back(found?1:0);
		}
		result.push_back(std::move(eq));
	}

	return result;
}

auto gauss_jordan_elimination(std::vector<equation> equations)
{
	struct result_type
	{
		std::vector<std::optional<equation>> equation_for;
		std::unordered_set<std::size_t> free;
	} result;

	const auto number_of_variables = equations.front().coefficients.size();

	const auto eliminate_from = [&](equation& target, std::size_t to_eliminate, const equation& source)
	{
		if(target.coefficients[to_eliminate]==0) return;

		const auto gcd = std::gcd(target.coefficients[to_eliminate], source.coefficients[to_eliminate]);
		const auto factor_to_keep = source.coefficients[to_eliminate]/gcd;
		const auto factor_to_remove = target.coefficients[to_eliminate]/gcd;

		for(std::size_t var_idx = 0; var_idx<number_of_variables; ++var_idx)
		{
			target.coefficients[var_idx]*=factor_to_keep;
			target.coefficients[var_idx]-=factor_to_remove*source.coefficients[var_idx];	
		}
		target.sum*=factor_to_keep;
		target.sum-=factor_to_remove*source.sum;
	};
	
	for(std::size_t i=0, next_unused = 0; i<number_of_variables; ++i)
	{
		bool any_found = false;
		for(std::size_t j=next_unused; j<equations.size(); ++j)
		{
			if(equations[j].coefficients[i]!=0)
			{
				std::swap(equations[next_unused], equations[j]);
				any_found = true;
				break;
			}
		}

		if(!any_found)
		{
			result.free.insert(i);
			continue;
		}

		for(std::size_t j=0; j<equations.size(); ++j)
		{
			if(j==next_unused) continue;
			eliminate_from(equations[j], i, equations[next_unused]);
		}
		++next_unused;
	}

	for(std::size_t i=0, next = 0; i<number_of_variables; ++i)
	{
		if(result.free.contains(i))
			result.equation_for.push_back(std::nullopt);
		else
			result.equation_for.push_back(std::move(equations[next++]));
	}

	return result;
}

bool try_resolve(std::vector<std::optional<long>>& partial, const auto& solution_set)
{
	const auto try_resolve_at = [&](this auto&& rec, std::size_t idx) -> std::optional<long>
	{
		if(!partial[idx])
		{
			long sum = solution_set.equation_for[idx]->sum;
			for(std::size_t i=0; i<partial.size(); ++i)
			{
				if(i==idx) continue;

				const auto factor = solution_set.equation_for[idx]->coefficients[i];
				if(factor!=0)
				{
					if(auto other = rec(i); other)
						sum-=factor**other;
					else
						return std::nullopt;
				}
			}

			if(sum%solution_set.equation_for[idx]->coefficients[idx]!=0) return std::nullopt;

			sum/=solution_set.equation_for[idx]->coefficients[idx];
			
			if(sum<0) return std::nullopt;
			
			return partial[idx] = sum;
		}

		return partial[idx];
	};

	for(std::size_t i=0; i<partial.size(); ++i)
		if(!solution_set.free.contains(i))
			partial[i] = std::nullopt;

	for(std::size_t i=0; i<solution_set.equation_for.size(); ++i)
		if(!try_resolve_at(i))
			return false;

	return true;
}

int main(int argc, char* argv[])
{
	const auto required_presses = [](const indicator& indy)
	{
		const auto equations = to_equations(indy);
		const auto solution_set = gauss_jordan_elimination(equations);

		std::vector<std::optional<long>> partial_solution(solution_set.equation_for.size(), std::nullopt);

		long min = std::ranges::fold_left(indy.requirements.values, 0l, std::plus<>{});
		const auto minimize = [&](this auto&& rec, auto next_free_it, long partial_sum)
		{
			if(partial_sum>=min) return;

			if(next_free_it==solution_set.free.end())
			{
				if(try_resolve(partial_solution, solution_set))
				{
					const auto total = std::transform_reduce(partial_solution.begin(), partial_solution.end(), 0l, std::plus<>{}, [](auto v) { return *v; });
					min = std::min(min, total);
				}
				return;
			}
			
			for(long i = 0; partial_sum+i<min; ++i)
			{
				partial_solution[*next_free_it] = i;
				rec(std::next(next_free_it), partial_sum+i);
			}
			partial_solution[*next_free_it] = std::nullopt;
		};

		minimize(solution_set.free.begin(), 0);
		return min;
	};

	auto input = std::views::istream<indicator>(std::cin);
	auto presses = std::views::transform(input, required_presses);
	const auto result = std::ranges::fold_left(presses, 0l, std::plus<>{});
	std::cout<<result;

	return 0;
}
