#pragma once

#include <tdl/platform.h>

#include <cstddef> // for ptrdiff_t
#include <string>
#include <vector>

#ifdef OPENMS_EXCEPTIONS_AVAILABLE
#include <OpenMS/Exceptions.h>
#endif

namespace tdl
{
  /**
    @brief Class to hold strings, numeric values, vectors of strings and vectors of numeric values using the stl types.

    - To choose one of these types, just use the appropriate constructor.
    - Automatic conversion is supported and throws Exceptions in case of invalid conversions.
    - An empty object is created with the default constructor.

    @ingroup Datastructures
  */
  class TDL_DLLAPI ParamValue
  {

public:

    /// Empty data value for comparisons
    static const ParamValue EMPTY;

    /// Supported types for ParamValue
    enum ValueType : unsigned char
    {
      STRING_VALUE, ///< string value
      INT_VALUE, ///< integer value
      DOUBLE_VALUE, ///< double value
      STRING_LIST, ///< string vector
      INT_LIST, ///< integer vector
      DOUBLE_LIST, ///< double vector
      EMPTY_VALUE ///< empty value
    };

    /// @name Constructors and destructors
    //@{
    /// Default constructor
    ParamValue();
    /// Copy constructor
    ParamValue(const ParamValue&);
    /// Move constructor
    ParamValue(ParamValue&&) noexcept;
    /// specific constructor for char* (converted to string)
    ParamValue(const char*);
    /// specific constructor for std::string values
    ParamValue(const std::string&);
    /// specific constructor for string vectors
    ParamValue(const std::vector<std::string>&);
    /// specific constructor for integer vectors
    ParamValue(const std::vector<int>&);
    /// specific constructor for double vectors
    ParamValue(const std::vector<double>&);
    /// specific constructor for long double values (note: the implementation uses double)
    ParamValue(long double);
    /// specific constructor for double values (note: the implementation uses double)
    ParamValue(double);
    /// specific constructor for float values (note: the implementation uses double)
    ParamValue(float);
    /// specific constructor for short int values (note: the implementation uses ptrdiff_t)
    ParamValue(short int);
    /// specific constructor for unsigned short int values (note: the implementation uses ptrdiff_t)
    ParamValue(unsigned short int);
    /// specific constructor for int values (note: the implementation uses ptrdiff_t)
    ParamValue(int);
    /// specific constructor for unsigned int values (note: the implementation uses ptrdiff_t)
    ParamValue(unsigned);
    /// specific constructor for long int values (note: the implementation uses ptrdiff_t)
    ParamValue(long int);
    /// specific constructor for unsigned long int values (note: the implementation uses ptrdiff_t)
    ParamValue(unsigned long);
    /// specific constructor for long long int values (note: the implementation uses ptrdiff_t)
    ParamValue(long long);
    /// specific constructor for unsigned long long int values (note: the implementation uses ptrdiff_t)
    ParamValue(unsigned long long);
    /// Destructor
    ~ParamValue();
    //@}

    ///@name Cast operators
    ///These methods are used when the DataType is known.
    ///If they are applied to a ParamValue with the wrong DataType, an exception (Exception::ConversionError) is thrown. In particular, none of these operators will work for an empty ParamValue (DataType EMPTY_VALUE) - except toChar(), which will return 0.
    //@{

    /**
      @brief conversion operator to string

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    operator std::string() const;

    /**
      @brief conversion operator to string vector

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    operator std::vector<std::string>() const;

    /**
      @brief conversion operator to integer vector

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    operator std::vector<int>() const;

    /**
      @brief conversion operator to double vector

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    operator std::vector<double>() const;

    /**
      @brief conversion operator to long double

      Note: The implementation uses typedef double (as opposed to float, double, long double.)

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    operator long double() const;

    /**
      @brief conversion operator to double

      Note: The implementation uses typedef double (as opposed to float, double, long double.)

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    operator double() const;

    /**
      @brief conversion operator to float

      Note: The implementation uses typedef double (as opposed to float, double, long double.)

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    operator float() const;

    /**
      @brief conversion operator to short int

      Note: The implementation uses typedef ptrdiff_t.

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    operator short int() const;

    /**
      @brief conversion operator to unsigned short int

      Note: The implementation uses typedef ptrdiff_t.

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    operator unsigned short int() const;

    /**
      @brief conversion operator to int

      Note: The implementation uses typedef ptrdiff_t.

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */

    operator int() const;

