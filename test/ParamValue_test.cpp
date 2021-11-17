#include <gtest/gtest.h>

#include <sstream>
#include <iostream>

#include <tdl/ParamValue.h>


TEST(ParamValue, size_of) {
  // Just as a sanity check, the size of ParamValue should be exactly 16 bytes
  // on a 64 bit system:
  // - 1 byte for the data type
  // - 7 byte padding
  // - 8 bytes for the actual data / pointers to data
  EXPECT_EQ(sizeof(tdl::ParamValue), 16);
}

TEST(ParamValue_construction, default_construction) {
  auto dv_ptr = new tdl::ParamValue;
  EXPECT_NE(dv_ptr, nullptr);
  delete dv_ptr;
}

TEST(ParamValue, ctor_long_double) {
  long double x = -3.4L;
  tdl::ParamValue d(x);
  // Note: The implementation uses typedef double (as opposed to float, double, long double.)
  EXPECT_FLOAT_EQ((double)d, -3.4L);
}

TEST(ParamValue, ctor_double) {
  double x = -3.0;
  tdl::ParamValue d(x);
  // Note: The implementation uses typedef double (as opposed to float, double, long double.)
  EXPECT_FLOAT_EQ((double)d, -3.0);
}

TEST(ParamValue, ctor_float) {
  float x = 3.0f;
  tdl::ParamValue d(x);
  // Note: The implementation uses typedef double (as opposed to float, double, long double.)
  EXPECT_FLOAT_EQ((double)d, 3.0);
}

TEST(ParamValue, ctor_short_int) {
  short int n = -3000;
  tdl::ParamValue d(n);
  EXPECT_EQ((short int)d, -3000);
}

TEST(ParamValue, ctor_unsigend_short_int) {
  unsigned short int n = 3000u;
  tdl::ParamValue d(n);
  EXPECT_EQ((unsigned short)d, 3000u);
}

TEST(ParamValue, ctor_int) {
  int n = -3000;
  tdl::ParamValue d(n);
  EXPECT_EQ((int)d, -3000);
}

TEST(ParamValue, ctor_unsigned) {
  unsigned int n = 3000u;
  tdl::ParamValue d(n);
  EXPECT_EQ((unsigned int)d, 3000u);
}

TEST(ParamValue, ctor_long_int) {
  long int n = -3000;
  tdl::ParamValue d(n);
  EXPECT_EQ((long int)d, -3000);
}

TEST(ParamValue, ctor_unsigned_long) {
  unsigned long int n = 3000u;
  tdl::ParamValue d(n);
  EXPECT_EQ((unsigned long int)d, 3000u);
}

TEST(ParamValue, ctor_long_long) {
  long long n = -3000;
  tdl::ParamValue d(n);
  EXPECT_EQ((long long) d, -3000);
}

TEST(ParamValue, ctor_unsigned_long_long) {
  unsigned long long n = 3000;
  tdl::ParamValue d(n);
  EXPECT_EQ((unsigned long long) d, 3000);
}

TEST(ParamValue, ctor_const_char_ptr) {
  const char* s = "test char";
  tdl::ParamValue d(s);
  EXPECT_EQ((std::string)d, "test char");
}

TEST(ParamValue, ctor_const_std_string) {
  std::string s = "test string";
  tdl::ParamValue d(s);
  EXPECT_EQ(d, "test string");
}

TEST(ParamValue, ctor_const_vector_string) {
  std::vector<std::string> sl = {"test string", "test string 2"};
  tdl::ParamValue d(sl);
  EXPECT_TRUE(d == sl);
}

TEST(ParamValue, ctor_const_vector_int) {
  std::vector<int> il = {1, 2};
  tdl::ParamValue d(il);
  EXPECT_TRUE(d == il);
}

