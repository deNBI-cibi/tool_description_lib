// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/deNBI-cibi/tool_description_lib/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides tdl::ParamValue.
 */

#pragma once

#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

#include <tdl/platform.h>
#include <tdl/exception.h>

namespace tdl
{
  /*!\brief Class to hold strings, numeric values, vectors of strings and vectors of numeric values using the stl types.
   * \ingroup Datastructures
   * \details
   * * To choose one of these types, just use the appropriate constructor.
   * * Automatic conversion is supported and throws exceptions in case of invalid conversions.
   * * An empty object is created with the default constructor.
   */
  class TDL_DLLAPI ParamValue
  {

  public:

    //!\brief Empty data value for comparisons.
    static const ParamValue EMPTY;

    //!\brief Supported types for ParamValue.
    enum ValueType : unsigned char
    {
      STRING_VALUE, //!< string value
      INT_VALUE, //!< integer value
      DOUBLE_VALUE, //!< double value
      STRING_LIST, //!< string vector
      INT_LIST, //!< integer vector
      DOUBLE_LIST, //!< double vector
      EMPTY_VALUE //!< empty value
    };

    /*!\name Constructors and destructors
     * \{
     */
    //!\brief Default constructor.
    ParamValue();
    //!\brief Copy constructor.
    ParamValue(const ParamValue&);
    //!\brief Move constructor.
    ParamValue(ParamValue&&) noexcept;
    //!\brief Specific constructor for char* (converted to string).
    ParamValue(const char*);
    //!\brief Specific constructor for std::string values.
    ParamValue(const std::string&);
    //!\brief Specific constructor for string vectors.
    ParamValue(const std::vector<std::string>&);
    //!\brief Specific constructor for integer vectors.
    ParamValue(const std::vector<int>&);
    //!\brief Specific constructor for double vectors.
    ParamValue(const std::vector<double>&);
    //!\brief Specific constructor for long double values (note: the implementation uses double).
    ParamValue(long double);
    //!\brief Specific constructor for double values (note: the implementation uses double).
    ParamValue(double);
    //!\brief Specific constructor for float values (note: the implementation uses double).
    ParamValue(float);
    //!\brief Specific constructor for short int values (note: the implementation uses ptrdiff_t).
    ParamValue(short int);
    //!\brief Specific constructor for unsigned short int values (note: the implementation uses ptrdiff_t).
    ParamValue(unsigned short int);
    //!\brief Specific constructor for int values (note: the implementation uses ptrdiff_t).
    ParamValue(int);
    //!\brief Specific constructor for unsigned int values (note: the implementation uses ptrdiff_t).
    ParamValue(unsigned int);
    //!\brief Specific constructor for long int values (note: the implementation uses ptrdiff_t).
    ParamValue(long int);
    //!\brief Specific constructor for unsigned long int values (note: the implementation uses ptrdiff_t).
    ParamValue(unsigned long int);
    //!\brief Specific constructor for long long int values (note: the implementation uses ptrdiff_t).
    ParamValue(long long);
    //!\brief Specific constructor for unsigned long long int values (note: the implementation uses ptrdiff_t).
    ParamValue(unsigned long long);
    //!\brief Destructor.
    ~ParamValue();
    //!\}

    /*!\name Cast operators
     * \details
     * These methods are used when the DataType is known.
     * If they are applied to a ParamValue with the wrong DataType, an exception (Exception::ConversionError) is thrown.
     * In particular, none of these operators will work for an empty ParamValue (DataType EMPTY_VALUE) -
     * except toChar(), which will return 0.
     * \{
     */

