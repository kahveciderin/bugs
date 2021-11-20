#pragma once

#include <vector>

template <typename T>
std::vector<T> operator+(const std::vector<T>& A, const std::vector<T>& B);
template <typename T>
std::vector<T>& operator+=(std::vector<T>& A, const std::vector<T>& B);