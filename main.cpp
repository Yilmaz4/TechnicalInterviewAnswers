#include <iostream>
#include <string>
#include <vector>

template <typename type> concept supports_comparison = requires(type x, type y) {
	{ x < y } noexcept -> std::same_as<bool>;
	{ x > y } noexcept -> std::same_as<bool>;
};
template <typename type> concept supported_by_ostream = requires(type x) {
	{ std::cout << x } noexcept -> std::convertible_to<std::ostream>;
};

template <typename type> requires std::integral<type> || supported_by_ostream<type>
void print_array(type* arr, uint64_t n, bool endl = false) {
	for (int i = 0; i < n; i++) {
		std::cout << (!i ? "[" : "") << *(arr + i) << (i != n - 1 ? ", " : "]");
	}
	if (endl) std::cout << std::endl;
}

template <typename type> requires std::integral<type> || supported_by_ostream<type>
void print_array_matrix(type* matrix, const int h, const int w, bool endl = false) {
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
	std::cout << '{';
	for (auto const& v : matrix) {
		print_vector<type>(v);
		std::cout << ',' << std::endl;
	}
	std::cout << '}';
	if (endl) std::cout << std::endl;
}

template <typename type> requires std::constructible_from<std::string> || supported_by_ostream<type>
std::vector<type> take_vector_input(std::string q = '>') noexcept {
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

uint64_t factorial(const uint64_t num) {
	if (!num)
		return num + 1;
	uint64_t out = num;
	for (uint64_t n = num - 1; n > 0; n--)
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
		print_array_matrix<int>(&matrix[0][0], h, w);
	}

	void generate_pascals_triangle() {
		std::cout << "N: ";
		int n;
		std::cin >> n;

		int* prow = new int[n] { 0 };
		int* crow = new int[n] { 0 };

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
				auto v = (!j || j == i - 1 ? 1 : (prow[j - 1] + prow[j]));
				std::cout << std::string(max - floor(log10(v) + 1), '0') + std::to_string(v) << std::string(max + 2, ' ');
				crow[j] = v;
			}
			for (int j = 0; j < i; j++) {
				prow[j] = crow[j];
			}
			std::cout << std::endl;
		}
		delete[] prow;
		delete[] crow;
	}

	template <typename type> requires supports_comparison<type>
	std::vector<std::vector<type>> find_next_lexicographic_permutation(std::vector<type>& in) {
		// TO-CONTINUE (i've got the algorithm right)
		std::vector<std::vector<type>> perms(factorial(in.size()), std::vector<type>(in.size()));
		for (int i = 0; i < in.size(); i++) {
			uint64_t n = factorial((uint64_t)(in.size() - i - 1));
			for (int j = 0; j < in.size(); j++) {
				std::vector<type> elms = in;
				for (uint64_t idx = 0; idx < (!i ? 0 : elms.size()); idx++) {
					for (int k = 0; k < i; k++) {
						if (elms[idx] == perms[j][k]) {
							elms.erase(elms.begin() + idx);
						}
					}
				}
				uint64_t idx = 0;
				for (int k = 0; k < n; k++) {
					perms[(j + 1) * k][i] = elms[idx];
					if (k % (uint64_t)(n / (in.size() - i - 1)) == 0) {
						idx++;
					}
				}
			}
		}
		return perms;
	}

	int64_t find_max_subarray_sum(std::vector<int>& v) {
		int max = INT_MIN;
		for (int i = 0; i < v.size(); i++) {
			max = (v.at(i) > max ? v[i] : max);
		}
		// omg this is way harder than i expected!
		return NULL;
	}
}


int main(int argc, char* argv[]) {
	arrays::generate_pascals_triangle();
	/*auto v = take_vector_input<int>("V: ");
	print_vectorial_matrix(arrays::find_next_lexicographic_permutation(v));*/
}