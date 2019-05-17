#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

using std::string;
using std::unique_ptr;

struct Cell
{
	virtual string str() const = 0;
};

using ParseResult = std::optional<std::pair<size_t, unique_ptr<Cell>>>;

class IntCell : public Cell
{
	int value;

public:
	static ParseResult parse(string str)
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

		return {std::pair{chars_read, std::make_unique<IntCell>(num)}};
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

struct EmptyCell : Cell
{
	static ParseResult parse(string)
	{
		return {std::pair{0, std::make_unique<EmptyCell>()}};
	}

	string str() const { return ""; }
};

class StringCell : public Cell
{
	string value;

public:
	StringCell(string str) : value(str) {}

	static ParseResult parse(string str)
	{
		bool behindBackquote = false;
		char quote = '"';
		if (str[0] != quote) return {};

		int length = 1;
		while ((behindBackquote || str[length] != quote) and
		       length < str.size()) {
			++length;
		}

		if (behindBackquote || str[length] != quote) return {};

		return {std::pair(length,
		                  std::make_unique<StringCell>(str.substr(0, length)))};
	}

	string str() const { return value; }
};

template <typename CellType> auto parse(string str)
{
	static_assert(std::is_base_of<Cell, CellType>::value,
	              "Can parse only Cells");
	return CellType::parse(str);
}

using std::vector;

class Table {
	using RowT = vector<unique_ptr<Cell>>;
	vector<RowT> data;

public:
	Table () = default;
};

int main(int argc, char* argv[])
{
	ParseResult result = parse<IntCell>(argv[1]);

	if (result.has_value()) {
		auto& value = result.value();
		auto& cell = value.second;
		std::cout << "Parsed " << cell->str() << " with " << value.first
		          << " chars." << std::endl;
	} else {
		std::cout << "Couldn't parse!" << std::endl;
	}
}
