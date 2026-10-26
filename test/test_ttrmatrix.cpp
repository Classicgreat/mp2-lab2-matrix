#include "../include/tmatrix.h"

#include <../gtest/gtest.h>

TEST(TDynamicTRMatrix, can_create_matrix_with_positive_length) {
  ASSERT_NO_THROW(TDynamicTRMatrix<int> m(5));
}

TEST(TDynamicTRMatrix, cant_create_too_large_matrix) {
  ASSERT_ANY_THROW(TDynamicTRMatrix<int> m(MAX_MATRIX_SIZE + 1));
}

TEST(TDynamicTRMatrix, throws_when_create_matrix_with_negative_length) {
  ASSERT_ANY_THROW(TDynamicTRMatrix<int> m(-5));
}

TEST(TDynamicTRMatrix, can_create_copied_matrix) {
  TDynamicTRMatrix<int> m(5);

  ASSERT_NO_THROW(TDynamicTRMatrix<int> m1(m));
}

TEST(TDynamicTRMatrix, copied_matrix_is_equal_to_source_one) {
  TDynamicTRMatrix<int> m(5);
  for (size_t i = 0; i < m.size(); ++i)
    for (size_t j = i; j < m.size(); ++j)
      m[i][j] = static_cast<int>(i * m.size() + j);

  TDynamicTRMatrix<int> m1(m);
  EXPECT_EQ(m, m1);
}

TEST(TDynamicTRMatrix, copied_matrix_has_its_own_memory) {
  TDynamicTRMatrix<int> m(5);
  m[0][0] = 42;

  TDynamicTRMatrix<int> m1(m);
  m1[0][0] = 7;

  EXPECT_EQ(m[0][0], 42);
  EXPECT_EQ(m1[0][0], 7);
  EXPECT_NE(&m[0][0], &m1[0][0]);
}

TEST(TDynamicTRMatrix, can_get_size) {
  TDynamicTRMatrix<int> m(5);
  EXPECT_EQ(m.size(), static_cast<size_t>(5));
}

TEST(TDynamicTRMatrix, can_set_and_get_element) {
  TDynamicTRMatrix<int> m(5);
  m[2][3] = 42;
  EXPECT_EQ(m[2][3], 42);
}

TEST(TDynamicTRMatrix, throws_when_set_element_with_negative_index) {
  TDynamicTRMatrix<int> m(5);
  EXPECT_ANY_THROW(m.at(static_cast<size_t>(-1)));
}

TEST(TDynamicTRMatrix, throws_when_set_element_with_too_large_index) {
  TDynamicTRMatrix<int> m(5);
  EXPECT_ANY_THROW(m.at(5));
}

TEST(TDynamicTRMatrix, can_assign_matrix_to_itself) {
  TDynamicTRMatrix<int> m(5);
  m[0][0] = 42;

  m = m;

  EXPECT_EQ(m.size(), static_cast<size_t>(5));
  EXPECT_EQ(m[0][0], 42);
}

TEST(TDynamicTRMatrix, can_assign_matrices_of_equal_size) {
  TDynamicTRMatrix<int> m1(5), m2(5);
  m1[0][0] = 1;
  m2[0][0] = 2;

  m1 = m2;

  EXPECT_EQ(m1.size(), static_cast<size_t>(5));
  EXPECT_EQ(m1[0][0], 2);
}

TEST(TDynamicTRMatrix, assign_operator_change_matrix_size) {
  TDynamicTRMatrix<int> m1(5), m2(3);

  m1 = m2;

  EXPECT_EQ(m1.size(), static_cast<size_t>(3));
}

TEST(TDynamicTRMatrix, can_assign_matrices_of_different_size) {
  TDynamicTRMatrix<int> m1(5), m2(3);
  m1[0][0] = 1;
  m2[0][0] = 2;
  m2[2][2] = 9;

  m1 = m2;

  EXPECT_EQ(m1.size(), static_cast<size_t>(3));
  EXPECT_EQ(m1[0][0], 2);
  EXPECT_EQ(m1[2][2], 9);
}

TEST(TDynamicTRMatrix, compare_equal_matrices_return_true) {
  TDynamicTRMatrix<int> m1(5), m2(5);
  for (size_t i = 0; i < m1.size(); ++i)
    for (size_t j = i; j < m1.size(); ++j) {
      int v = static_cast<int>(i * m1.size() + j);
      m1[i][j] = v;
      m2[i][j] = v;
    }

  EXPECT_TRUE(m1 == m2);
}

TEST(TDynamicTRMatrix, compare_matrix_with_itself_return_true) {
  TDynamicTRMatrix<int> m(5);
  EXPECT_TRUE(m == m);
}

TEST(TDynamicTRMatrix, matrices_with_different_size_are_not_equal) {
  TDynamicTRMatrix<int> m1(5), m2(3);
  EXPECT_FALSE(m1 == m2);
}

TEST(TDynamicTRMatrix, can_add_matrices_with_equal_size) {
  TDynamicTRMatrix<int> m1(5), m2(5);
  for (size_t i = 0; i < m1.size(); ++i)
    for (size_t j = i; j < m1.size(); ++j) {
      m1[i][j] = static_cast<int>(i + j);
      m2[i][j] = static_cast<int>(i - j);
    }

  TDynamicTRMatrix<int> m3 = m1 + m2;

  for (size_t i = 0; i < m3.size(); ++i)
    for (size_t j = i; j < m3.size(); ++j)
      EXPECT_EQ(m3[i][j], m1[i][j] + m2[i][j]);
}

TEST(TDynamicTRMatrix, cant_add_matrices_with_not_equal_size) {
  TDynamicTRMatrix<int> m1(5), m2(3);
  EXPECT_ANY_THROW(m1 + m2);
}

TEST(TDynamicTRMatrix, can_subtract_matrices_with_equal_size) {
  TDynamicTRMatrix<int> m1(5), m2(5);
  for (size_t i = 0; i < m1.size(); ++i)
    for (size_t j = i; j < m1.size(); ++j) {
      m1[i][j] = static_cast<int>(i * 3 + j);
      m2[i][j] = static_cast<int>(i + j);
    }

  TDynamicTRMatrix<int> m3 = m1 - m2;

  for (size_t i = 0; i < m3.size(); ++i)
    for (size_t j = i; j < m3.size(); ++j)
      EXPECT_EQ(m3[i][j], m1[i][j] - m2[i][j]);
}

TEST(TDynamicTRMatrix, cant_subtract_matrixes_with_not_equal_size) {
  TDynamicTRMatrix<int> m1(5), m2(3);
  EXPECT_ANY_THROW(m1 - m2);
}
