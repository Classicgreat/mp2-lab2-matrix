#pragma once

template <typename T> class TVector {
protected:
  T *pMem = nullptr;
  int size = 0;

public:
  TVector(int n = 0) : pMem(n ? new T[n]() : nullptr), size(n) {}

  TVector(const TVector &other) : pMem(new T[other.size]), size(other.size) {
    for (int i = 0; i < size; ++i) {
      pMem[i] = other.pMem[i];
    }
  }

  TVector &operator=(const TVector &other) {
    if (this == &other) {
      return *this;
    }

    size = other.size;
    delete[] pMem;
    pMem = new T[size];
    for (int i = 0; i < size; ++i) {
      pMem[i] = other.pMem[i];
    }
    return *this;
  }

  TVector(TVector &&other) noexcept : pMem(other.pMem), size(other.size) {
    other.pMem = nullptr;
    other.size = 0;
  }

  TVector &operator=(TVector &&other) noexcept {
    if (this != &other) {
      delete[] pMem;
      pMem = other.pMem;
      size = other.size;
      other.pMem = nullptr;
      other.size = 0;
    }
    return *this;
  }

  ~TVector() { delete[] pMem; }

  T &operator[](int i) { return pMem[i]; }
  const T &operator[](int i) const { return pMem[i]; }

  int Size() const { return size; }
};