    /*!\brief Conversion operator to string.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator std::string() const;

    /*!\brief Conversion operator to string vector.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator std::vector<std::string>() const;

    /*!\brief Conversion operator to integer vector.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator std::vector<int>() const;

    /*!\brief Conversion operator to double vector.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator std::vector<double>() const;

    /*!\brief Conversion operator to long double.
     * \note The implementation uses typedef double (as opposed to float, double, long double).
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator long double() const;

    /*!\brief Conversion operator to double.
     * \note The implementation uses typedef double (as opposed to float, double, long double).
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator double() const;

    /*!\brief Conversion operator to float.
     * \note The implementation uses typedef double (as opposed to float, double, long double).
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator float() const;

    /*!\brief Conversion operator to short int.
     * \note The implementation uses typedef ptrdiff_t.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator short int() const;

    /*!\brief Conversion operator to unsigned short int.
     * \note The implementation uses typedef ptrdiff_t.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator unsigned short int() const;

    /*!\brief Conversion operator to int.
     * \note The implementation uses typedef ptrdiff_t.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator int() const;

    /*!\brief Conversion operator to unsigned int.
     * \note The implementation uses typedef ptrdiff_t.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator unsigned int() const;

    /*!\brief Conversion operator to long int.
     * \note The implementation uses typedef ptrdiff_t.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator long int() const;

    /*!\brief Conversion operator to unsigned long int.
     * \note The implementation uses typedef ptrdiff_t.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator unsigned long int() const;

    /*!\brief Conversion operator to long long.
     * \note The implementation uses typedef ptrdiff_t.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator long long() const;

    /*!\brief Conversion operator to unsigned long long.
     * \note The implementation uses typedef ptrdiff_t.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    operator unsigned long long() const;

    /*!\brief Converts the strings 'true' and 'false' to a bool.
     * \throws Exception::ConversionError for non-string parameters and string parameters with values other
     *         than 'true' and 'false'.
     */
    bool toBool() const;

    /*!\brief Convert ParamValues to char*.
     * \returns `nullptr` if ParamValue is empty, data pointer otherwise.
     */
    const char* toChar() const;

    /*!\brief Convert ParamValue to string.
     * \throws Exception::ConversionError for ParamValue::EMPTY.
     */
    std::string toString(bool full_precision = true) const;

    /*!\brief Explicitly convert ParamValue to string vector.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    std::vector<std::string> toStringVector() const;

    /*!\brief Explicitly convert ParamValue to IntList.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    std::vector<int> toIntVector() const;

    /*!\brief Explicitly convert ParamValue to DoubleList.
     * \throws Exception::ConversionError if a cast from the the wrong type is requested.
     */
    std::vector<double> toDoubleVector() const;
    //!\}

    /*!\name Assignment operators
     * \details
     * These methods are used to assign supported types directly to a ParamValue object.
     * \{
     */
    //!\brief Assignment operator.
    ParamValue& operator=(const ParamValue&);
    //!\brief Move assignment operator.
    ParamValue& operator=(ParamValue&&) noexcept;
    //!\brief Specific assignment for char* (converted to string).
    ParamValue& operator=(const char*);
    //!\brief Specific assignment for std::string values.
    ParamValue& operator=(const std::string&);
    //!\brief Specific assignment for string vectors.
    ParamValue& operator=(const std::vector<std::string>&);
    //!\brief Specific assignment for integer vectors.
    ParamValue& operator=(const std::vector<int>&);
    //!\brief Specific assignment for double vectors.
    ParamValue& operator=(const std::vector<double>&);
    //!\brief Specific assignment for long double values (note: the implementation uses double).
    ParamValue& operator=(const long double);
    //!\brief Specific assignment for double values (note: the implementation uses double).
    ParamValue& operator=(const double);
    //!\brief Specific assignment for float values (note: the implementation uses double).
    ParamValue& operator=(const float);
    //!\brief Specific assignment for short int values (note: the implementation uses ptrdiff_t).
    ParamValue& operator=(const short int);
    //!\brief Specific assignment for unsigned short int values (note: the implementation uses ptrdiff_t).
    ParamValue& operator=(const unsigned short int);
    //!\brief Specific assignment for int values (note: the implementation uses ptrdiff_t).
    ParamValue& operator=(const int);
    //!\brief Specific assignment for unsigned int values (note: the implementation uses ptrdiff_t).
    ParamValue& operator=(const unsigned int);
    //!\brief Specific assignment for long int values (note: the implementation uses ptrdiff_t).
    ParamValue& operator=(const long int);
    //!\brief Specific assignment for unsigned long int values (note: the implementation uses ptrdiff_t).
    ParamValue& operator=(const unsigned long int);
    //!\brief Specific assignment for long long int values (note: the implementation uses ptrdiff_t).
    ParamValue& operator=(const long long);
    //!\brief Specific assignment for unsigned long long int values (note: the implementation uses ptrdiff_t).
    ParamValue& operator=(const unsigned long long);
    //!\}

