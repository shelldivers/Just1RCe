
#include <iostream>
#include <string>
#include <vector>

#include "../includes/ft_optional.h"

int main() {
  ft::optional<std::string> test("test construction");
  std::cout << "Dose test has value ? : " << std::boolalpha << test.has_value()
            << '\n';
  std::cout << "the value is : " << test.value() << '\n';

  std::cout << "Before initializing test1.\n";
  ft::optional<std::string> test1;
  std::cout << "Dose test1 has value ? : " << std::boolalpha
            << test1.has_value() << '\n';

  std::cout << "After initializing.\n";
  test1 = std::string("hello world");
  std::cout << "Dose test1 has value ? : " << std::boolalpha
            << test1.has_value() << '\n';
  std::cout << "the value is : " << test1.value() << '\n';
  std::cout << "the value is : " << *test1 << '\n';

  std::cout << "Const ref of test1\n";
  const ft::optional<std::string> &ref_test1 = test1;
  std::cout << "the value is : " << ref_test1.value() << '\n';
  std::cout << "the value is : " << *ref_test1 << '\n';

  std::cout << "ref of test1\n";
  std::cout << "the value is : " << ref_test1.value() << '\n';
  std::cout << "the value is : " << *ref_test1 << '\n';

  std::cout << "mutability test\n";
  *test1 = "goodbye world";
  std::cout << "the value is : " << test1.value() << '\n';
  std::cout << "the value is : " << *test1 << '\n';

  // how about primal type, no destructor
  std::cout << "initializing test2.\n";
  ft::optional<int> test2(0);
  std::cout << "Dose test2 has value ? : " << std::boolalpha
            << test1.has_value() << '\n';
  std::cout << "the value is : " << test2.value() << '\n';

  // optional vector string
  std::cout << "optional + vector + string tests\n";
  ft::optional<std::vector<std::string> > test3((std::vector<std::string>()));
  (*test3).push_back("hello\n");
  std::cout << (*test3)[0];
}
