
#ifndef UTILITY_42OPTIONAL_H
#define UTILITY_42OPTIONAL_H

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
  T *data_;

 public:
  optional();
  explicit optional(T const &val);
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
optional<T>::optional() : data_(NULL) {}

/**
 * @brief constructor with value T
 * @param val value to set
 * @return option with value
 */
template <typename T>
optional<T>::optional(T const &val) : data_(new T(val)) {}

/**
 * @brief copy constructor
 * @param other source to copy
 * @return either empty or not, depends on parameter
 */
template <typename T>
optional<T>::optional(optional<T> const &other) {
  if (other.data_ != NULL) {
    this->data_ = new T(*other.data_);
  } else {
    this->data_ = NULL;
  }
}

/**
 * @brief copy assign operator
 * @param rhs source to copy
 * @return reference of assigned var
 */
template <typename T>
optional<T> &optional<T>::operator=(optional<T> const &rhs) {
  if (this != &rhs) {
    if (this->has_value()) {
      delete this->data_;
      this->data_ = NULL;
    }
    if (rhs.has_value()) {
      this->data_ = new T(*rhs.data_);
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
  delete this->data_;
  this->data_ = NULL;
}

/**
 * @brief getter for has_value_
 * @return true if the option has value
 */
template <typename T>
bool optional<T>::has_value() const {
  return (data_ != NULL);
}

/**
 * @brief getter for the value_
 * @attention UB if it does not have value
 * @return const reference to the value_
 */
template <typename T>
T const &optional<T>::value() const {
  return *data_;
}

/**
 * @brief operator* to have access to the value_
 * @attention UB if it does not have value
 * @return const reference to the value_
 */
template <typename T>
T const &optional<T>::operator*() const {
  return *data_;
}

/**
 * @brief operator* to have access to the value_
 * @attention UB if it does not have value
 * @return reference to the value_
 */
template <typename T>
T &optional<T>::operator*() {
  return *data_;
}

}  // namespace ft

#endif