    //!\brief Returns the type of the stored value.
    inline ValueType valueType() const
    {
      return value_type_;
    }

    /*!\brief Tests whether the value is empty.
     * \note A ParamValue containing an empty string ("") does not count as empty!
     */
    inline bool isEmpty() const
    {
      return value_type_ == EMPTY_VALUE;
    }

    //!\brief Formatted output.
    friend TDL_DLLAPI std::ostream& operator<<(std::ostream&, const ParamValue&);

    //!\brief Equality comparator.
    friend TDL_DLLAPI bool operator==(const ParamValue&, const ParamValue&);

    //!\brief Smaller than comparator (for vectors we use the size).
    friend TDL_DLLAPI bool operator<(const ParamValue&, const ParamValue&);

    //!\brief Greater than comparator (for vectors we use the size).
    friend TDL_DLLAPI bool operator>(const ParamValue&, const ParamValue&);

    //!\brief Inequality comparator.
    friend TDL_DLLAPI bool operator!=(const ParamValue&, const ParamValue&);

  protected:

    //!\brief Type of the currently stored value.
    ValueType value_type_;

    //!\brief The data.
    union
    {
      ptrdiff_t ssize_;
      double dou_;
      std::string* str_;
      std::vector<std::string>* str_list_;
      std::vector<int>* int_list_;
      std::vector<double>* dou_list_;
    } data_{};

  private:

    //!\brief Clears the current state of the ParamValue and release every used memory.
    void clear_() noexcept;

    /*!\brief Convert a double to std::string.
     * \details
     * With full precision, 15 decimal places are returned, otherwise 3.
     * Numbers above 10000 or below 0.0001 are returned in scientific notation (i.e. 1.0e04).
     */
    static std::string doubleToString(double value, bool full_precision = true);

  };

  inline const ParamValue ParamValue::EMPTY;

  // default ctor
  inline ParamValue::ParamValue() :
    value_type_(EMPTY_VALUE)
  {
  }

  // destructor
  inline ParamValue::~ParamValue()
  {
    clear_();
  }

  //-------------------------------------------------------------------
  //    ctor for all supported types a ParamValue object can hold
  //--------------------------------------------------------------------
  inline ParamValue::ParamValue(long double p) :
    value_type_(DOUBLE_VALUE)
  {
    data_.dou_ = p;
  }

  inline ParamValue::ParamValue(double p) :
    value_type_(DOUBLE_VALUE)
  {
    data_.dou_ = p;
  }

  inline ParamValue::ParamValue(float p) :
    value_type_(DOUBLE_VALUE)
  {
    data_.dou_ = p;
  }

