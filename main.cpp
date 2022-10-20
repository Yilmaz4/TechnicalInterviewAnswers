#include <iostream>
#include <string>
#include <vector>

#include <algorithm> // temporary

template <typename type> requires std::integral<type> || requires(type x) {
	{ std::cout << x } noexcept -> std::same_as<std::ostream>;
} void print_matrix(type* matrix, const int h, const int w, bool endl = false) {
	for (int x = 0; x < w; x++) {
		std::cout << ((!x) ? "[[" : " [");
		for (int y = 0; y < h; y++) {
			std::cout << *(matrix + y + h * x) << ((y != h - 1) ? ", " : "]");
		}
		std::cout << ((x != w - 1) ? ",\n" : "]");
	}
	if (endl) std::cout << std::endl;
}

template <typename type> requires std::integral<type> || requires(type x) {
	{ std::cout << x } noexcept -> std::same_as<std::ostream>;
} void print_vector(std::vector<type> v, bool endl = false) noexcept {
	std::cout << '{';
	for (int i = 0; i < v.size(); i++) {
		std::cout << v.operator[](i) << ((i != v.size() - 1) ? ", " : "");
	}
	std::cout << '}';
	if (endl) std::cout << std::endl;
}

template <typename type> requires std::constructible_from<std::string> || requires(type x) {
	{ std::cout << x } noexcept -> std::same_as<std::ostream>;
} std::vector<type> take_vector_input(std::string q = '>') noexcept {
	while (true) {
		std::cout << q;
		std::string set;
		std::getline(std::cin, set);

		if (set.at(0) != '{' || set.at(set.length() - 1) != '}') {
		syntax_error:
			std::cout << "Invalid syntax" << std::endl;
			continue;
		}

		// reverse the vector
		for (int i = 0; i < set.size() / 2; i++) {
			char t = set[i];
			set[i] = set[set.size() - i - 1];
			set[set.size() - i - 1] = t;
		}

		std::vector<int> out;
		int j = 0, n = 0;
		bool nF = false;
		for (const char& c : set) {
			switch (c) {
			case '-':
				nF = true;
				continue;
			case '{':
			case ',':
				out.push_back(n);
				j = n = nF = 0;
				if (c == '{')
					return out;
				__fallthrough;
			case '}':
			case ' ':
				break;
			default:
				if (!isdigit(c)) {
					goto syntax_error;
				}
				n += (int)(pow(10, j++)) * (c - '\60');
			}
		}
		for (int i = 0; i < out.size() / 2; i++) {
			auto t = out[i];
			out[i] = out[out.size() - i - 1];
			out[out.size() - i - 1] = t;
		}
	}
}

uint64_t factorial(const uint64_t num) {
	uint64_t out = num;
	for (int n = num - 1; n > 0; n--)
		out *= n;
	return out;
}

namespace arrays {
	void set_matrix_zeroes() {
		static constexpr int h = 3;
		static constexpr int w = 3;

		int matrix[w][h] = {
			{1, 1, 1},
			{1, 0, 1},
			{1, 1, 1}
		};

		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				matrix[x][y] = (!matrix[x][y] ? -1 : matrix[x][y]);
			}
		}
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				if (matrix[x][y] == -1) {
					for (int i = 0; i < h; i++) {
						matrix[x][i] = 0;
					}
					for (int i = 0; i < w; i++) {
						matrix[i][y] = 0;
					}
				}
			}
		}
		print_matrix<int>(&matrix[0][0], h, w);
	}

	void generate_pascals_triangle() {
		// TO-CONTINUE (i can't, so hard)
		std::cout << "N: ";
		uint64_t n;
		std::cin >> n;

		int* crow = new int[n] { 0 };
		int* prow = new int[n] { 0 };

		int cridx = 0;

		for (int i = 1; i <= n; i++) {
			std::cout << std::string(n - i, ' ');
			for (int j = 0; j < i; j++) {
				auto v = (!j ? 1 : (prow[j] + prow[j + 1]));
				std::cout << v << ' ';
				crow[cridx++] = v;
			}
			std::cout << std::endl;
			cridx = 0;
			for (int j = 0; j < i; j++) {
				prow[j] = crow[j];
			}
		}
	}

	template <typename type> void permute(std::vector<type> v, uint64_t sIdx = static_cast<uint64_t>(-1), uint64_t eIdx = static_cast<uint64_t>(-1)) {
		if (sIdx == eIdx) {
			print_vector(v);
			return;
		}
		if (sIdx == static_cast<uint64_t>(-1))
			sIdx = 0;
		if (eIdx == static_cast<uint64_t>(-1))
			eIdx = v.size() - 1;
		for (int i = 0; i <= eIdx; i++) {
			std::swap(v[sIdx], v[i]);
			permute<type>(v, sIdx + 1, eIdx);
			std::swap(v[sIdx], v[i]);
		}
	}

	void find_next_lexicographical_permutation() {
		std::vector<int> in = take_vector_input<int>("V: ");
		std::vector<int> be = in;
		
		std::stable_sort(in.begin(), in.end());
		print_vector(in, true);
		int i;
		for (i = 1; in != be; i++) {
			std::next_permutation(in.begin(), in.end());
			print_vector(in, true);
		}
		std::cout << "Total of " << i << " permutations\n";
	}
}


int main(int argc, char* argv[]) {
	while (true)
	arrays::find_next_lexicographical_permutation();
}