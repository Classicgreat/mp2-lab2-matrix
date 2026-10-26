// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Динамический вектор -
// шаблонный вектор на динамической памяти.
//
// [0, 0, 0, *, *, *]
//           |     |
//           v     v
//      start_id   sz
template <typename T> class TDynamicVector {
protected:
  size_t sz;
  size_t start_id = 0;
  T *pMem;

  // лежит ли индекс в хранимой области
  bool in_storage(size_t i) const noexcept {
    return i >= start_id && i < start_id + sz;
  }

public:
  TDynamicVector(size_t size = 1, size_t si = 0) : sz(size), start_id(si) {
    if (sz > MAX_VECTOR_SIZE)
      throw std::out_of_range("Vector size is too large");
    if (sz == 0)
      throw std::out_of_range("Vector size should be greater than zero");
    pMem = new T[sz]();
  }

  TDynamicVector(T *arr, size_t s) : sz(s), start_id(0) {
    assert(arr != nullptr && "TDynamicVector requires non-nullptr arg");
    if (sz > MAX_VECTOR_SIZE)
      throw std::out_of_range("Vector size is too large");
    if (sz == 0)
      throw std::out_of_range("Vector size should be greater than zero");
    pMem = new T[sz];
    std::copy(arr, arr + sz, pMem);
  }

  TDynamicVector(const TDynamicVector &other)
      : sz(other.sz), start_id(other.start_id) {
    pMem = new T[sz];
    std::copy(other.pMem, other.pMem + sz, pMem);
  }

  TDynamicVector(TDynamicVector &&v) noexcept
      : sz(v.sz), start_id(v.start_id), pMem(v.pMem) {
    v.pMem = nullptr;
    v.sz = 0;
    v.start_id = 0;
  }

  ~TDynamicVector() { delete[] pMem; }

  TDynamicVector &operator=(const TDynamicVector &v) {
    if (this != &v) {
      TDynamicVector tmp(v);
      swap(*this, tmp);
    }
    return *this;
  }

  TDynamicVector &operator=(TDynamicVector &&v) noexcept {
    if (this != &v) {
      delete[] pMem;
      pMem = v.pMem;
      sz = v.sz;
      start_id = v.start_id;
      v.pMem = nullptr;
      v.sz = 0;
      v.start_id = 0;
    }
    return *this;
  }

  // логическая длина вектора (с учётом неявных нулей)
  size_t size() const noexcept { return start_id + sz; }

  // сколько элементов реально хранится
  size_t stored_size() const noexcept { return sz; }

  // сколько неявных нулей в начале
  size_t offset() const noexcept { return start_id; }

  T &operator[](size_t i) {
    if (!in_storage(i))
      throw std::out_of_range("Index out of range");
    return pMem[i - start_id];
  }

  const T &operator[](size_t i) const {
    if (!in_storage(i))
      throw std::out_of_range("Index out of range");
    return pMem[i - start_id];
  }

  // индексация с контролем
  T &at(size_t ind) { return (*this)[ind]; }
  const T &at(size_t ind) const { return (*this)[ind]; }

  // значение элемента с учётом неявных нулей (возврат по значению!)
  T get(size_t i) const {
    if (i >= size())
      throw std::out_of_range("Index out of range");
    return (i < start_id) ? T() : pMem[i - start_id];
  }

  // сравнение (по логическим значениям)
  bool operator==(const TDynamicVector &v) const noexcept {
    if (size() != v.size())
      return false;
    for (size_t i = 0; i < size(); ++i)
      if (!(get(i) == v.get(i)))
        return false;
    return true;
  }

  bool operator!=(const TDynamicVector &v) const noexcept {
    return !(*this == v);
  }

  // скалярные операции
  TDynamicVector operator+(const T &val) const {
    size_t n = size();
    TDynamicVector result(n);
    for (size_t i = 0; i < n; ++i)
      result.pMem[i] = get(i) + val;
    return result;
  }

  TDynamicVector operator-(const T &val) const {
    size_t n = size();
    TDynamicVector result(n);
    for (size_t i = 0; i < n; ++i)
      result.pMem[i] = get(i) - val;
    return result;
  }

  TDynamicVector operator*(const T &val) const {
    TDynamicVector result(sz, start_id);
    for (size_t i = 0; i < sz; ++i)
      result.pMem[i] = pMem[i] * val;
    return result;
  }

  // векторные операции
  TDynamicVector operator+(const TDynamicVector &v) const {
    if (size() != v.size())
      throw std::invalid_argument("Vector sizes mismatch");

    size_t n = size();
    size_t s = std::min(start_id, v.start_id);
    TDynamicVector result(n - s, s);
    for (size_t i = s; i < n; ++i)
      result.pMem[i - s] = get(i) + v.get(i);
    return result;
  }

  TDynamicVector operator-(const TDynamicVector &v) const {
    if (size() != v.size())
      throw std::invalid_argument("Vector sizes mismatch");

    size_t n = size();
    size_t s = std::min(start_id, v.start_id);
    TDynamicVector result(n - s, s);
    for (size_t i = s; i < n; ++i)
      result.pMem[i - s] = get(i) - v.get(i);
    return result;
  }

  // скалярное произведение
  T operator*(const TDynamicVector &v) const {
    if (size() != v.size())
      throw std::invalid_argument("Vector sizes mismatch");

    size_t n = size();
    size_t s = std::max(start_id, v.start_id);
    T result{};
    for (size_t i = s; i < n; ++i)
      result += pMem[i - start_id] * v.pMem[i - v.start_id];
    return result;
  }

  friend void swap(TDynamicVector &lhs, TDynamicVector &rhs) noexcept {
    std::swap(lhs.sz, rhs.sz);
    std::swap(lhs.start_id, rhs.start_id);
    std::swap(lhs.pMem, rhs.pMem);
  }

  // ввод/вывод
  friend istream &operator>>(istream &istr, TDynamicVector &v) {
    for (size_t i = 0; i < v.sz; i++)
      istr >> v.pMem[i];
    return istr;
  }

  friend ostream &operator<<(ostream &ostr, const TDynamicVector &v) {
    for (size_t i = 0; i < v.size(); i++)
      ostr << v.get(i) << ' ';
    return ostr;
  }
};

