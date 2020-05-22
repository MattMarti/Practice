#include <iostream>
#include <string>
#include <regex>
#include <iterator>

int main() {
    using namespace std;

    cout << "Starting" << endl;

    string str = "some subsequence within a substring within a string";
    regex exp("\\b(sub)([^ ]*)"); // Match words beginning with sub
    string rep = "sub-$2";

	// using string/c-string (3) version:
    cout << regex_replace(str, exp, rep) << endl;

	// Using range/c-string
    string result;
    regex_replace(back_inserter(result), str.begin(), str.end(), exp, "$2");
    cout << result << endl;

	// With flags
    cout << regex_replace(str, exp, "$1 and $2", regex_constants::format_no_copy) << endl;


    cout << "End" << endl;

    return 0;
}