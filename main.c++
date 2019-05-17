#include <optional>
#include <string>

using std::string;

struct Cell
{
};

class IntCell : Cell
{
	int value;

public:
	static std::optional<std::pair<size_t, Cell>> parse(string str)
	{
		if (str.empty()) return {};

		bool is_signed = str[0] == '-' || str[0] == '+',
		     positive = str[0] != '-';
		int chars_read = 0;

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

		return { { chars_read, IntCell { num } } };
	};

	IntCell(int value) : value(value) {}
};

int main() {}