TEST(ParamValue, ctor_const_vector_double) {
  std::vector<double> dl = {1.2, 22.3333};
  tdl::ParamValue d(dl);
  EXPECT_TRUE(d == dl);
}
TEST(ParamValue, ctor_copy) {
  tdl::ParamValue p1((double) 1.23);
  tdl::ParamValue p3((float) 1.23);
  tdl::ParamValue p4((int) -3);
  tdl::ParamValue p5((unsigned int) 123);
  tdl::ParamValue p6("test char");
  tdl::ParamValue p7(std::string("test string"));
  tdl::ParamValue p8({"test string","string2","last string"});
  tdl::ParamValue p9;
  tdl::ParamValue p10(std::vector<int>{1,2,3,4,5});
  tdl::ParamValue p11(std::vector<double>{1.2,2.3,3.4});
  tdl::ParamValue copy_of_p1(p1);
  tdl::ParamValue copy_of_p3(p3);
  tdl::ParamValue copy_of_p4(p4);
  tdl::ParamValue copy_of_p5(p5);
  tdl::ParamValue copy_of_p6(p6);
  tdl::ParamValue copy_of_p7(p7);
  tdl::ParamValue copy_of_p8(p8);
  tdl::ParamValue copy_of_p9(p9);
  tdl::ParamValue copy_of_p10(p10);
  tdl::ParamValue copy_of_p11(p11);
  EXPECT_FLOAT_EQ( (double) copy_of_p1, 1.23);
  EXPECT_FLOAT_EQ( (float) copy_of_p3, 1.23);
  EXPECT_EQ( (int) copy_of_p4, -3);
  EXPECT_EQ( (unsigned int) copy_of_p5, 123);
  EXPECT_EQ( (std::string) copy_of_p6, "test char");
  EXPECT_EQ( (std::string) copy_of_p7, "test string");
  EXPECT_TRUE( copy_of_p8 == (std::vector<std::string>{"test string", "string2", "last string"}));
  EXPECT_TRUE( (copy_of_p9.isEmpty()));
  EXPECT_TRUE( copy_of_p10 == (std::vector<int>{1, 2, 3, 4, 5}));
  EXPECT_TRUE( copy_of_p11 == (std::vector<double>{1.2,2.3,3.4}));
}

TEST(ParamValue, ctor_move) {
  // Ensure that ParamValue has a no-except move constructor (otherwise
  // std::vector is inefficient and will copy instead of move).
  EXPECT_TRUE(noexcept(tdl::ParamValue(std::declval<tdl::ParamValue&&>())));

  tdl::ParamValue empty;
  tdl::ParamValue p1((double) 1.23);
  tdl::ParamValue p3((float) 1.23);
  tdl::ParamValue p4((int) -3);
  tdl::ParamValue p5((unsigned int) 123);
  tdl::ParamValue p6("test char");
  tdl::ParamValue p7(std::string("test string"));
  tdl::ParamValue p8({"test string","string2","last string"});
  tdl::ParamValue p9;
  tdl::ParamValue p10(std::vector<int>{1,2,3,4,5});
  tdl::ParamValue p11(std::vector<double>{1.2,2.3,3.4});
  tdl::ParamValue copy_of_p1(std::move(p1));
  tdl::ParamValue copy_of_p3(std::move(p3));
  tdl::ParamValue copy_of_p4(std::move(p4));
  tdl::ParamValue copy_of_p5(std::move(p5));
  tdl::ParamValue copy_of_p6(std::move(p6));
  tdl::ParamValue copy_of_p7(std::move(p7));
  tdl::ParamValue copy_of_p8(std::move(p8));
  tdl::ParamValue copy_of_p9(std::move(p9));
  tdl::ParamValue copy_of_p10(std::move(p10));
  tdl::ParamValue copy_of_p11(std::move(p11));
  EXPECT_FLOAT_EQ( (double) copy_of_p1, 1.23);
  EXPECT_FLOAT_EQ( (float) copy_of_p3, 1.23);
  EXPECT_EQ( (int) copy_of_p4, -3);
  EXPECT_EQ( (unsigned int) copy_of_p5, 123);
  EXPECT_EQ( (std::string) copy_of_p6, "test char");
  EXPECT_EQ( (std::string) copy_of_p7, "test string");
  EXPECT_TRUE( copy_of_p8 == (std::vector<std::string>{"test string", "string2", "last string"}));
  EXPECT_TRUE( (copy_of_p9.isEmpty()));
  EXPECT_TRUE( copy_of_p10 == (std::vector<int>{1, 2, 3, 4, 5}));
  EXPECT_TRUE( copy_of_p11 == (std::vector<double>{1.2, 2.3, 3.4}));

  EXPECT_TRUE(p1 == empty);
  EXPECT_TRUE(p3 == empty);
  EXPECT_TRUE(p4 == empty);
  EXPECT_TRUE(p5 == empty);
  EXPECT_TRUE(p6 == empty);
  EXPECT_TRUE(p7 == empty);
  EXPECT_TRUE(p8 == empty);
  EXPECT_TRUE(p9 == empty);
  EXPECT_TRUE(p10 == empty);
  EXPECT_TRUE(p11 == empty);
}