  inline ParamValue::ParamValue(short int p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  inline ParamValue::ParamValue(unsigned short int p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  inline ParamValue::ParamValue(int p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  inline ParamValue::ParamValue(unsigned int p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  inline ParamValue::ParamValue(long int p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  inline ParamValue::ParamValue(unsigned long int p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  inline ParamValue::ParamValue(long long p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  inline ParamValue::ParamValue(unsigned long long p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  inline ParamValue::ParamValue(const char* p) :
    value_type_(STRING_VALUE)
  {
    data_.str_ = new std::string(p);
  }

  inline ParamValue::ParamValue(const std::string& p) :
    value_type_(STRING_VALUE)
  {
    data_.str_ = new std::string(p);
  }

  inline ParamValue::ParamValue(const std::vector<std::string>& p) :
    value_type_(STRING_LIST)
  {
    data_.str_list_ = new std::vector<std::string>(p);
  }

  inline ParamValue::ParamValue(const std::vector<int>& p) :
    value_type_(INT_LIST)
  {
    data_.int_list_ = new std::vector<int>(p);
  }

  inline ParamValue::ParamValue(const std::vector<double>& p) :
    value_type_(DOUBLE_LIST)
  {
    data_.dou_list_ = new std::vector<double>(p);
  }

  //--------------------------------------------------------------------
  //                   copy and move constructors
  //--------------------------------------------------------------------
  inline ParamValue::ParamValue(const ParamValue& p) :
    value_type_(p.value_type_)
  {
    switch (value_type_)
    {
    case STRING_VALUE:
        data_.str_ = new std::string(*p.data_.str_);
    break;
    case STRING_LIST:
        data_.str_list_ = new std::vector<std::string>(*p.data_.str_list_);
    break;
    case INT_LIST:
        data_.int_list_ = new std::vector<int>(*p.data_.int_list_);
    break;
    case DOUBLE_LIST:
        data_.dou_list_ = new std::vector<double>(*p.data_.dou_list_);
    break;
    default:
        data_ = p.data_;
    break;
    }
  }

  inline ParamValue::ParamValue(ParamValue&& rhs) noexcept :
    value_type_(std::move(rhs.value_type_)),
    data_(std::move(rhs.data_))
  {
    // clean up rhs, take ownership of data_
    // NOTE: value_type_ == EMPTY_VALUE implies data_ is empty and can be reset
    rhs.value_type_ = EMPTY_VALUE;
  }

  inline void ParamValue::clear_() noexcept
  {
    switch (value_type_)
    {
      case STRING_VALUE:
        delete data_.str_;
      break;
      case STRING_LIST:
        delete data_.str_list_;
      break;
      case INT_LIST:
        delete data_.int_list_;
      break;
      case DOUBLE_LIST:
        delete data_.dou_list_;
      break;
      default:
      break;
    }
    value_type_ = EMPTY_VALUE;
  }

  //--------------------------------------------------------------------
  //                    copy and move assignment operators
  //--------------------------------------------------------------------
  inline ParamValue& ParamValue::operator=(const ParamValue& p)
  {
    // Check for self-assignment
    if (this == &p)
    {
      return *this;
    }

    // clean up
    clear_();

    // assign
    switch (p.value_type_)
    {
    case STRING_VALUE:
        data_.str_ = new std::string(*p.data_.str_);
        break;
    case STRING_LIST:
        data_.str_list_ = new std::vector<std::string>(*p.data_.str_list_);
        break;
    case INT_LIST:
        data_.int_list_ = new std::vector<int>(*p.data_.int_list_);
        break;
    case DOUBLE_LIST:
        data_.dou_list_ = new std::vector<double>(*p.data_.dou_list_);
        break;
    default:
        data_ = p.data_;
        break;
    }

    // copy type
    value_type_ = p.value_type_;

    return *this;
  }

  //!\brief Move assignment operator
  inline ParamValue& ParamValue::operator=(ParamValue&& rhs) noexcept
  {
    // Check for self-assignment
    if (this == &rhs)
    {
      return *this;
    }

    // clean up *this
    clear_();

    // assign values to *this
    data_ = rhs.data_;
    value_type_ = rhs.value_type_;
    // clean up rhs
    rhs.value_type_ = EMPTY_VALUE;

    return *this;
  }

  //--------------------------------------------------------------------
  //                assignment conversion operator
  //--------------------------------------------------------------------

  inline ParamValue& ParamValue::operator=(const char* arg)
  {
    clear_();
    data_.str_ = new std::string(arg);
    value_type_ = STRING_VALUE;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const std::string& arg)
  {
    clear_();
    data_.str_ = new std::string(arg);
    value_type_ = STRING_VALUE;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const std::vector<std::string>& arg)
  {
    clear_();
    data_.str_list_ = new std::vector<std::string>(arg);
    value_type_ = STRING_LIST;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const std::vector<int>& arg)
  {
    clear_();
    data_.int_list_ = new std::vector<int>(arg);
    value_type_ = INT_LIST;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const std::vector<double>& arg)
  {
    clear_();
    data_.dou_list_ = new std::vector<double>(arg);
    value_type_ = DOUBLE_LIST;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const long double arg)
  {
    clear_();
    data_.dou_ = arg;
    value_type_ = DOUBLE_VALUE;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const double arg)
  {
    clear_();
    data_.dou_ = arg;
    value_type_ = DOUBLE_VALUE;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const float arg)
  {
    clear_();
    data_.dou_ = arg;
    value_type_ = DOUBLE_VALUE;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const short int arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const unsigned short int arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const int arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const unsigned int arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const long int arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const unsigned long int arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const long long arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  inline ParamValue& ParamValue::operator=(const unsigned long long arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  //---------------------------------------------------------------------------
  //                      Conversion operators
  //----------------------------------------------------------------------------
  inline ParamValue::operator long double() const
  {
    if (value_type_ == EMPTY_VALUE)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert ParamValue::EMPTY to long double");
    }
    else if (value_type_ == INT_VALUE)
    {
      return (long double)(data_.ssize_);
    }
    return data_.dou_;
  }

  inline ParamValue::operator double() const
  {
    if (value_type_ == EMPTY_VALUE)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert ParamValue::EMPTY to double");
    }
    else if (value_type_ == INT_VALUE)
    {
      return double(data_.ssize_);
    }
    return data_.dou_;
  }

  inline ParamValue::operator float() const
  {
    if (value_type_ == EMPTY_VALUE)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert ParamValue::EMPTY to float");
    }
    else if (value_type_ == INT_VALUE)
    {
      return float(data_.ssize_);
    }
    return data_.dou_;
  }

  inline ParamValue::operator short int() const
  {
    if (value_type_ != INT_VALUE)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert non-integer ParamValue to short int");
    }
    return data_.ssize_;
  }

  inline ParamValue::operator unsigned short int() const
  {
    if (value_type_ != INT_VALUE)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert non-integer ParamValue to UInt");
    }
    if (data_.ssize_ < 0.0)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert negative integer ParamValue to unsigned short int");
    }
    return data_.ssize_;
  }

  inline ParamValue::operator int() const
  {
    if (value_type_ != INT_VALUE)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert non-integer ParamValue to int");
    }
    return data_.ssize_;
  }

  inline ParamValue::operator unsigned int() const
  {
    if (value_type_ != INT_VALUE)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert non-integer ParamValue to unsigned int");
    }
    if (data_.ssize_ < 0.0)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert negative integer ParamValue to unsigned int");
    }
    return data_.ssize_;
  }

  inline ParamValue::operator long int() const
  {
    if (value_type_ != INT_VALUE)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert non-integer ParamValue to long int");
    }
    return data_.ssize_;
  }

