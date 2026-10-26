#pragma once
#include "tvector.h"

template <typename T> class TMatrix : public TVector<TVector<T>> {
public:
  TMatrix(int n) : TVector<TVector<T>>(n) {
    for (int i = 0; i < n; ++i) {
      this->pMem[i] = TVector<T>(n);
    }
  }
};