    /**
      @brief conversion operator to unsigned int

      Note: The implementation uses typedef ptrdiff_t.

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    operator unsigned int() const;

    /**
      @brief conversion operator to long int

      Note: The implementation uses typedef ptrdiff_t.

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    operator long int() const;

    /**
      @brief conversion operator to unsigned long int

      Note: The implementation uses typedef ptrdiff_t.

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    operator unsigned long int() const;

    /**
      @brief conversion operator to long long

      Note: The implementation uses typedef ptrdiff_t.

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    operator long long() const;

    /**
      @brief conversion operator to unsigned long long

      Note: The implementation uses typedef ptrdiff_t.

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    operator unsigned long long() const;

    /**
      @brief Conversion to bool

      Converts the strings 'true' and 'false' to a bool.

      @exception Exception::ConversionError is thrown for non-string parameters and string parameters with values other than 'true' and 'false'.
    */
    bool toBool() const;

    /**
      @brief Convert ParamValues to char*

      If the ParamValue contains a string, a pointer to it's char* is returned.
      If the ParamValue is empty, nullptr is returned.
    */
    const char* toChar() const;

    /**
     * @brief Convert ParamValue to string
     *
     * @exception Exception::ConversionError is thrown for ParamValue::EMPTY and
     */

    std::string toString(bool full_precision = true) const;

    /**
      @brief Explicitly convert ParamValue to string vector

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    std::vector<std::string> toStringVector() const;

    /**
      @brief Explicitly convert ParamValue to IntList

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    std::vector<int> toIntVector() const;

    /**
      @brief Explicitly convert ParamValue to DoubleList

      @exception Exception::ConversionError is thrown if a cast from the the wrong type is requested
    */
    std::vector<double> toDoubleVector() const;
    //@}

    ///@name Assignment operators
    ///These methods are used to assign supported types directly to a ParamValue object.
    //@{
    /// Assignment operator
    ParamValue& operator=(const ParamValue&);
    /// Move assignment operator
    ParamValue& operator=(ParamValue&&) noexcept;
    /// specific assignment for char* (converted to string)
    ParamValue& operator=(const char*);
    /// specific assignment for std::string values
    ParamValue& operator=(const std::string&);
    /// specific assignment for string vectors
    ParamValue& operator=(const std::vector<std::string>&);
    /// specific assignment for integer vectors
    ParamValue& operator=(const std::vector<int>&);
    /// specific assignment for double vectors
    ParamValue& operator=(const std::vector<double>&);
    /// specific assignment for long double values (note: the implementation uses double)
    ParamValue& operator=(const long double);
    /// specific assignment for double values (note: the implementation uses double)
    ParamValue& operator=(const double);
    /// specific assignment for float values (note: the implementation uses double)
    ParamValue& operator=(const float);
    /// specific assignment for short int values (note: the implementation uses ptrdiff_t)
    ParamValue& operator=(const short int);
    /// specific assignment for unsigned short int values (note: the implementation uses ptrdiff_t)
    ParamValue& operator=(const unsigned short int);
    /// specific assignment for int values (note: the implementation uses ptrdiff_t)
    ParamValue& operator=(const int);
    /// specific assignment for unsigned int values (note: the implementation uses ptrdiff_t)
    ParamValue& operator=(const unsigned);
    /// specific assignment for long int values (note: the implementation uses ptrdiff_t)
    ParamValue& operator=(const long int);
    /// specific assignment for unsigned long int values (note: the implementation uses ptrdiff_t)
    ParamValue& operator=(const unsigned long);
    /// specific assignment for long long int values (note: the implementation uses ptrdiff_t)
    ParamValue& operator=(const long long);
    /// specific assignment for unsigned long long int values (note: the implementation uses ptrdiff_t)
    ParamValue& operator=(const unsigned long long);
    //@}

    /// returns the type of value stored
    inline ValueType valueType() const
    {
      return value_type_;
    }

    /**
       @brief Test if the value is empty

       @note A ParamValue containing an empty string ("") does not count as empty!
    */
    inline bool isEmpty() const
    {
      return value_type_ == EMPTY_VALUE;
    }

    /// output stream operator
    friend TDL_DLLAPI std::ostream& operator<<(std::ostream&, const ParamValue&);

    /// Equality comparator
    friend TDL_DLLAPI bool operator==(const ParamValue&, const ParamValue&);

    /// Smaller than comparator (for vectors we use the size)
    friend TDL_DLLAPI bool operator<(const ParamValue&, const ParamValue&);

    /// Greater than comparator (for vectors we use the size)
    friend TDL_DLLAPI bool operator>(const ParamValue&, const ParamValue&);