TEST(ParamValue, assignment_operator) {
  tdl::ParamValue p1((double) 1.23);
  tdl::ParamValue p3((float) 1.23);
  tdl::ParamValue p4((int) -3);
  tdl::ParamValue p5((unsigned int) 123);
  tdl::ParamValue p6("test char");
  tdl::ParamValue p7(std::string("test string"));
  tdl::ParamValue p8({"test string","string2","last string"});
  tdl::ParamValue p9;
  tdl::ParamValue p10(std::vector<int>{1,2,3,4,5});
  tdl::ParamValue p11(std::vector<double>{1.2,2.3,3.4});
  tdl::ParamValue copy_of_p;
  copy_of_p = p1;
  EXPECT_FLOAT_EQ( (double) copy_of_p, 1.23);
  copy_of_p = p3;
  EXPECT_FLOAT_EQ( (float) copy_of_p, 1.23);
  copy_of_p = p4;
  EXPECT_EQ( (int) copy_of_p, -3);
  copy_of_p = p5;
  EXPECT_EQ( (unsigned int) copy_of_p, 123);
  copy_of_p = p6;
  EXPECT_EQ( (std::string) copy_of_p, "test char");
  copy_of_p = p7;
  EXPECT_EQ( (std::string) copy_of_p, "test string");
  copy_of_p = p8;
  EXPECT_TRUE( copy_of_p == (std::vector<std::string>{"test string", "string2", "last string"}));
  copy_of_p = p9;
  EXPECT_TRUE( (copy_of_p.isEmpty()));
  copy_of_p = p10;
  EXPECT_TRUE(copy_of_p == (std::vector<int>{1,2,3,4,5}));
  copy_of_p = p11;
  EXPECT_TRUE(copy_of_p == (std::vector<double>{1.2,2.3,3.4}));
}

TEST(ParamValue, move_operator) {
  // Ensure that ParamValue has a no-except move assignment operator.
  EXPECT_TRUE(noexcept(std::declval<tdl::ParamValue&>() = std::declval<tdl::ParamValue &&>()));

  tdl::ParamValue empty;
  tdl::ParamValue p1((double) 1.23);
  tdl::ParamValue p3((float) 1.23);
  tdl::ParamValue p4((int) -3);
  tdl::ParamValue p5((unsigned int) 123);
  tdl::ParamValue p6("test char");
  tdl::ParamValue p7(std::string("test string"));
  tdl::ParamValue p8({"test string","string2","last string"});
  tdl::ParamValue p9;
  tdl::ParamValue p10(std::vector<int>{1,2,3,4,5});
  tdl::ParamValue p11(std::vector<double>{1.2,2.3,3.4});
  tdl::ParamValue copy_of_p;
  copy_of_p = std::move(p1);
  EXPECT_FLOAT_EQ( (double) copy_of_p, 1.23);
  copy_of_p = std::move(p3);
  EXPECT_FLOAT_EQ( (float) copy_of_p, 1.23);
  copy_of_p = std::move(p4);
  EXPECT_EQ( (int) copy_of_p, -3);
  copy_of_p = std::move(p5);
  EXPECT_EQ( (unsigned int) copy_of_p, 123);
  copy_of_p = std::move(p6);
  EXPECT_EQ( (std::string) copy_of_p, "test char");
  copy_of_p = std::move(p7);
  EXPECT_EQ( (std::string) copy_of_p, "test string");
  copy_of_p = std::move(p8);
  EXPECT_TRUE( copy_of_p == (std::vector<std::string>{"test string","string2","last string"}));
  copy_of_p = std::move(p9);
  EXPECT_TRUE( (copy_of_p.isEmpty()));
  copy_of_p = std::move(p10);
  EXPECT_TRUE(copy_of_p == (std::vector<int>{1,2,3,4,5}));
  copy_of_p = std::move(p11);
  EXPECT_TRUE(copy_of_p == (std::vector<double>{1.2,2.3,3.4}));

  EXPECT_TRUE(p1 == empty);
  EXPECT_TRUE(p3 == empty);
  EXPECT_TRUE(p4 == empty);
  EXPECT_TRUE(p5 == empty);
  EXPECT_TRUE(p6 == empty);
  EXPECT_TRUE(p7 == empty);
  EXPECT_TRUE(p8 == empty);
  EXPECT_TRUE(p9 == empty);
  EXPECT_TRUE(p10 == empty);
  EXPECT_TRUE(p11 == empty);
}