// Динамическая матрица -
// шаблонная матрица на динамической памяти
template <typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>> {
  using TDynamicVector<TDynamicVector<T>>::pMem;
  using TDynamicVector<TDynamicVector<T>>::sz;

public:
  TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s) {
    if (s > MAX_MATRIX_SIZE)
      throw std::out_of_range("Matrix size is too large");
    for (size_t i = 0; i < sz; i++)
      pMem[i] = TDynamicVector<T>(sz);
  }

  TDynamicMatrix(const TDynamicMatrix<T> &m)
      : TDynamicVector<TDynamicVector<T>>(m) {}

  TDynamicMatrix(TDynamicMatrix<T> &&m) noexcept
      : TDynamicVector<TDynamicVector<T>>(std::move(m)) {}

  TDynamicMatrix &operator=(const TDynamicMatrix<T> &m) {
    TDynamicVector<TDynamicVector<T>>::operator=(m);
    return *this;
  }

  TDynamicMatrix &operator=(TDynamicMatrix<T> &&m) noexcept {
    TDynamicVector<TDynamicVector<T>>::operator=(std::move(m));
    return *this;
  }

  using TDynamicVector<TDynamicVector<T>>::at;
  using TDynamicVector<TDynamicVector<T>>::operator[];
  using TDynamicVector<TDynamicVector<T>>::size;

  // сравнение
  bool operator==(const TDynamicMatrix<T> &m) const noexcept {
    if (sz != m.sz)
      return false;
    for (size_t i = 0; i < sz; ++i)
      if (pMem[i] != m.pMem[i])
        return false;
    return true;
  }

  bool operator!=(const TDynamicMatrix<T> &m) const noexcept {
    return !(*this == m);
  }

  // матрично-скалярные операции
  TDynamicMatrix<T> operator*(const T &val) const {
    TDynamicMatrix<T> res(sz);
    for (size_t i = 0; i < sz; ++i)
      res.pMem[i] = pMem[i] * val;
    return res;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T> &v) const {
    if (sz != v.size())
      throw std::invalid_argument("Matrix and vector sizes mismatch");

    TDynamicVector<T> res(sz);
    for (size_t i = 0; i < sz; ++i) {
      T s{};
      for (size_t j = 0; j < sz; ++j)
        s += pMem[i].get(j) * v.get(j);
      res[i] = s;
    }
    return res;
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix<T> &m) const {
    if (sz != m.sz)
      throw std::invalid_argument("Matrix sizes mismatch");

    TDynamicMatrix res(sz);
    for (size_t i = 0; i < sz; ++i)
      res.pMem[i] = pMem[i] + m.pMem[i];
    return res;
  }

  TDynamicMatrix<T> operator-(const TDynamicMatrix<T> &m) const {
    if (sz != m.sz)
      throw std::invalid_argument("Matrix sizes mismatch");

    TDynamicMatrix<T> res(sz);
    for (size_t i = 0; i < sz; ++i)
      res.pMem[i] = pMem[i] - m.pMem[i];
    return res;
  }

  TDynamicMatrix<T> operator*(const TDynamicMatrix<T> &m) const {
    if (sz != m.sz)
      throw std::invalid_argument("Matrix sizes mismatch");

    TDynamicMatrix<T> res(sz);
    for (size_t i = 0; i < sz; ++i)
      for (size_t j = 0; j < sz; ++j) {
        T s{};
        for (size_t k = 0; k < sz; ++k)
          s += pMem[i].get(k) * m.pMem[k].get(j);
        res[i][j] = s;
      }
    return res;
  }

  // ввод/вывод
  friend istream &operator>>(istream &istr, TDynamicMatrix<T> &m) {
    for (size_t i = 0; i < m.sz; ++i)
      istr >> m.pMem[i];
    return istr;
  }

  friend ostream &operator<<(ostream &ostr, const TDynamicMatrix<T> &m) {
    for (size_t i = 0; i < m.sz; ++i) {
      for (size_t j = 0; j < m.pMem[i].size(); ++j)
        ostr << m.pMem[i].get(j) << ' ';
      ostr << '\n';
    }
    return ostr;
  }
};