    /// Equality comparator
    friend TDL_DLLAPI bool operator!=(const ParamValue&, const ParamValue&);

protected:

    /// Type of the currently stored value
    ValueType value_type_;

    /// Space to store the data
    union
    {
      ptrdiff_t ssize_;
      double dou_;
      std::string* str_;
      std::vector<std::string>* str_list_;
      std::vector<int>* int_list_;
      std::vector<double>* dou_list_;
    } data_;

private:

    /// Clears the current state of the ParamValue and release every used memory.
    void clear_() noexcept;

    /// Convert a double to std::string
    /// with full precision 15 decimal places are given, otherwise 3
    /// numbers above 10000 or below 0.0001 are given in scientific notation (i.e. 1.0e04)
    static std::string doubleToString(double value, bool full_precision = true);

  };

  const ParamValue ParamValue::EMPTY;

  // default ctor
  ParamValue::ParamValue() :
    value_type_(EMPTY_VALUE)
  {
  }

  // destructor
  ParamValue::~ParamValue()
  {
    clear_();
  }

  //-------------------------------------------------------------------
  //    ctor for all supported types a ParamValue object can hold
  //--------------------------------------------------------------------
  ParamValue::ParamValue(long double p) :
    value_type_(DOUBLE_VALUE)
  {
    data_.dou_ = p;
  }

  ParamValue::ParamValue(double p) :
    value_type_(DOUBLE_VALUE)
  {
    data_.dou_ = p;
  }

  ParamValue::ParamValue(float p) :
    value_type_(DOUBLE_VALUE)
  {
    data_.dou_ = p;
  }

