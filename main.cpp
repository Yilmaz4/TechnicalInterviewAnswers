#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <numbers>

template <typename type> concept supports_comparison = requires(type x, type y) {
	{ x < y } noexcept -> std::same_as<bool>;
	{ x > y } noexcept -> std::same_as<bool>;
};
template <typename type> concept supported_by_ostream = true;

template <typename type> requires std::integral<type> || supported_by_ostream<type>
void print_array(type * arr, uint64_t n, bool endl = false) {
	for (int i = 0; i < n; i++) {
		std::cout << (!i ? "[" : "") << *(arr + i) << (i != n - 1 ? ", " : "]");
	}
	if (endl) std::cout << std::endl;
}

template <typename type> requires std::integral<type> || supported_by_ostream<type>
void print_array_matrix(type * matrix, const int h, const int w, bool endl = false) {
	for (int x = 0; x < w; x++) {
		std::cout << (!x ? "[[" : " [");
		for (int y = 0; y < h; y++) {
			std::cout << *(matrix + y + h * x) << ((y != h - 1) ? ", " : "]");
		}
		std::cout << (x != w - 1 ? ",\n" : "]");
	}
	if (endl) std::cout << std::endl;
}

template <typename type> requires std::integral<type> || supported_by_ostream<type>
void print_vector(std::vector<type> v, bool endl = false) noexcept {
	std::cout << '{';
	for (int i = 0; i < v.size(); i++) {
		std::cout << v.operator[](i) << ((i != v.size() - 1) ? ", " : "");
	}
	std::cout << '}';
	if (endl) std::cout << std::endl;
}

template <typename type> requires std::integral<type> || supported_by_ostream<type>
void print_vectorial_matrix(std::vector<std::vector<type>> matrix, bool endl = false) {
	std::cout << '{' << std::endl;
	for (auto const& v : matrix) {
		std::cout << "    ";
		print_vector<type>(v);
		std::cout << ',' << std::endl;
	}
	std::cout << '}';
	if (endl) std::cout << std::endl;
}

