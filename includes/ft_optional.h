
#ifndef UTILITY_42OPTIONAL_H
#define UTILITY_42OPTIONAL_H

#include <cstring>
#include <iostream>

namespace ft {

/**
 *
 * @brief replica of std::optional in C++17
 * @author eldeshue
 * @date 2024-12-20
 *
 */
template <typename T>
class optional {
 private:
  bool has_value_;
  union {
    char none_;
    T value_;
  };

 public:
  optional();
  optional(T const &val);
  optional(optional<T> const &other);
  optional<T> &operator=(optional<T> const &rhs);
  ~optional();

  bool has_value() const;
  T const &value() const;

  T const &operator*() const;
  T &operator*();
};

/**
 * @brief default constructor, no value
 * @return std::no_ops like empty option
 */
template <typename T>
optional<T>::optional() : has_value_(false), none_(0) {
  std::memset(&none_, 0, sizeof(T));
};

/**
 * @brief constructor with value T
 * @param val value to set
 * @return option with value
 */
template <typename T>
optional<T>::optional(T const &val) : has_value_(true), none_(1) {
  std::memset(&none_, 0, sizeof(T));
  value_ = val;
};

/**
 * @brief copy constructor
 * @param other source to copy
 * @return either empty or not, depends on parameter
 */
template <typename T>
optional<T>::optional(optional<T> const &other)
    : has_value_(other.has_value_), none_(0) {
  std::memset(&none_, 0, sizeof(T));
  value_ = other.value_;
};

/**
 * @brief copy assign operator
 * @param rhs source to copy
 * @return reference of assigned var
 */
template <typename T>
optional<T> &optional<T>::operator=(optional<T> const &rhs) {
  if (this != &rhs) {
    has_value_ = rhs.has_value_;
    if (has_value_) {
      value_ = rhs.value_;
    } else {
      none_ = rhs.none_;
    }
  }
  return *this;
}

/**
 * @brief destructor
 * @attention value_ is part of union, destructor of value_ must be called \n
  explicitly.
 */
template <typename T>
optional<T>::~optional() {
  if (has_value_) value_.~T();
};

/**
 * @brief getter for has_value_
 * @return true if the option has value
 */
template <typename T>
bool optional<T>::has_value() const {
  return has_value_;
};

/**
 * @brief getter for the value_
 * @attention UB if it does not have value
 * @return const reference to the value_
 */
template <typename T>
T const &optional<T>::value() const {
  return value_;
};

/**
 * @brief operator* to have access to the value_
 * @attention UB if it does not have value
 * @return const reference to the value_
 */
template <typename T>
T const &optional<T>::operator*() const {
  return value_;
};

/**
 * @brief operator* to have access to the value_
 * @attention UB if it does not have value
 * @return reference to the value_
 */
template <typename T>
T &optional<T>::operator*() {
  return value_;
};

}  // namespace ft

#endif