/*
 Верхнетреугольная матрица

 (* * * *)
 (0 * * *)
 (0 0 * *)
 (0 0 0 *)
*/
template <typename T>
class TDynamicTRMatrix : private TDynamicVector<TDynamicVector<T>> {
  using TDynamicVector<TDynamicVector<T>>::pMem;
  using TDynamicVector<TDynamicVector<T>>::sz;

public:
  TDynamicTRMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s) {
    if (s > MAX_MATRIX_SIZE)
      throw std::out_of_range("Matrix size is too large");
    for (size_t i = 0; i < sz; i++)
      pMem[i] = TDynamicVector<T>(sz - i, i);
  }

  TDynamicTRMatrix(const TDynamicTRMatrix &m)
      : TDynamicVector<TDynamicVector<T>>(m) {}

  TDynamicTRMatrix(TDynamicTRMatrix &&m) noexcept
      : TDynamicVector<TDynamicVector<T>>(std::move(m)) {}

  TDynamicTRMatrix &operator=(const TDynamicTRMatrix<T> &m) {
    TDynamicVector<TDynamicVector<T>>::operator=(m);
    return *this;
  }

  TDynamicTRMatrix &operator=(TDynamicTRMatrix<T> &&m) noexcept {
    TDynamicVector<TDynamicVector<T>>::operator=(std::move(m));
    return *this;
  }

  using TDynamicVector<TDynamicVector<T>>::at;
  using TDynamicVector<TDynamicVector<T>>::operator[];
  using TDynamicVector<TDynamicVector<T>>::size;

  // сравнение
  bool operator==(const TDynamicTRMatrix &m) const noexcept {
    if (sz != m.sz)
      return false;
    for (size_t i = 0; i < sz; ++i)
      if (pMem[i] != m.pMem[i])
        return false;
    return true;
  }

  bool operator!=(const TDynamicTRMatrix &m) const noexcept {
    return !(*this == m);
  }

  // матрично-скалярные операции
  TDynamicTRMatrix<T> operator*(const T &val) const {
    TDynamicTRMatrix<T> res(sz);
    for (size_t i = 0; i < sz; ++i)
      res.pMem[i] = pMem[i] * val;
    return res;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T> &v) const {
    if (sz != v.size())
      throw std::invalid_argument("Matrix and vector sizes mismatch");

    TDynamicVector<T> res(sz);
    for (size_t i = 0; i < sz; ++i) {
      T s{};
      for (size_t j = i; j < sz; ++j)
        s += pMem[i].get(j) * v.get(j);
      res[i] = s;
    }
    return res;
  }

  // матрично-матричные операции
  TDynamicTRMatrix operator+(const TDynamicTRMatrix &m) const {
    if (sz != m.sz)
      throw std::invalid_argument("Matrix sizes mismatch");

    TDynamicTRMatrix res(sz);
    for (size_t i = 0; i < sz; ++i)
      res.pMem[i] = pMem[i] + m.pMem[i];
    return res;
  }

  TDynamicTRMatrix operator-(const TDynamicTRMatrix &m) const {
    if (sz != m.sz)
      throw std::invalid_argument("Matrix sizes mismatch");

    TDynamicTRMatrix res(sz);
    for (size_t i = 0; i < sz; ++i)
      res.pMem[i] = pMem[i] - m.pMem[i];
    return res;
  }

  TDynamicTRMatrix operator*(const TDynamicTRMatrix &m) const {
    if (sz != m.sz)
      throw std::invalid_argument("Matrix sizes mismatch");

    TDynamicTRMatrix res(sz);
    for (size_t i = 0; i < sz; ++i)
      for (size_t j = i; j < sz; ++j) {
        T s{};
        for (size_t k = i; k <= j; ++k)
          s += pMem[i].get(k) * m.pMem[k].get(j);
        res.pMem[i][j] = s;
      }
    return res;
  }

  friend istream &operator>>(istream &istr, TDynamicTRMatrix &m) {
    T val;
    for (size_t i = 0; i < m.sz; ++i)
      for (size_t j = 0; j < m.sz; ++j) {
        istr >> val;
        if (j >= i)
          m.pMem[i][j] = val;
      }
    return istr;
  }

  friend ostream &operator<<(ostream &ostr, const TDynamicTRMatrix &m) {
    for (size_t i = 0; i < m.sz; ++i) {
      for (size_t j = 0; j < m.sz; ++j)
        ostr << m.pMem[i].get(j) << ' ';
      ostr << '\n';
    }
    return ostr;
  }
};

#endif