template <typename type> requires std::constructible_from<std::string> || supported_by_ostream<type>
std::vector<type> take_vector_input(std::string q = ">") noexcept {
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
		for (const char& c : set) {
			switch (c) {
			case '-':
				n *= -1;
				continue;
			case '{':
			case ',':
				out.push_back(n);
				j = n = 0;
				if (c == '{') {
					for (int i = 0; i < out.size() / 2; i++) {
						auto t = out[i];
						out[i] = out[out.size() - i - 1];
						out[out.size() - i - 1] = t;
					}
					return out;
				}
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
	}
}

uint64_t factorial(const uint64_t num, const uint64_t limit = 0) noexcept {
	if (num == 0)
		return num + 1;
	if (num <= 4 && !limit)
		return static_cast<uint64_t>(sqrt(2 * atan2(0, -1) * num) * pow(num / exp(1.0), num) + 0.5);
	else {
		uint64_t out = num;
		for (uint64_t n = num - 1; n > limit; n--)
			out *= n;
		return out;
	}
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
		print_array_matrix<int>(&matrix[0][0], h, w);
	}

	void generate_pascals_triangle() {
		std::cout << "N: ";
		int n;
		std::cin >> n;

		int* pr = new int[n] { 0 };
		int* cr = new int[n] { 0 };

		auto max = [](int n) {
			for (int r = 1, v = 1; r <= n / 2; r++) {
				if (r == n / 2)
					return (int)floor(log10(v) + 1);
				v = v * (n - r) / r;
			}
		} (n);

		for (int i = 1; i <= n; i++) {
			std::cout << std::string(max * (n - i) + (max - i + n), ' ');
			for (int j = 0; j < i; j++) {
				auto v = (!j || j == i - 1 ? 1 : (pr[j - 1] + pr[j]));
				std::cout << std::string(max - (unsigned __int64)floor(log10(v) + 1), '0') + std::to_string(v) << std::string(max + 2, ' ');
				cr[j] = v;
			}
			for (int j = 0; j < i; j++) {
				pr[j] = cr[j];
			}
			std::cout << std::endl;
		}
		delete[] pr;
		delete[] cr;
	}

	std::vector<int> find_next_lexicographic_permutation(std::vector<int>& in) {
		std::vector<int> v = in;
		for (uint64_t i = 0; i < v.size(); i++) {
			int max = INT_MAX;
			uint64_t idx = -1;
			for (uint64_t j = i; j < v.size(); j++) {
				if (v[j] < max) {
					max = v[j];
					idx = j;
				}
			}
			int tmp = v[i];
			v[i] = v[idx];
			v[idx] = tmp;
		}
		uint64_t n = factorial(static_cast<uint64_t>(v.size()));
		std::vector<std::vector<int>> permutations(n, std::vector<int>(v.size()));
		for (uint64_t i = 0; i < v.size(); i++) {
			unsigned __int64 n1 = factorial(static_cast<uint64_t>(v.size()), static_cast<uint64_t>(v.size()) - i - 1);
			unsigned __int64 n2 = n / n1;
			uint64_t idx = 0;
			uint64_t y = 0;
			std::vector<int> prvs(v.size());
			for (uint64_t j = 0; j < n1; j++) {
				std::vector<int> cv = v;
				for (uint64_t k = 0; k < i; k++) {
					for (uint64_t l = 0; l < cv.size(); l++) {
						if (cv[l] == permutations[y][k]) {
							cv.erase(cv.begin() + l);
						}
					}
				}
				for (uint64_t k = 0; k < i; k++) {
					if (prvs[k] != permutations[y][k]) {
						for (uint64_t l = 0; l < i; l++) {
							prvs[l] = permutations[y][l];
						}
						idx = 0;
					}
				}
				int obj = cv[idx++];
				for (uint64_t k = 0; k < n2; k++) {
					permutations[y++][i] = obj;
				}
			}
		}
		for (uint64_t i = 0; i < permutations.size(); i++) {
			if (permutations[i] == in) {
				return permutations[i + 1];
			}
		}
	}

	int64_t find_max_subarray_sum(std::vector<int>& v) {
		int64_t sum = INT_MIN;
		for (int i = 0; i < v.size(); i++) {
			for (int j = i; j < v.size(); j++) {
				int64_t tmp = sum;
				sum = 0;
				for (int k = i; k <= j; k++) {
					sum += v[k];
				}
				if (tmp > sum) {
					sum = tmp;
				}
			}
		}
		return sum;
	}

	void sort_array_of_0s_1s_and_2s(std::vector<int>& v) {
		uint64_t lo = 0;
		uint64_t hi = v.size() - 1;
		uint64_t mid = 0;

		while (mid <= hi) {
			switch (v[mid]) {
			case 0:
				std::swap(v[lo++], v[mid++]);
				break;
			case 1:
				mid++;
				break;
			case 2:
				std::swap(v[mid], v[hi--]);
				break;
			}
		}
	}

	uint64_t max_profit(std::vector<int> prices) {
		int min = INT_MAX;
		uint64_t idx = 0;
		for (uint64_t i = 0; i < prices.size(); i++) {
			if (prices[i] < min) {
				idx = i;
				min = prices[i];
			}
		}
		int max = INT_MIN;
		for (uint64_t i = idx; i < prices.size(); i++) {
			if (prices[i] > max) {
				max = prices[i];
			}
		}
		return max - min;
	}

	std::vector<std::vector<int>> rotate_matrix_90_clockwise(std::vector<std::vector<int>> matrix) {
		std::vector<std::vector<int>> out = matrix;
		for (uint64_t i = 0; i < matrix[0].size(); i++) {
			for (int64_t j = matrix.size() - 1; j >= 0; j--) {
				out[i][matrix.size() - 1 - j] = matrix[j][i];
			}
		}
		return out;
	}
}

int main(int argc, char* argv[]) {
	print_vectorial_matrix(arrays::rotate_matrix_90_clockwise(
		{ {5,1,9,11},{2,4,8,10},{13,3,6,7},{15,14,12,16} }
	));
}