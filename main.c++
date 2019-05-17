#include <algorithm>
#include <iostream>
#include <optional>
#include <string>

using std::string;

struct Cell
{
	virtual string str() const = 0;
};

class IntCell : public Cell
{
	int value;

public:
	static std::optional<std::pair<size_t, IntCell>> parse(string str)
	{
		if (str.empty()) return {};

		bool is_signed = str[0] == '-' || str[0] == '+',
		     positive = str[0] != '-';
		size_t chars_read = 0;

		if (is_signed) {
			str = str.substr(1);
			++chars_read;
		}

		int num = 0;

		for (char c : str) {
			if (!isdigit(c)) break;

			num = num * 10 + (c - '0');
			++chars_read;
		}

		if (is_signed && chars_read == 1 or !is_signed && chars_read == 0)
			return {};

		return {std::pair{chars_read, IntCell{num}}};
	};

	IntCell(int value) : value(value) {}

	IntCell(IntCell const& other) : value(other.value) {}

	string str() const
	{
		string x;
		int value = this->value;
		while (value) {
			x += value % 10 + '0';
			value /= 10;
		}

		const int size = x.size();
		for (int i = 0; i < size / 2; ++i) {
			std::swap(x[i], x[size - i - 1]);
		}

		return x;
	}
};

class EmptyCell
{
	static std::optional<std::pair<size_t, EmptyCell>> parse(string)
	{
		return {std::pair{0, EmptyCell{}}};
	}

	string str() { return ""; }
};

template <typename CellType> auto parse(string str)
{
	static_assert(std::is_base_of<Cell, CellType>::value,
	              "Can parse only Cells");
	return CellType::parse(str);
}

int main(int argc, char* argv[])
{
	auto result = parse<IntCell>(argv[1]);

	if (result.has_value()) {
		auto value = result.value();
		Cell& cell = value.second;
		std::cout << "Parsed " << cell.str() << " with " << value.first
		          << " chars." << std::endl;
	} else {
		std::cout << "Couldn't parse!" << std::endl;
	}
}