TEST(ParamValue, isEmpty) {
  tdl::ParamValue p1;
  EXPECT_TRUE(p1.isEmpty());

  tdl::ParamValue p2((float)1.2);
  EXPECT_FALSE(p2.isEmpty());
  EXPECT_FLOAT_EQ((float) p2, 1.2);

  tdl::ParamValue p3("");
  EXPECT_FALSE(p3.isEmpty()); // empty std::string does not count as empty!

  tdl::ParamValue p4("2");
  EXPECT_FALSE(p4.isEmpty());
  EXPECT_EQ((std::string) p4, "2");
}

TEST(ParamValue, conversion_string_operator) {
  tdl::ParamValue d((std::string) "test string");
  std::string k = d;
  EXPECT_EQ(k,"test string");
}

TEST(ParamValue, conversion_vector_string_operator) {
  std::vector<std::string> sl = {"test string list"};
  tdl::ParamValue d(sl);
  std::vector<std::string> sl_op = d;
  EXPECT_TRUE(sl_op == d);
}

TEST(ParamValue, toStringVector) {
  std::vector<std::string> sl = {"test string list"};
  tdl::ParamValue d(sl);
  std::vector<std::string> sl_op = d.toStringVector();
  EXPECT_TRUE(sl_op == d);
}

TEST(ParamValue, conversion_vector_int_operator) {
  std::vector<int> il = {1, 2};
  tdl::ParamValue d(il);
  std::vector<int> il_op = d;
  EXPECT_TRUE(il_op == il);
  //TEST_EXCEPTION(Exception::ConversionError, std::vector<string> sl = ParamValue("abc,ab");)
}

TEST(ParamValue, toIntVector) {
  std::vector<int> il = {1, 2};
  tdl::ParamValue d(il);
  std::vector<int> il_op = d.toIntVector();
  EXPECT_TRUE(il_op == il);
  //TEST_EXCEPTION(Exception::ConversionError, std::vector<string> sl = ParamValue("abc,ab").toStringVector();)
}

TEST(ParamValue, conversion_vector_double_operator) {
  std::vector<double> dl = {1.2, 22.34455};
  tdl::ParamValue d(dl);
  std::vector<double> dl_op = d;
  EXPECT_TRUE(dl_op == d);
}

TEST(ParamValue, toDoubleVector) {
  std::vector<double> dl = {1.2, 22.34455};
  tdl::ParamValue d(dl);
  std::vector<double> dl_op = d.toDoubleVector();
  EXPECT_TRUE(dl_op == d);
}

TEST(ParamValue, conversion_long_double_operator) {
  tdl::ParamValue d(5.4L);
  long double k = d;
  EXPECT_FLOAT_EQ(k,5.4L);
}

TEST(ParamValue, conversion_double_operator) {
  tdl::ParamValue d(5.4);
  double k = d;
  EXPECT_FLOAT_EQ(k,5.4);
}

TEST(ParamValue, conversion_float_operator) {
  tdl::ParamValue d(5.4f);
  float k = d;
  EXPECT_FLOAT_EQ(k,5.4f);
}