  inline ParamValue::operator unsigned long int() const
  {
    if (value_type_ != INT_VALUE)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert non-integer ParamValue to unsigned long int");
    }
    if (data_.ssize_ < 0.0)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert negative integer ParamValue to unsigned long int");
    }
    return data_.ssize_;
  }

  inline ParamValue::operator long long() const
  {
    if (value_type_ != INT_VALUE)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert non-integer ParamValue to Int");
    }
    return data_.ssize_;
  }

  inline ParamValue::operator unsigned long long() const
  {
    if (value_type_ != INT_VALUE)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert non-integer ParamValue to UInt");
    }
    if (data_.ssize_ < 0.0)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert negative integer ParamValue to UInt");
    }
    return data_.ssize_;
  }

  inline ParamValue::operator std::string() const
  {
    if (value_type_ != STRING_VALUE)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert non-string ParamValue to string");
    }
    return *(data_.str_);
  }

  //!\cond
  inline ParamValue::operator std::vector<std::string>() const
  {
    return this->toStringVector();
  }
  //!\endcond

  inline ParamValue::operator std::vector<int>() const
  {
    return this->toIntVector();
  }

  inline ParamValue::operator std::vector<double>() const
  {
    return this->toDoubleVector();
  }

  // Convert ParamValues to char*
  inline const char* ParamValue::toChar() const
  {
    switch (value_type_)
    {
    case STRING_VALUE:
        return data_.str_->c_str();
    break;
    case EMPTY_VALUE:
        return nullptr;
    break;
    default:
        throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert non-string ParamValue to char*");
    break;
    }
  }

  inline std::string ParamValue::toString(bool full_precision) const
  {
    std::string str;
    switch (value_type_)
    {
    case EMPTY_VALUE:
        return "";
    break;
    case STRING_VALUE:
        return *data_.str_;
    break;
    case INT_VALUE:
        return std::to_string(data_.ssize_);
    break;
    case DOUBLE_VALUE:
    {
        return doubleToString(data_.dou_, full_precision);
    }
    break;
    case STRING_LIST:
        str = "[";
        if (!data_.str_list_->empty())
        {
            for (std::vector<std::string>::const_iterator it = data_.str_list_->begin();
                 it != data_.str_list_->end() - 1; ++it)
            {
              str += *it + ", ";
            }
            str += data_.str_list_->back();
        }
        str += "]";
    break;
    case INT_LIST:
        str = "[";
        if (!data_.int_list_->empty()) {
            for (std::vector<int>::const_iterator it = data_.int_list_->begin();
                 it != data_.int_list_->end() - 1; ++it)
            {
                str += std::to_string(*it) + ", ";
            }
            str += std::to_string(data_.int_list_->back());
        }
        str += "]";
    break;
    case DOUBLE_LIST:
        str = "[";
        if (!data_.dou_list_->empty()) {
            for (std::vector<double>::const_iterator it = data_.dou_list_->begin();
                 it != data_.dou_list_->end() - 1; ++it) {
                str += doubleToString(*it, full_precision) + ", ";
            }
            str += doubleToString(data_.dou_list_->back(), full_precision);
        }
        str +=  "]";
    break;
    default:
        throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert ParamValue to String");
    break;
    }
    return str;
  }

  inline std::vector<std::string> ParamValue::toStringVector() const
  {
    if (value_type_ != STRING_LIST)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert non-std::vector<std::string> ParamValue to std::vector<std::string>");
    }
    return *(data_.str_list_);
  }

  inline std::vector<int> ParamValue::toIntVector() const
  {
    if (value_type_ != INT_LIST)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert non-std::vector<int> ParamValue to std::vector<int>");
    }
    return *(data_.int_list_);
  }

  inline std::vector<double> ParamValue::toDoubleVector() const {
    if (value_type_ != DOUBLE_LIST)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert non-std::vector<double> ParamValue to std::vector<double>");
    }
    return *(data_.dou_list_);
  }

  inline bool ParamValue::toBool() const
  {
    if (value_type_ != STRING_VALUE)
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert non-string ParamValue to bool.");
    }
    else if (!(*(data_.str_) == "true" || *(data_.str_) == "false"))
    {
      throw ConversionError(Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}, "Could not convert '" + *(data_.str_) + "' to bool. Valid stings are 'true' and 'false'.");
    }

    return *(data_.str_) == "true";
  }

  // ----------------- Comparator ----------------------

  //!\brief Equality comparator.
  inline bool operator==(const ParamValue& a, const  ParamValue& b)
  {
    if (a.value_type_ == b.value_type_)
    {
      switch (a.value_type_)
      {
      case ParamValue::EMPTY_VALUE:
          return true;
      break;
      case ParamValue::STRING_VALUE:
          return *(a.data_.str_) == *(b.data_.str_);
      break;
      case ParamValue::STRING_LIST:
          return *(a.data_.str_list_) == *(b.data_.str_list_);
      break;
      case ParamValue::INT_LIST:
          return *(a.data_.int_list_) == *(b.data_.int_list_);
      break;
      case ParamValue::DOUBLE_LIST:
          return *(a.data_.dou_list_) == *(b.data_.dou_list_);
      break;
      case ParamValue::INT_VALUE:
          return a.data_.ssize_ == b.data_.ssize_;
      break;
      case ParamValue::DOUBLE_VALUE:
          return a.data_.dou_ == b.data_.dou_;
          //return std::fabs(a.data_.dou_ - b.data_.dou_) < 1e-6; This would add an include for <cmath>
      break;
      }
    }
    return false;
  }

  //!\brief Smaller than comparator.
  inline bool operator<(const ParamValue& a, const  ParamValue& b)
  {
    if (a.value_type_ == b.value_type_)
    {
      switch (a.value_type_)
      {
      case ParamValue::EMPTY_VALUE:
          return false;
      break;
      case ParamValue::STRING_VALUE:
          return *(a.data_.str_) < *(b.data_.str_);
      break;
      case ParamValue::STRING_LIST:
          return a.data_.str_list_->size() < b.data_.str_list_->size();
      break;
      case ParamValue::INT_LIST:
          return a.data_.int_list_->size() < b.data_.int_list_->size();
      break;
      case ParamValue::DOUBLE_LIST:
          return a.data_.dou_list_->size() < b.data_.dou_list_->size();
      break;
      case ParamValue::INT_VALUE:
          return a.data_.ssize_ < b.data_.ssize_;
      break;
      case ParamValue::DOUBLE_VALUE:
          return a.data_.dou_ < b.data_.dou_;
      break;
      }
    }
    return false;
  }

  //!\brief Greater than comparator.
  inline bool operator>(const ParamValue& a, const  ParamValue& b)
  {
    if (a.value_type_ == b.value_type_)
    {
      switch (a.value_type_)
      {
      case ParamValue::EMPTY_VALUE:
          return false;
      break;
      case ParamValue::STRING_VALUE:
          return *(a.data_.str_) > *(b.data_.str_);
      break;
      case ParamValue::STRING_LIST:
          return a.data_.str_list_->size() > b.data_.str_list_->size();
      break;
      case ParamValue::INT_LIST:
          return a.data_.int_list_->size() > b.data_.int_list_->size();
      break;
      case ParamValue::DOUBLE_LIST:
          return a.data_.dou_list_->size() > b.data_.dou_list_->size();
      break;
      case ParamValue::INT_VALUE:
          return a.data_.ssize_ > b.data_.ssize_;
      break;
      case ParamValue::DOUBLE_VALUE:
          return a.data_.dou_ > b.data_.dou_;
      break;
      }
    }
    return false;
  }

  //!\brief Inequality comparator.
  inline bool operator!=(const ParamValue& a, const ParamValue& b)
  {
    return !(a == b);
  }

  // ----------------- Output operator ----------------------

  /*!\brief Formatted output.
   * \details
   * For doubles or lists of doubles, you get full precision.
   * Use ParamValue::toString(false) if you only need low precision.
   */
  inline std::ostream& operator<<(std::ostream& os, const ParamValue& p)
  {
    switch (p.value_type_)
    {
    case ParamValue::STRING_VALUE:
        os << *(p.data_.str_);
    break;
    case ParamValue::STRING_LIST:
        os << "[";
        if (!p.data_.str_list_->empty())
        {
          for (auto it = p.data_.str_list_->begin(), end = p.data_.str_list_->end() - 1; it != end; ++it)
          {
            os << *it << ", ";
          }
          os << p.data_.str_list_->back();
        }
        os << "]";
    break;
    case ParamValue::INT_LIST:
        os << "[";
        if (!p.data_.int_list_->empty())
        {
          for (auto it = p.data_.int_list_->begin(), end = p.data_.int_list_->end() - 1; it != end; ++it)
          {
            os << *it << ", ";
          }
          os << p.data_.int_list_->back();
        }
        os << "]";
    break;
    case ParamValue::DOUBLE_LIST:
        os << "[";
        if (!p.data_.dou_list_->empty())
        {
          for (auto it = p.data_.dou_list_->begin(), end = p.data_.dou_list_->end() - 1; it != end; ++it)
          {
            os << *it << ", ";
          }
          os << p.data_.dou_list_->back();
        }
        os << "]";
    break;
    case ParamValue::INT_VALUE:
        os << p.data_.ssize_;
    break;
    case ParamValue::DOUBLE_VALUE:
        os << p.data_.dou_;
    break;
    case ParamValue::EMPTY_VALUE:
    break;
    }
    return os;
  }

  inline std::string ParamValue::doubleToString(double value, bool full_precision)
  {
    std::ostringstream os;
    std::string s;
    if (full_precision)
    {
      os.precision(15);
    }
    else
    {
      os.precision(3);
    }
    if (value != 0 &&
        (std::abs(value) >= 10000.0 ||
         std::abs(value) < 0.001 ||
         (full_precision && std::abs(value) < 0.01)))
    {
      os << std::scientific << value;
      s = os.str();
      size_t cutoff_end = s.find_last_of('e');
      size_t cutoff_start = s.substr(0, cutoff_end).find_last_not_of('0');
      if (s.at(cutoff_end + 1) == '+')
      {
        s.erase(cutoff_end + 1, 1);
      }
      if (cutoff_start != cutoff_end)
      {
        if (s.find_first_of('.') == cutoff_start)
        {
          ++cutoff_start;
        }
        s.erase(cutoff_start + 1, cutoff_end - cutoff_start - 1);
      }
    }
    else
    {
      os << std::fixed << value;
      s = os.str();
      size_t cutoff = s.find_last_not_of('0');
      if (cutoff != std::string::npos)
      {
        if (s.find_first_of('.') == cutoff)
        {
          ++cutoff;
        }
        s.erase(cutoff + 1);
      }
    }
    return s;
  }

} // namespace tdl