  ParamValue::ParamValue(short int p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  ParamValue::ParamValue(unsigned short int p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  ParamValue::ParamValue(int p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  ParamValue::ParamValue(unsigned int p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  ParamValue::ParamValue(long int p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  ParamValue::ParamValue(unsigned long int p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  ParamValue::ParamValue(long long p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  ParamValue::ParamValue(unsigned long long p) :
    value_type_(INT_VALUE)
  {
    data_.ssize_ = p;
  }

  ParamValue::ParamValue(const char* p) :
    value_type_(STRING_VALUE)
  {
    data_.str_ = new std::string(p);
  }

  ParamValue::ParamValue(const std::string& p) :
    value_type_(STRING_VALUE)
  {
    data_.str_ = new std::string(p);
  }

  ParamValue::ParamValue(const std::vector<std::string>& p) :
    value_type_(STRING_LIST)
  {
    data_.str_list_ = new std::vector<std::string>(p);
  }

  ParamValue::ParamValue(const std::vector<int>& p) :
    value_type_(INT_LIST)
  {
    data_.int_list_ = new std::vector<int>(p);
  }

  ParamValue::ParamValue(const std::vector<double>& p) :
    value_type_(DOUBLE_LIST)
  {
    data_.dou_list_ = new std::vector<double>(p);
  }

  //--------------------------------------------------------------------
  //                   copy and move constructors
  //--------------------------------------------------------------------
  ParamValue::ParamValue(const ParamValue& p) :
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

  ParamValue::ParamValue(ParamValue&& rhs) noexcept :
    value_type_(std::move(rhs.value_type_)),
    data_(std::move(rhs.data_))
  {
    // clean up rhs, take ownership of data_
    // NOTE: value_type_ == EMPTY_VALUE implies data_ is empty and can be reset
    rhs.value_type_ = EMPTY_VALUE;
  }

  void ParamValue::clear_() noexcept
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
  ParamValue& ParamValue::operator=(const ParamValue& p)
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

  /// Move assignment operator
  ParamValue& ParamValue::operator=(ParamValue&& rhs) noexcept
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

  ParamValue& ParamValue::operator=(const char* arg)
  {
    clear_();
    data_.str_ = new std::string(arg);
    value_type_ = STRING_VALUE;
    return *this;
  }

  ParamValue& ParamValue::operator=(const std::string& arg)
  {
    clear_();
    data_.str_ = new std::string(arg);
    value_type_ = STRING_VALUE;
    return *this;
  }

  ParamValue& ParamValue::operator=(const std::vector<std::string>& arg)
  {
    clear_();
    data_.str_list_ = new std::vector<std::string>(arg);
    value_type_ = STRING_LIST;
    return *this;
  }

  ParamValue& ParamValue::operator=(const std::vector<int>& arg)
  {
    clear_();
    data_.int_list_ = new std::vector<int>(arg);
    value_type_ = INT_LIST;
    return *this;
  }

  ParamValue& ParamValue::operator=(const std::vector<double>& arg)
  {
    clear_();
    data_.dou_list_ = new std::vector<double>(arg);
    value_type_ = DOUBLE_LIST;
    return *this;
  }

  ParamValue& ParamValue::operator=(const long double arg)
  {
    clear_();
    data_.dou_ = arg;
    value_type_ = DOUBLE_VALUE;
    return *this;
  }

  ParamValue& ParamValue::operator=(const double arg)
  {
    clear_();
    data_.dou_ = arg;
    value_type_ = DOUBLE_VALUE;
    return *this;
  }

  ParamValue& ParamValue::operator=(const float arg)
  {
    clear_();
    data_.dou_ = arg;
    value_type_ = DOUBLE_VALUE;
    return *this;
  }

  ParamValue& ParamValue::operator=(const short int arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  ParamValue& ParamValue::operator=(const unsigned short int arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  ParamValue& ParamValue::operator=(const int arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  ParamValue& ParamValue::operator=(const unsigned arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  ParamValue& ParamValue::operator=(const long int arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  ParamValue& ParamValue::operator=(const unsigned long arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  ParamValue& ParamValue::operator=(const long long arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  ParamValue& ParamValue::operator=(const unsigned long long arg)
  {
    clear_();
    data_.ssize_ = arg;
    value_type_ = INT_VALUE;
    return *this;
  }

  //---------------------------------------------------------------------------
  //                      Conversion operators
  //----------------------------------------------------------------------------
  ParamValue::operator long double() const
  {
    if (value_type_ == EMPTY_VALUE)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert ParamValue::EMPTY to long double");
#else
        throw std::invalid_argument{"Could not convert ParamValue::EMPTY to long double"};
#endif
    }
    else if (value_type_ == INT_VALUE)
    {
      return (long double)(data_.ssize_);
    }
    return data_.dou_;
  }

  ParamValue::operator double() const
  {
    if (value_type_ == EMPTY_VALUE)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert ParamValue::EMPTY to double");
#else
        throw std::invalid_argument{"Could not convert ParamValue::EMPTY to double"};
#endif
    }
    else if (value_type_ == INT_VALUE)
    {
      return double(data_.ssize_);
    }
    return data_.dou_;
  }

  ParamValue::operator float() const
  {
    if (value_type_ == EMPTY_VALUE)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert ParamValue::EMPTY to float");
#else
        throw std::invalid_argument{"Could not convert ParamValue::EMPTY to float"};
#endif
    }
    else if (value_type_ == INT_VALUE)
    {
      return float(data_.ssize_);
    }
    return data_.dou_;
  }

  ParamValue::operator short int() const
  {
    if (value_type_ != INT_VALUE)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert non-integer ParamValue to short int");
#else
        throw std::invalid_argument{"Could not convert non-integer ParamValue to short int"};
#endif
    }
    return data_.ssize_;
  }

  ParamValue::operator unsigned short int() const
  {
    if (value_type_ != INT_VALUE)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert non-integer ParamValue to UInt");
#else
        throw std::invalid_argument{"Could not convert non-integer ParamValue to UInt"};
#endif
    }
    if (data_.ssize_ < 0.0)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert negative integer ParamValue to unsigned short int");
#else
        throw std::invalid_argument{"Could not convert negative integer ParamValue to unsigned short int"};
#endif
    }
    return data_.ssize_;
  }

  ParamValue::operator int() const
  {
    if (value_type_ != INT_VALUE)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert non-integer ParamValue to int");
#else
        throw std::invalid_argument{"Could not convert non-integer ParamValue to int"};
#endif
    }
    return data_.ssize_;
  }

  ParamValue::operator unsigned int() const
  {
    if (value_type_ != INT_VALUE)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert non-integer ParamValue to unsigned int");
#else
        throw std::invalid_argument{"Could not convert non-integer ParamValue to unsigned int"};
#endif
    }
    if (data_.ssize_ < 0.0)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert negative integer ParamValue to unsigned int");
#else
        throw std::invalid_argument{"Could not convert negative integer ParamValue to unsigned int"};
#endif
    }
    return data_.ssize_;
  }

  ParamValue::operator long int() const
  {
    if (value_type_ != INT_VALUE)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert non-integer ParamValue to long int");
#else
        throw std::invalid_argument{"Could not convert non-integer ParamValue to long int"};
#endif
    }
    return data_.ssize_;
  }

  ParamValue::operator unsigned long int() const
  {
    if (value_type_ != INT_VALUE)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert non-integer ParamValue to unsigned long int");
#else
        throw std::invalid_argument{"Could not convert non-integer ParamValue to unsigned long int"};
#endif
    }
    if (data_.ssize_ < 0.0)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert negative integer ParamValue to unsigned long int");
#else
        throw std::invalid_argument{"Could not convert negative integer ParamValue to unsigned long int"};
#endif
    }
    return data_.ssize_;
  }

  ParamValue::operator long long() const
  {
    if (value_type_ != INT_VALUE)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert non-integer ParamValue to Int");
#else
        throw std::invalid_argument{"Could not convert non-integer ParamValue to Int"};
#endif
    }
    return data_.ssize_;
  }

  ParamValue::operator unsigned long long() const
  {
    if (value_type_ != INT_VALUE)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert non-integer ParamValue to UInt");
#else
        throw std::invalid_argument{"Could not convert non-integer ParamValue to UInt"};
#endif
    }
    if (data_.ssize_ < 0.0)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert negative integer ParamValue to UInt");
#else
        throw std::invalid_argument{"Could not convert negative integer ParamValue to UInt"};
#endif
    }
    return data_.ssize_;
  }

  ParamValue::operator std::string() const
  {
    if (value_type_ != STRING_VALUE)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert non-string ParamValue to string");
#else
        throw std::invalid_argument{"Could not convert non-string ParamValue to string"};
#endif
    }
    return *(data_.str_);
  }

  ParamValue::operator std::vector<std::string>() const
  {
    return this->toStringVector();
  }

  ParamValue::operator std::vector<int>() const
  {
    return this->toIntVector();
  }

  ParamValue::operator std::vector<double>() const
  {
    return this->toDoubleVector();
  }

  // Convert ParamValues to char*
  const char* ParamValue::toChar() const
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
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
        throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert non-string ParamValue to char*");