TEST(ParamValue, conversion_int_operator) {
  tdl::ParamValue d((int) -55);
  int k = d;
  EXPECT_EQ(k,-55);

  //TEST_EXCEPTION(Exception::ConversionError, (int)ParamValue(55.4))
}

TEST(ParamValue, conversion_unsigned_int_operator) {
  tdl::ParamValue d((int) 55);
  unsigned int k = d;
  EXPECT_EQ(k,55);

  //TEST_EXCEPTION(Exception::ConversionError, (unsigned int)ParamValue(-55))
  //TEST_EXCEPTION(Exception::ConversionError, (unsigned int)ParamValue(55.4))
}

TEST(ParamValue, conversion_short_int) {
  tdl::ParamValue d((short int) -55);
  short int k = d;
  EXPECT_EQ(k,-55);

  //TEST_EXCEPTION(Exception::ConversionError, (short int)ParamValue(55.4))
}

TEST(ParamValue, conversion_unsigned_short_int) {
  tdl::ParamValue d((short int) 55);
  unsigned short int k = d;
  EXPECT_EQ(k,55);

  //TEST_EXCEPTION(Exception::ConversionError, (unsigned short int)ParamValue(-55))
  //TEST_EXCEPTION(Exception::ConversionError, (unsigned short int)ParamValue(55.4))
}

TEST(ParamValue, conversion_long_int) {
  tdl::ParamValue d((long int) -55);
  long int k = d;
  EXPECT_EQ(k,-55);

  //TEST_EXCEPTION(Exception::ConversionError, (long int)ParamValue(55.4))
}

TEST(ParamValue, conversion_unsigned_long_int) {
  tdl::ParamValue d((long int) 55);
  unsigned long int k = d;
  EXPECT_EQ(k,55);

  //TEST_EXCEPTION(Exception::ConversionError, (unsigned long int)ParamValue(-55))
  //TEST_EXCEPTION(Exception::ConversionError, (unsigned long int)ParamValue(55.4))
}

TEST(ParamValue, conversion_long_long) {
  {
      tdl::ParamValue d((long long) 55);
      long long k = d;
      EXPECT_EQ(k,55);
  }
  {
      tdl::ParamValue d((long long) -1);
      long long k = d;
      EXPECT_EQ(k,-1);
  }
  {
      tdl::ParamValue d((ssize_t) -55);
      ssize_t k = d;
      EXPECT_EQ(k,-55);
  }

  //TEST_EXCEPTION(Exception::ConversionError, (long int)ParamValue(55.4))
}

TEST(ParamValue, conversion_unsigned_long_long) {
  {
      tdl::ParamValue d((unsigned long long) 55);
      unsigned long long k = d;
      EXPECT_EQ(k,55);
  }
  {
      tdl::ParamValue d((size_t) 55);
      size_t k = d;
      EXPECT_EQ(k,55);
  }

  //TEST_EXCEPTION(Exception::ConversionError, (unsigned long int)ParamValue(-55))
  //TEST_EXCEPTION(Exception::ConversionError, (unsigned long int)ParamValue(55.4))
}

TEST(ParamValue, compare_operator) {
  tdl::ParamValue a(5.0);
  tdl::ParamValue b(5.0);
  EXPECT_TRUE(a==b);
  a = tdl::ParamValue((double)15.13);
  b = tdl::ParamValue((double)15.13);
  EXPECT_TRUE(a==b);
  a = tdl::ParamValue((float)15.13);
  b = tdl::ParamValue((float)(17-1.87));
  EXPECT_TRUE(a==b);
  a = tdl::ParamValue((int)5);
  b = tdl::ParamValue((int)5);
  EXPECT_TRUE(a==b);
  a = tdl::ParamValue((unsigned int)5000);
  b = tdl::ParamValue((unsigned int)5000);
  EXPECT_TRUE(a==b);
  a = tdl::ParamValue("hello");
  b = tdl::ParamValue(std::string("hello"));
  EXPECT_TRUE(a==b);
  a = tdl::ParamValue((float)15.13);
  b = tdl::ParamValue((float)(15.13001));
  EXPECT_FALSE(a==b);

}

