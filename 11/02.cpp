#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct node
{
	std::string name;
	std::unordered_set<std::string> connections;
};

std::istream& operator>>(std::istream& in, node& n)
{
	n.connections.clear();
	std::string line;
	std::getline(in, line);
	in>>std::ws;

	std::stringstream strm{line};

	strm>>n.name;
	n.name.pop_back();
	std::copy(std::istream_iterator<std::string>{strm}, {}, std::inserter(n.connections, n.connections.begin()));

	return in;
}
int main(int argc, char* argv[])
{
	std::vector<node> nodes{std::istream_iterator<node>{std::cin}, {}};
	std::unordered_map<std::string, std::size_t> node_indices_by_name;
	for(std::size_t i=0; i<nodes.size(); ++i)
		node_indices_by_name[nodes[i].name] = i;

	node_indices_by_name["out"] = nodes.size();
	
	const auto start_idx = node_indices_by_name["svr"];
	const auto target_idx = node_indices_by_name["out"];
	const auto fft_idx = node_indices_by_name["fft"];
	const auto dac_idx = node_indices_by_name["dac"];

	struct counter_collection
	{
		long long normal = 0, with_fft = 0, with_dac = 0, with_both = 0;

		void clear() { normal = with_fft = with_dac = with_both = 0; }

		bool is_empty() const { return (normal+with_fft+with_dac+with_both)==0; }

		counter_collection& operator+=(const counter_collection& other)
		{
			normal+=other.normal;
			with_fft+=other.with_fft;
			with_dac+=other.with_dac;
			with_both+=other.with_both;

			return *this;
		}
	};
	
	std::vector<counter_collection> counts(nodes.size()+1);
	counts[start_idx].normal = 1;

	std::deque<std::size_t> to_visit;
	to_visit.push_back(start_idx);

	while(!to_visit.empty())
	{
		const auto next = to_visit.front();
		to_visit.pop_front();

		if(counts[next].is_empty() || next==target_idx) continue;

		if(next==fft_idx)
		{
			counts[next].with_fft+=std::exchange(counts[next].normal, 0);
			counts[next].with_both+=std::exchange(counts[next].with_dac, 0);
		}

		if(next==dac_idx)
		{
			counts[next].with_dac+=std::exchange(counts[next].normal, 0);
			counts[next].with_both+=std::exchange(counts[next].with_fft, 0);
		}

		for(const auto& other: nodes[next].connections)
		{
			const auto other_idx = node_indices_by_name[other];

			counts[other_idx]+=counts[next];
			to_visit.push_back(other_idx);
		}
		counts[next].clear();
	}

	std::cout<<counts[target_idx].with_both;
}