#else
        throw std::invalid_argument{"Could not convert non-string ParamValue to char*"};
#endif
    break;
    }
  }

  std::string ParamValue::toString(bool full_precision) const
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
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
        throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert ParamValue to String");
#else
        throw std::invalid_argument{"Could not convert ParamValue to String"};
#endif
    break;
    }
    return str;
  }

  std::vector<std::string> ParamValue::toStringVector() const
  {
    if (value_type_ != STRING_LIST)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert non-std::vector<std::string> ParamValue to std::vector<std::string>");
#else
        throw std::invalid_argument{"Could not convert non-std::vector<std::string> ParamValue to std::vector<std::string>"};
#endif
    }
    return *(data_.str_list_);
  }

  std::vector<int> ParamValue::toIntVector() const
  {
    if (value_type_ != INT_LIST)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert non-std::vector<int> ParamValue to std::vector<int>");
#else
        throw std::invalid_argument{"Could not convert non-std::vector<int> ParamValue to std::vector<int>"};
#endif
    }
    return *(data_.int_list_);
  }

  std::vector<double> ParamValue::toDoubleVector() const {
    if (value_type_ != DOUBLE_LIST)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert non-std::vector<double> ParamValue to std::vector<double>");
#else
        throw std::invalid_argument{"Could not convert non-std::vector<double> ParamValue to std::vector<double>"};
#endif
    }
    return *(data_.dou_list_);
  }

  bool ParamValue::toBool() const
  {
    if (value_type_ != STRING_VALUE)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert non-string ParamValue to bool.");
#else
        throw std::invalid_argument{"Could not convert non-string ParamValue to bool."};
#endif
    }
    else if (!(*(data_.str_) == "true" || *(data_.str_) == "false"))
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ConversionError(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Could not convert '" + *(data_.str_) + "' to bool. Valid stings are 'true' and 'false'.");
#else
        throw std::invalid_argument{"Could not convert '" + *(data_.str_) + "' to bool. Valid stings are 'true' and 'false'."};
#endif
    }

    return *(data_.str_) == "true";
  }

  // ----------------- Comparator ----------------------

  bool operator==(const ParamValue& a, const  ParamValue& b)
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

  bool operator<(const ParamValue& a, const  ParamValue& b)
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

  bool operator>(const ParamValue& a, const  ParamValue& b)
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

  bool operator!=(const ParamValue& a, const ParamValue& b)
  {
    return !(a == b);
  }

  // ----------------- Output operator ----------------------

  /// for doubles or lists of doubles, you get full precision. Use ParamValue::toString(false) if you only need low precision
  std::ostream& operator<<(std::ostream& os, const ParamValue& p)
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

  std::string ParamValue::doubleToString(double value, bool full_precision)
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
