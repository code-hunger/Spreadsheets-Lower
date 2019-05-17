#include <string>

struct Cell
{
	static std::optional<std::pair<size_t, Cell>> parse(std::string);
};

int main() {}