TEST(ParamValue, unequal_operator) {
  tdl::ParamValue a(5.0);
  tdl::ParamValue b(5.1);
  EXPECT_TRUE(a!=b);
  a = tdl::ParamValue((double)15.13001);
  b = tdl::ParamValue((double)15.13);
  EXPECT_TRUE(a!=b);

  a = tdl::ParamValue("hello");
  b = tdl::ParamValue(std::string("hello"));
  EXPECT_FALSE(a!=b);
}

TEST(ParamValue, toChar) {
  tdl::ParamValue a;
  EXPECT_TRUE(a.toChar() == nullptr);
  a = tdl::ParamValue("hello");
  EXPECT_EQ(a.toChar(),std::string{"hello"});
  a = tdl::ParamValue(5);
  //TEST_EXCEPTION(Exception::ConversionError, a.toChar() )
}

TEST(ParamValue, toString) {
  tdl::ParamValue a;
  EXPECT_EQ(a.toString(), "");
  a = tdl::ParamValue("hello");
  EXPECT_EQ(a.toString(),"hello");
  a = tdl::ParamValue(5);
  EXPECT_EQ(a.toString(), "5");
  a = tdl::ParamValue(47.11);
  EXPECT_EQ(a.toString(), "47.109999999999999");
  EXPECT_EQ(a.toString(false), "47.11");
  a = tdl::ParamValue(-23456.78);
  EXPECT_EQ(a.toString(), "-2.345678e04");
  a = tdl::ParamValue(std::vector<std::string>{"test string", "string2", "last string"});
  EXPECT_EQ(a.toString(), "[test string, string2, last string]");
  a = tdl::ParamValue(std::vector<int>{1,2,3,4,5});
  EXPECT_EQ(a.toString(),"[1, 2, 3, 4, 5]");
  a = tdl::ParamValue(std::vector<double>{1.2,47.11,1.2345678e05});
  EXPECT_EQ(a.toString(),"[1.2, 47.109999999999999, 1.2345678e05]");
  EXPECT_EQ(a.toString(false), "[1.2, 47.11, 1.235e05]");
}

TEST(ParamValue, toBool) {
  //valid cases
  tdl::ParamValue a("true");
  EXPECT_TRUE(a.toBool());
  a = tdl::ParamValue("false");
  EXPECT_FALSE(a.toBool());

  //invalid cases
  a = tdl::ParamValue();
  //TEST_EXCEPTION(Exception::ConversionError, a.toBool() )
  a = tdl::ParamValue("bla");
  //TEST_EXCEPTION(Exception::ConversionError, a.toBool() )
  a = tdl::ParamValue(12);
  //TEST_EXCEPTION(Exception::ConversionError, a.toBool() )
  a = tdl::ParamValue(34.45);
  //TEST_EXCEPTION(Exception::ConversionError, a.toBool() )
}

TEST(ParamValue, stream_operator) {
  tdl::ParamValue a((int)5), b((unsigned int)100), c((double)1.111), d((double)1.1), e("hello "), f(std::string("world")), g;
  std::ostringstream os;
  os << a << b << c << d << e << f << g;
  EXPECT_EQ(os.str(),"51001.1111.1hello world");
}

TEST(ParamValue, valueType) {
  tdl::ParamValue a;
  EXPECT_EQ(a.valueType(), tdl::ParamValue::EMPTY_VALUE);

  tdl::ParamValue a1(1.45);
  EXPECT_EQ(a1.valueType(), tdl::ParamValue::DOUBLE_VALUE);

  tdl::ParamValue a2(1.34f);
  EXPECT_EQ(a2.valueType(), tdl::ParamValue::DOUBLE_VALUE);

  tdl::ParamValue a3(123);
  EXPECT_EQ(a3.valueType(), tdl::ParamValue::INT_VALUE);

  tdl::ParamValue a4("bla");
  EXPECT_EQ(a4.valueType(), tdl::ParamValue::STRING_VALUE);

  tdl::ParamValue a5(std::vector<std::string>{"test string","string2","last string"});
  EXPECT_EQ(a5.valueType(), tdl::ParamValue::STRING_LIST);

  tdl::ParamValue a6((unsigned int)(2));
  EXPECT_EQ(a6.valueType(), tdl::ParamValue::INT_VALUE);

  tdl::ParamValue a7(std::vector<int>{1,2,3});
  EXPECT_EQ(a7.valueType(), tdl::ParamValue::INT_LIST);

  tdl::ParamValue a8(std::vector<double>{1.2,32.4567});
  EXPECT_EQ(a8.valueType(), tdl::ParamValue::DOUBLE_LIST);
}

