#include "../include/tmatrix.h"

#include <gtest/gtest.h>

TEST(TDynamicVector, can_create_vector_with_positive_length) {
  ASSERT_NO_THROW(TDynamicVector<int> v(5));
}

TEST(TDynamicVector, cant_create_too_large_vector) {
  ASSERT_ANY_THROW(TDynamicVector<int> v(MAX_VECTOR_SIZE + 1));
}

TEST(TDynamicVector, throws_when_create_vector_with_negative_length) {
  ASSERT_ANY_THROW(TDynamicVector<int> v(-5));
}

TEST(TDynamicVector, can_create_copied_vector) {
  TDynamicVector<int> v(10);

  ASSERT_NO_THROW(TDynamicVector<int> v1(v));
}

TEST(TDynamicVector, copied_vector_is_equal_to_source_one) {
  TDynamicVector<int> v1(10);
  for (int i = 0; i < v1.size(); ++i) {
    v1[i] = i;
  }
  TDynamicVector<int> v2(v1);

  EXPECT_EQ(v1, v2);
  EXPECT_EQ(v2.size(), v1.size());
}

TEST(TDynamicVector, copied_vector_has_its_own_memory) {
  TDynamicVector<int> v1(10);
  TDynamicVector<int> v2(v1);
  EXPECT_NE(&v1, &v2);
}

TEST(TDynamicVector, can_get_size) {
  TDynamicVector<int> v(4);

  EXPECT_EQ(4, v.size());
}

TEST(TDynamicVector, can_set_and_get_element) {
  TDynamicVector<int> v(4);
  v[0] = 4;

  EXPECT_EQ(4, v[0]);
}

TEST(TDynamicVector, throws_when_set_element_with_negative_index) {
  TDynamicVector<int> v(10);
  ASSERT_ANY_THROW(v[-1] = 1);
}

TEST(TDynamicVector, throws_when_set_element_with_too_large_index) {
  TDynamicVector<int> v(10);
  ASSERT_ANY_THROW(v[20] = 1);
}

TEST(TDynamicVector, can_assign_vector_to_itself) {
  TDynamicVector<int> v(10);
  ASSERT_NO_THROW(v = v);
}

TEST(TDynamicVector, can_assign_vectors_of_equal_size) {
  TDynamicVector<int> v1(10);
  TDynamicVector<int> v2(10);
  ASSERT_NO_THROW(v1 = v2);
}

TEST(TDynamicVector, assign_operator_change_vector_size) {
  TDynamicVector<int> v1(10);
  TDynamicVector<int> v2(20);
  v1 = v2;
  EXPECT_EQ(v1.size(), 20);
}

TEST(TDynamicVector, can_assign_vectors_of_different_size) {
  TDynamicVector<int> v1(2);
  TDynamicVector<int> v2(4);
  for (int i = 0; i < v2.size(); ++i) {
    v2[i] = i;
  }

  ASSERT_NO_THROW(v1 = v2);
  EXPECT_EQ(v2, v1);
}

TEST(TDynamicVector, compare_equal_vectors_return_true) {
  TDynamicVector<int> v1(3);
  TDynamicVector<int> v2(3);
  for (int i = 0; i < v1.size(); ++i) {
    v1[i] = i;
    v2[i] = i;
  }

  EXPECT_TRUE(v1 == v2);
}

TEST(TDynamicVector, compare_vector_with_itself_return_true) {
  TDynamicVector<int> v(3);
  for (int i = 0; i < v.size(); ++i) {
    v[i] = i + 1;
  }

  EXPECT_TRUE(v == v);
}

TEST(TDynamicVector, vectors_with_different_size_are_not_equal) {
  TDynamicVector<int> v1(2);
  TDynamicVector<int> v2(3);

  EXPECT_FALSE(v1 == v2);
}

TEST(TDynamicVector, can_add_scalar_to_vector) {
  TDynamicVector<int> v(3);
  v[0] = 1;
  v[1] = 2;
  v[2] = 3;

  TDynamicVector<int> result = v + 10;

  TDynamicVector<int> expected(3);
  expected[0] = 11;
  expected[1] = 12;
  expected[2] = 13;

  EXPECT_EQ(expected, result);
}

TEST(TDynamicVector, can_subtract_scalar_from_vector) {
  TDynamicVector<int> v(3);
  v[0] = 1;
  v[1] = 2;
  v[2] = 3;

  TDynamicVector<int> result = v - 1;

  TDynamicVector<int> expected(3);
  expected[0] = 0;
  expected[1] = 1;
  expected[2] = 2;

  EXPECT_EQ(expected, result);
}

TEST(TDynamicVector, can_multiply_scalar_by_vector) {
  TDynamicVector<int> v(3);
  v[0] = 1;
  v[1] = 2;
  v[2] = 3;

  TDynamicVector<int> result = v * 2;

  TDynamicVector<int> expected(3);
  expected[0] = 2;
  expected[1] = 4;
  expected[2] = 6;

  EXPECT_EQ(expected, result);
}

TEST(TDynamicVector, can_add_vectors_with_equal_size) {
  TDynamicVector<int> v1(3);
  TDynamicVector<int> v2(3);

  v1[0] = 1;
  v1[1] = 2;
  v1[2] = 3;
  v2[0] = 4;
  v2[1] = 5;
  v2[2] = 6;

  TDynamicVector<int> result = v1 + v2;

  TDynamicVector<int> expected(3);
  expected[0] = 5;
  expected[1] = 7;
  expected[2] = 9;

  EXPECT_EQ(expected, result);
}

TEST(TDynamicVector, cant_add_vectors_with_not_equal_size) {
  TDynamicVector<int> v1(2);
  TDynamicVector<int> v2(3);

  ASSERT_ANY_THROW(v1 + v2);
}

TEST(TDynamicVector, can_subtract_vectors_with_equal_size) {
  TDynamicVector<int> v1(3);
  TDynamicVector<int> v2(3);

  v1[0] = 4;
  v1[1] = 5;
  v1[2] = 6;
  v2[0] = 1;
  v2[1] = 2;
  v2[2] = 3;

  TDynamicVector<int> result = v1 - v2;

  TDynamicVector<int> expected(3);
  expected[0] = 3;
  expected[1] = 3;
  expected[2] = 3;

  EXPECT_EQ(expected, result);
}

TEST(TDynamicVector, cant_subtract_vectors_with_not_equal_size) {
  TDynamicVector<int> v1(2);
  TDynamicVector<int> v2(3);

  ASSERT_ANY_THROW(v1 - v2);
}

TEST(TDynamicVector, can_multiply_vectors_with_equal_size) {
  TDynamicVector<int> v1(3);
  TDynamicVector<int> v2(3);

  v1[0] = 1;
  v1[1] = 2;
  v1[2] = 3;
  v2[0] = 4;
  v2[1] = 5;
  v2[2] = 6;

  EXPECT_EQ(32, v1 * v2);
}

TEST(TDynamicVector, cant_multiply_vectors_with_not_equal_size) {
  TDynamicVector<int> v1(2);
  TDynamicVector<int> v2(3);

  ASSERT_ANY_THROW(v1 * v2);
}
