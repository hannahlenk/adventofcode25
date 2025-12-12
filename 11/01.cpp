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

	const auto start_idx = node_indices_by_name["you"];
	const auto target_idx = node_indices_by_name["out"];
	
	std::vector<int> counts(nodes.size(), 0);
	counts[start_idx] = 1;

	std::deque<std::size_t> to_visit;
	to_visit.push_back(start_idx);

	while(!to_visit.empty())
	{
		const auto next = to_visit.front();
		to_visit.pop_front();

		if(counts[next] == 0 || next==target_idx) continue;

		for(const auto& other: nodes[next].connections)
		{
			const auto other_idx = node_indices_by_name[other];
			counts[other_idx]+=counts[next];
			to_visit.push_back(other_idx);
		}
		counts[next] = 0;
	}

	std::cout<<counts[target_idx];
}