TEST(ParamValue, assign_operator_char_ptr) {
  const char * v = "value";
  tdl::ParamValue a("v");
  a = v;
  EXPECT_EQ(a, "value");
}

TEST(ParamValue, assign_operator_string) {
  std::string v = "value";
  tdl::ParamValue a("v");
  a = v;
  EXPECT_EQ(a, "value");
}

TEST(ParamValue, assign_operator_vector_string) {
  std::vector<std::string> v = {"value","value2"};
  tdl::ParamValue a("v");
  a = v;
  std::vector<std::string> sla = a;
  ASSERT_EQ(sla.size(), 2);
  EXPECT_EQ(sla[0], "value");
  EXPECT_EQ(sla[1], "value2");
}

TEST(ParamValue, assign_operator_vector_int) {
  std::vector<int> v = {2,-3};
  tdl::ParamValue a("v");
  a = v;
  std::vector<int> dv = a;
  ASSERT_EQ(dv.size(), 2);
  EXPECT_EQ(dv[0], 2);
  EXPECT_EQ(dv[1], -3);
}

TEST(ParamValue, assign_operator_vector_double) {
  std::vector<double> v = {2.14,-3.45};
  tdl::ParamValue a("v");
  a = v;
  std::vector<double> adl = a;
  ASSERT_EQ(adl.size(), 2);
  EXPECT_EQ(adl[0], 2.14);
  EXPECT_EQ(adl[1], -3.45);
}

TEST(ParamValue, assign_operator_const_long_double) {
  const long double v = 2.44;
  tdl::ParamValue a("v");
  a = v;
  EXPECT_EQ((long double)a, 2.44);
}

TEST(ParamValue, assign_operator_const_double) {
  const double v = 2.44;
  tdl::ParamValue a("v");
  a = v;
  EXPECT_EQ((double)a, 2.44);
}

TEST(ParamValue, assign_operator_const_float) {
  const float v = 2.44f;
  tdl::ParamValue a("v");
  a = v;
  EXPECT_EQ((float)a, 2.44f);
}

TEST(ParamValue, assign_operator_const_short_int) {
  const short int v = 2;
  tdl::ParamValue a("v");
  a = v;
  EXPECT_EQ((short int)a, 2);
}

TEST(ParamValue, assign_operator_const_unsigned_short_int) {
  const unsigned short int v = 2;
  tdl::ParamValue a("v");
  a = v;
  EXPECT_EQ((unsigned short int)a, 2);
}

TEST(ParamValue, assign_operator_const_int) {
  const int v = 2;
  tdl::ParamValue a("v");
  a = v;
  EXPECT_EQ((int)a, 2);
}

TEST(ParamValue, assign_operator_const_unsigned) {
  const unsigned v = 2;
  tdl::ParamValue a("v");
  a = v;
  EXPECT_EQ((unsigned)a, 2);
}

TEST(ParamValue, assign_operator_const_long_int) {
  const long int v = 2;
  tdl::ParamValue a("v");
  a = v;
  EXPECT_EQ((long int)a, 2);
}

TEST(ParamValue, assign_operator_const_unsigned_long) {
  const unsigned long v = 2;
  tdl::ParamValue a("v");
  a = v;
  EXPECT_EQ((unsigned long)a, 2);
}

TEST(ParamValue, assign_operator_const_long_long) {
  const long long v = 2;
  tdl::ParamValue a("v");
  a = v;
  EXPECT_EQ((long long)a, 2);
}

TEST(ParamValue, assign_operator_const_unsigned_long_long) {
  const unsigned long long v = 2;
  tdl::ParamValue a("v");
  a = v;
  EXPECT_EQ((unsigned long long)a, 2);
}
