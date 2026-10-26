#include "../include/tmatrix.h"

#include <../gtest/gtest.h>

TEST(TDynamicMatrix, can_create_matrix_with_positive_length) {
  ASSERT_NO_THROW(TDynamicMatrix<int> m(5));
}

TEST(TDynamicMatrix, cant_create_too_large_matrix) {
  ASSERT_ANY_THROW(TDynamicMatrix<int> m(MAX_MATRIX_SIZE + 1));
}

TEST(TDynamicMatrix, throws_when_create_matrix_with_negative_length) {
  ASSERT_ANY_THROW(TDynamicMatrix<int> m(-5));
}

TEST(TDynamicMatrix, can_create_copied_matrix) {
  TDynamicMatrix<int> m(5);

  ASSERT_NO_THROW(TDynamicMatrix<int> m1(m));
}

TEST(TDynamicMatrix, copied_matrix_is_equal_to_source_one) {
  TDynamicMatrix<int> m(5);
  for (int i = 0; i < m.size(); ++i)
    for (int j = 0; j < m.size(); ++j)
      m[i][j] = i * m.size() + j;

  TDynamicMatrix<int> m1(m);
  EXPECT_EQ(m, m1);
}

TEST(TDynamicMatrix, copied_matrix_has_its_own_memory) {
  TDynamicMatrix<int> m(5);
  m[0][0] = 42;

  TDynamicMatrix<int> m1(m);
  m1[0][0] = 7;

  EXPECT_EQ(m[0][0], 42);
  EXPECT_EQ(m1[0][0], 7);
  EXPECT_NE(&m[0][0], &m1[0][0]);
}

TEST(TDynamicMatrix, can_get_size) {
  TDynamicMatrix<int> m(5);
  EXPECT_EQ(m.size(), static_cast<size_t>(5));
}

TEST(TDynamicMatrix, can_set_and_get_element) {
  TDynamicMatrix<int> m(5);
  m[2][3] = 42;
  EXPECT_EQ(m[2][3], 42);
}

TEST(TDynamicMatrix, throws_when_set_element_with_negative_index) {
  TDynamicMatrix<int> m(5);
  EXPECT_ANY_THROW(m.at(static_cast<size_t>(-1)));
}

TEST(TDynamicMatrix, throws_when_set_element_with_too_large_index) {
  TDynamicMatrix<int> m(5);
  EXPECT_ANY_THROW(m.at(5));
}

TEST(TDynamicMatrix, can_assign_matrix_to_itself) {
  TDynamicMatrix<int> m(5);
  m[0][0] = 42;

  m = m;

  EXPECT_EQ(m.size(), static_cast<size_t>(5));
  EXPECT_EQ(m[0][0], 42);
}

TEST(TDynamicMatrix, can_assign_matrices_of_equal_size) {
  TDynamicMatrix<int> m1(5), m2(5);
  m1[0][0] = 1;
  m2[0][0] = 2;

  m1 = m2;

  EXPECT_EQ(m1.size(), static_cast<size_t>(5));
  EXPECT_EQ(m1[0][0], 2);
}

TEST(TDynamicMatrix, assign_operator_change_matrix_size) {
  TDynamicMatrix<int> m1(5), m2(3);

  m1 = m2;

  EXPECT_EQ(m1.size(), static_cast<size_t>(3));
}

TEST(TDynamicMatrix, can_assign_matrices_of_different_size) {
  TDynamicMatrix<int> m1(5), m2(3);
  m1[0][0] = 1;
  m2[0][0] = 2;
  m2[2][2] = 9;

  m1 = m2;

  EXPECT_EQ(m1.size(), static_cast<size_t>(3));
  EXPECT_EQ(m1[0][0], 2);
  EXPECT_EQ(m1[2][2], 9);
}

TEST(TDynamicMatrix, compare_equal_matrices_return_true) {
  TDynamicMatrix<int> m1(5), m2(5);
  for (size_t i = 0; i < m1.size(); ++i)
    for (size_t j = 0; j < m1.size(); ++j) {
      int v = static_cast<int>(i * m1.size() + j);
      m1[i][j] = v;
      m2[i][j] = v;
    }

  EXPECT_TRUE(m1 == m2);
}

TEST(TDynamicMatrix, compare_matrix_with_itself_return_true) {
  TDynamicMatrix<int> m(5);
  for (size_t i = 0; i < m.size(); ++i)
    for (size_t j = 0; j < m.size(); ++j) {
      m[i][j] = static_cast<int>(i + j);
    }
  EXPECT_TRUE(m == m);
}

TEST(TDynamicMatrix, matrices_with_different_size_are_not_equal) {
  TDynamicMatrix<int> m1(5), m2(3);
  EXPECT_FALSE(m1 == m2);
}

TEST(TDynamicMatrix, can_add_matrices_with_equal_size) {
  TDynamicMatrix<int> m1(5), m2(5);
  for (size_t i = 0; i < m1.size(); ++i)
    for (size_t j = 0; j < m1.size(); ++j) {
      m1[i][j] = static_cast<int>(i + j);
      m2[i][j] = static_cast<int>(i - j);
    }

  TDynamicMatrix<int> m3 = m1 + m2;

  for (size_t i = 0; i < m3.size(); ++i)
    for (size_t j = 0; j < m3.size(); ++j)
      EXPECT_EQ(m3[i][j], m1[i][j] + m2[i][j]);
}

TEST(TDynamicMatrix, cant_add_matrices_with_not_equal_size) {
  TDynamicMatrix<int> m1(5), m2(3);
  EXPECT_ANY_THROW(m1 + m2);
}

TEST(TDynamicMatrix, can_subtract_matrices_with_equal_size) {
  TDynamicMatrix<int> m1(5), m2(5), true_res(5);
  for (size_t i = 0; i < m1.size(); ++i)
    for (size_t j = 0; j < m1.size(); ++j) {
      m1[i][j] = static_cast<int>(i * 3 + j);
      m2[i][j] = static_cast<int>(i + j);
      true_res[i][j] = static_cast<int>(i * 2);
    }

  TDynamicMatrix<int> m3 = m1 - m2;

  for (size_t i = 0; i < m3.size(); ++i)
    for (size_t j = 0; j < m3.size(); ++j)
      EXPECT_EQ(m3[i][j], m1[i][j] - m2[i][j]);
  EXPECT_EQ(m3, true_res);
}

TEST(TDynamicMatrix, cant_subtract_matrixes_with_not_equal_size) {
  TDynamicMatrix<int> m1(5), m2(3);
  EXPECT_ANY_THROW(m1 - m2);
}
