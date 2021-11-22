// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/deNBI-cibi/tool_description_lib/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides tdl::Param.
 */

#pragma once

#include <algorithm>
#include <limits>
#include <map>
#include <set>
#include <string>

#include <tdl/ParamValue.h>

namespace tdl
{

  /*!\namespace Logger
   * \brief The Logger namespace.
   */
  namespace Logger
  {
    class LogStream;
  }

  /*!\brief Management and storage of parameters / INI files.
   * \ingroup Datastructures
   * \details
   * This class provides a means to associate string names to int/double/string/StringList values.
   * It allows for parameter hierarchies and to save/load the data as XML.
   *
   * * Hierarchy levels are separated from each other by colons.
   * * Each parameter and section has a description. Newline characters in the description are possible.
   * * Each parameter can be annotated with an arbitrary number of tags. Tags must not contain comma characters!
   *   For example, the <i>advanced</i> tag indicates if this parameter is shown to all users or in advanced mode only.
   *
   * Example: 'common:file_options:default_file_open_path = /share/'
   *
   * \see DefaultParamHandler
   */
  class TDL_DLLAPI Param
  {
  public:

    //!\brief Representation of a single character
    struct TDL_DLLAPI ParamEntry
    {
      ParamEntry() = default; //!< Defaulted.
      ParamEntry(ParamEntry const &) = default; //!< Defaulted.
      ParamEntry(ParamEntry &&) = default; //!< Defaulted.
      ParamEntry & operator=(ParamEntry const &) = default; //!< Defaulted.
      ParamEntry & operator=(ParamEntry &&) = default; //!< Defaulted.
      ~ParamEntry() = default; //!< Defaulted.

      //!\brief Constructs an entry from name, description, value, and advanced flag.
      ParamEntry(const std::string& n, const ParamValue& v, const std::string& d, const std::vector<std::string>& t = std::vector<std::string>());

      //!\brief Checks if 'value' fulfills restrictions.
      bool isValid(std::string& message) const;
      //!\brief Equality operator (only name and value are compared).
      bool operator==(const ParamEntry& rhs) const;

      std::string name{}; //!< Name of the entry.
      std::string description{}; //!< Description of the entry.
      ParamValue value{}; //!< Value associated with the entry.
      std::set<std::string> tags{}; //!< Tags list, for example, advanced parameter tag.

      /*!\name Restrictions to accepted values (used in checkDefaults).
       * \{
       */
      double min_float{-std::numeric_limits<double>::max()}; //!< Default: - std::numeric_limits<double>::max()
      double max_float{std::numeric_limits<double>::max()}; //!< Default: std::numeric_limits<double>::max()
      int min_int{-std::numeric_limits<int>::max()}; //!< Default: - std::numeric_limits<int>::max()
      int max_int{std::numeric_limits<int>::max()}; //!< Default: std::numeric_limits<int>::max()
      std::vector<std::string> valid_strings{}; //!< Default: empty
      //!\}
    };

    //!\brief Node inside a Param object which is used to build the internal tree.
    struct TDL_DLLAPI ParamNode
    {
      //!\brief Iterator for child nodes.
      typedef std::vector<ParamNode>::iterator NodeIterator;
      //!\brief Iterator for entries.
      typedef std::vector<ParamEntry>::iterator EntryIterator;
      //!\brief Iterator for child nodes.
      typedef std::vector<ParamNode>::const_iterator ConstNodeIterator;
      //!\brief Iterator for entries.
      typedef std::vector<ParamEntry>::const_iterator ConstEntryIterator;

      ParamNode() = default; //!< Defaulted.
      ParamNode(ParamNode const &) = default; //!< Defaulted.
      ParamNode(ParamNode &&) = default; //!< Defaulted.
      ParamNode & operator=(ParamNode const &) = default; //!< Defaulted.
      ParamNode & operator=(ParamNode &&) = default; //!< Defaulted.
      ~ParamNode() = default; //!< Defaulted.

      //!\brief Construct from name and description.
      ParamNode(const std::string& n, const std::string& d);

      //!\brief Equality operator (name, entries and subnodes are compared).
      bool operator==(const ParamNode& rhs) const;

      /*!\brief Looks up entry of this node (local search).
       * \returns Iterator to the entry, or the end iterator if no entry is found.
       */
      EntryIterator findEntry(const std::string& name);

      /*!\brief Looks up subnode of this node (local search).
       * \returns Iterator to the subnode, or the end iterator if no subnode is found.
       */
      NodeIterator findNode(const std::string& name);

      /*!\brief Look up the parent node of the entry or node corresponding to `name` (tree search).
       * \returns Pointer to the parent node, or `nullptr` if no parent node is found.
       */
      ParamNode* findParentOf(const std::string& name);

      /*!\brief Look up entry by name (tree search).
       * \returns Pointer to the entry, or `nullptr` if no entry is found.
       */
      ParamEntry* findEntryRecursive(const std::string& name);

      //!\brief Inserts a node with the given prefix.
      void insert(const ParamNode& node, const std::string& prefix = "");

      //!\brief Inserts an entry with the given prefix.
      void insert(const ParamEntry& entry, const std::string& prefix = "");

      //!\brief Returns the number of entries in the whole subtree.
      size_t size() const;

      //!\brief Returns the name suffix of a key (the part behind the last ':' character).
      std::string suffix(const std::string& key) const;

      std::string name{}; //!< Name of the node.
      std::string description{}; //!< Description of the node.
      std::vector<ParamEntry> entries{}; //!< Entries (leafs) in the node.
      std::vector<ParamNode> nodes{}; //!< Subnodes.
    };

  public:

    //!\brief The ParamIterator.
    class TDL_DLLAPI ParamIterator
    {
    public:
      //!\brief Stores information about left/entered nodes.
      struct TDL_DLLAPI TraceInfo
      {
        TraceInfo() = default; //!< Defaulted.
        TraceInfo(TraceInfo const &) = default; //!< Defaulted.
        TraceInfo(TraceInfo &&) = default; //!< Defaulted.
        TraceInfo & operator=(TraceInfo const &) = default; //!< Defaulted.
        TraceInfo & operator=(TraceInfo &&) = default; //!< Defaulted.
        ~TraceInfo() = default; //!< Defaulted.

        //!\brief Construct from name, description, and open flag.
        inline TraceInfo(const std::string& n, const std::string& d, bool o) :
          name(n),
          description(d),
          opened(o)
        {
        }

        std::string name{}; //!< Name of the node.
        std::string description{}; //!< Description of the node.
        bool opened{}; //!< Was the node open?
      };

      ParamIterator() = default; //!< Defaulted.
      ParamIterator(ParamIterator const &) = default; //!< Defaulted.
      ParamIterator(ParamIterator &&) = default; //!< Defaulted.
      ParamIterator & operator=(ParamIterator const &) = default; //!< Defaulted.
      ParamIterator & operator=(ParamIterator &&) = default; //!< Defaulted.
      ~ParamIterator() = default; //!< Defaulted.

      //!\brief Construct from a node.
      ParamIterator(const Param::ParamNode& root);

      //!\brief Dereference.
      const Param::ParamEntry& operator*();

      //!\brief Dereference.
      const Param::ParamEntry* operator->();

      //!\brief Prefix increment operator.
      ParamIterator& operator++();

      //!\brief Postfix increment operator.
      ParamIterator operator++(int);

      //!\brief Equality operator.
      bool operator==(const ParamIterator& rhs) const;

      //!\brief Inequality operator.
      bool operator!=(const ParamIterator& rhs) const;

      //!\brief Returns the absolute path of the current element (including all sections).
      std::string getName() const;

      //!\brief Returns the traceback of the opened and closed sections.
      const std::vector<TraceInfo>& getTrace() const;

  protected:
      const Param::ParamNode* root_{nullptr}; //!< Pointer to the root node.
      int current_{}; //!< Index of the current ParamEntry (-1 means invalid).
      std::vector<const Param::ParamNode*> stack_{}; //!< Pointers to the ParamNodes we are in.
      std::vector<TraceInfo> trace_{}; //!< Node traversal data during last ++ operation.

    };

    Param() = default; //!< Defaulted.
    Param(Param const &) = default; //!< Defaulted.
    Param(Param &&) = default; //!< Defaulted.
    Param & operator=(Param const &) = default; //!< Defaulted.
    Param & operator=(Param &&) = default; //!< Defaulted.
    ~Param() = default; //!< Defaulted.

    //!\brief Equality operator.
    bool operator==(const Param& rhs) const;

    //!\brief Begin iterator for the internal tree.
    ParamIterator begin() const;

    //!\brief End iterator for the internal tree.
    ParamIterator end() const;

    /*!\name Accessors for single parameters
     * \{
     */
    /*!\brief Sets a value.
     * \param key String key. Can contain ':' which separates section names.
     * \param value The actual value.
     * \param description Verbose description of the parameter.
     * \param tags list of tags associated to this parameter.
     */
    void setValue(const std::string& key, const ParamValue& value, const std::string& description = "", const std::vector<std::string>& tags = std::vector<std::string>());

    /*!\brief Returns a value of a parameter.
     * \throws Exception::ElementNotFound if the parameter does not exist.
     */
    const ParamValue& getValue(const std::string& key) const;

    /*!\brief Returns the type of a parameter.
     * \throws Exception::ElementNotFound if the parameter does not exist.
     */
    ParamValue::ValueType getValueType(const std::string& key) const;

    /*!\brief Returns the whole parameter entry.
     * \throws Exception::ElementNotFound if the parameter does not exist.
     */
    const ParamEntry& getEntry(const std::string& key) const;

    /*!\brief Tests if a parameter is set (expecting its fully qualified name, e.g., TextExporter:1:proteins_only)
     * \param key The fully qualified name of the parameter to check.
     * \returns True if the parameter exists, false otherwise.
    */
    bool exists(const std::string& key) const;

    /*!\brief Checks whether a section is present.
     * \param key The key of the section to be searched for. May or may not contain ":" suffix.
     * \returns True if the section exists, false otherwise.
     */
    bool hasSection(const std::string& key) const;

    /*!\brief Find leaf node by name.
     * \param leaf The name of the parameter to find excluding the path parameter, e.g., given the parameter TextExporter:1:proteins_only the leaf would be named proteins_only.
     * \returns Iterator to the leaf node, or `end()` if not found.
     */
    ParamIterator findFirst(const std::string& leaf) const;

    /*!\brief Find next leaf node by name, not considering the `start_leaf`.
     * \param leaf The name of the parameter to find excluding the path parameter, e.g., given the parameter TextExporter:1:proteins_only the leaf would be named proteins_only.
     * \param start_leaf The already found leaf that should not be considered during this search.
     * \returns Iterator to the leaf node, or `end()` if not found.
     */
    ParamIterator findNext(const std::string& leaf, const ParamIterator& start_leaf) const;
    //!\}

    /*!\name Tags handling
     * \{
     */

    /*!\brief Adds a tag to an entry.
     * \throws Exception::ElementNotFound if the parameter does not exist.
     * \throws Exception::InvalidValue if the tag contain a comma character.
     * \details
     * Example of a tag: "advanced", "required", "input file", "output file"
     */
    void addTag(const std::string& key, const std::string& tag);

    /*!\brief Appends a list of tags to an entry.
     * \throws Exception::ElementNotFound if the parameter does not exist.
     * \throws Exception::InvalidValue if a tag contains a comma character.
     */
    void addTags(const std::string& key, const std::vector<std::string>& tags);

    /*!\brief Returns whether an entry has a tag.
     * \throws Exception::ElementNotFound if the parameter does not exist.
     * \details
     * Example: The tag 'advanced' is used in the GUI to determine which parameters are always displayed
     * and which parameters are displayed only in 'advanced mode'.
     */
    bool hasTag(const std::string& key, const std::string& tag) const;

    /*!\brief Returns the tags of an entry.
     * \throws Exception::ElementNotFound if the parameter does not exist.
     */
    std::vector<std::string> getTags(const std::string& key) const;

    /*!\brief Removes all tags from an etry.
     * \throws Exception::ElementNotFound if the parameter does not exist.
     */
    void clearTags(const std::string& key);
    //!\}


    /*!\name Descriptions handling
     * \{
     */

    /*!\brief Returns the description of a parameter.
     * \throws Exception::ElementNotFound if the parameter does not exist.
     */
    const std::string& getDescription(const std::string& key) const;

    /*!\brief Sets a description for an existing section.
     * \throws Exception::ElementNotFound if the section does not exist.
     * \details
     * Descriptions for values cannot be set with this method.
     * They have to be set when inserting the value itself.
     */
    void setSectionDescription(const std::string& key, const std::string& description);

    /*!\brief Returns the description corresponding to a given section.
     * \returns The description, or an empty string if the section does not exist.
     */
    const std::string& getSectionDescription(const std::string& key) const;

    /*!\brief Adds a parameter section.
     * \details
     * Non-empty descriptions of existing section will not be overwritten.
     */
    void addSection(const std::string& key, const std::string& description);
    //!\}

    /*!\name Manipulation of the whole parameter set
     * \{
     */

    //!\brief Returns the number of entries (leafs).
    size_t size() const;

    //!\brief Returns whether there are no entries.
    bool empty() const;

    //!\brief Deletes all entries.
    void clear();

    /*!\brief Inserts all values of a parameter and prepends a prefix.
     * \details
     * Appending `:` to the prefix will cause it to be a section.
     */
    void insert(const std::string& prefix, const Param& param);

    /*!\brief Remove an entry or a section (when suffix is ':').
     * \details
     * No partial matches are accepted.
     * If an empty internal node remains, the tree is pruned until every node has either a successor node
     * or a leaf, i.e. no naked nodes remain.
     */
    void remove(const std::string& key);

    /*!\brief Remove all entries that starting with a prefix.
     * \details
     * Partial matches are valid. All entries and sections which match the prefix are deleted.
     * If an empty internal node remains, the tree is pruned until every node has either a successor node
     * or a leaf, i.e. no naked nodes remain.
     */
    void removeAll(const std::string& prefix);

    /*!\brief Returns a new Param object containing all entries that start with a given prefix.
     * \param prefix Should end in ':' to extract a subtree. Otherwise, not only nodes, but also values with that
     *               prefix are copied.
     * \param remove_prefix Indicates whether the prefix is removed before adding entries to the new Param.
     */
    Param copy(const std::string& prefix, bool remove_prefix = false) const;

    /*!\brief Returns a new Param object containing all entries in the given subset.
     * \param subset The subset of Param nodes that should be copied from the object.
     *               Includes values etc. Does not check any compatibility. Just matches the names.
     * \note Only matches entries and nodes at the root=top level and copies whole subtrees if matched.
     *       This function is mainly used for copying subsection parameters that were not registered as
     *       as an actual subsection e.g. for backwards compatibility of param names.
     */
    Param copySubset(const Param& subset) const;

    /*!\brief Rescue parameter <b>values</b> from an outdated param to this.
     * \details
     * Calls \ref update(const Param&, bool, bool, bool, bool, TDL_LOGSTREAM_TYPE&)
     * "update(p_outdated, true, add_unknown, false, false, TDL_LOGSTREAM_WARN)" and returns its value.
     */
    bool update(const Param& p_outdated, const bool add_unknown = false);

    /*!\brief Rescue parameter <b>values</b> from an outdated param to this.
     * \details
     * Calls \ref update(const Param&, bool, bool, bool, bool, TDL_LOGSTREAM_TYPE&)
     * "update(p_outdated, true, add_unknown, false, false, stream)" and returns its value.
     */
    bool update(const Param& p_outdated, const bool add_unknown, TDL_LOGSTREAM_TYPE& stream);

    /*!\brief Rescue parameter <b>values</b> from an outdated param to this.
     * \param p_outdated Old/outdated param object whose valid values are used to update this object.
     * \param verbose Print information about expected value updates.
     * \param add_unknown Add unknown parameters from old param to this param object.
     * \param fail_on_invalid_values Return false if outdated parameters hold invalid values.
     * \param fail_on_unknown_parameters Return false if outdated parameters contain unknown parameters (takes precedence over add_unknown).
     * \param stream The stream where all the logging output is sent to.
     * \returns true on success, false on failure.
     * \details
     * All parameters present in both param objects will be transferred into this object, given that:
     * <ul>
     *   <li>the name is equal</li>
     *   <li>the type is equal</li>
     *   <li>the value from p_outdated meets the new restrictions</li>
     * </ul>
     * Not transferred are values from parameter "version" (to preserve the new version) or "type" (to preserve layout).
     */
    bool update(const Param& p_outdated, bool verbose, bool add_unknown, bool fail_on_invalid_values, bool fail_on_unknown_parameters, TDL_LOGSTREAM_TYPE& stream);

    /*!\brief Adds missing parameters from the given param to this param. Existing parameters will not be modified.
     * \param toMerge The Param object from which parameters should be added to this param.
     */
    void merge(const Param& toMerge);
    //!\}

    /*!\name Default value handling
     * \{
     */

    /*!\brief Adds defaults for missing values and prepends an prefix.
     * \param defaults The default values.
     * \param prefix The prefix to add to all defaults.
     * \param showMessage If <tt>true</tt>, each default that is actually set is printed to stdout as well.
     * \see checkDefaults
     */
    void setDefaults(const Param& defaults, const std::string& prefix = "", bool showMessage = false);

    /*!\brief Validates the current parameter entries against defaults.
     * \param name The name that is used in error messages.
     * \param defaults The default values.
     * \param prefix The prefix where to check for the defaults.
     * \throws Exception::InvalidParameter if errors occur during the check.
     * \details
     * Several checks are performed:
     * - Existence of parameters with no defaults: warning.
     * - Mismatch of parameter type and its default type: exception.
     * - String/String-list parameters containing invalid characters: exception.
     * - Numeric/Numeric-list parameters out of valid range: exception.
     *
     * Warnings etc. will be sent to TDL_LOGSTREAM_WARN.
     */
    void checkDefaults(const std::string& name, const Param& defaults, const std::string& prefix = "") const;
    //!\}

    /*!\name Restriction handling
     * \{
     */

    /*!\brief Sets the valid strings for a parameter.
     * \throws Exception::InvalidParameter if one of the strings contains a comma character.
     * \throws Exception::ElementNotFound if the parameter is no string parameter.
     * \see checkDefaults()
     */
    void setValidStrings(const std::string& key, const std::vector<std::string>& strings);

    /*!\brief Sets the minimum value for the integer or integer list parameter.
     * \throws Exception::ElementNotFound if parameter is not found or the parameter type is wrong.
     * \see checkDefaults()
     */
    void setMinInt(const std::string& key, int min);

    /*!\brief Sets the maximum value for the integer or integer list parameter.
     * \throws Exception::ElementNotFound if parameter is not found or the parameter type is wrong.
     * \see checkDefaults()
     */
    void setMaxInt(const std::string& key, int max);

    /*!\brief Sets the minimum value for the floating point or floating point list parameter.
     * \throws Exception::ElementNotFound if parameter is not found or the parameter type is wrong.
     * \see checkDefaults()
     */
    void setMinFloat(const std::string& key, double min);

    /*!\brief Sets the maximum value for the floating point or floating point list parameter.
     * \throws Exception::ElementNotFound if parameter is not found or the parameter type is wrong.
     * \see checkDefaults()
     */
    void setMaxFloat(const std::string& key, double max);
    //!\}

    /*!\name Command line parsing
     * \{
     */

    /*!\brief Parses command line arguments.
     * \param argc argc variable from command line.
     * \param argv argv variable from command line.
     * \param prefix prefix for all options.
     * \details
     * This method distinguishes three types of arguments:
     * * options (starting with '-') that have a text argument
     * * options (starting with '-') that have no text argument
     * * text arguments (not starting with '-')
     *
     * Command line arguments '-a avalue -b -c bvalue misc1 misc2' would be stored like this:
     * * "prefix:-a" -> "avalue"
     * * "prefix:-b" -> ""
     * * "prefix:-c" -> "bvalue"
     * * "prefix:misc" -> list("misc1","misc2")
     */
    void parseCommandLine(const int argc, const char** argv, const std::string& prefix = "");

    /*!\brief Parses command line arguments to specified key locations.
     * \param argc argc variable from command line.
     * \param argv argv variable from command line.
     * \param options_with_one_argument a map of options that are followed by one argument (with key where they are stored).
     * \param options_without_argument a map of options that are not followed by an argument (with key where they are stored). Options specified on the command line are set to the string 'true'.
     * \param options_with_multiple_argument a map of options that are followed by several arguments (with key where they are stored).
     * \param misc key where a StringList of all non-option arguments are stored.
     * \param unknown key where a StringList of all unknown options are stored.
    */
    void parseCommandLine(const int argc, const char** argv, const std::map<std::string, std::string>& options_with_one_argument, const std::map<std::string, std::string>& options_without_argument, const std::map<std::string, std::string>& options_with_multiple_argument, const std::string& misc = "misc", const std::string& unknown = "unknown");
    //!\}

  protected:
    /*!\brief Returns a mutable reference to a parameter entry.
     * \throws Exception::ElementNotFound for unset parameters.
     */
    ParamEntry& getEntry_(const std::string& key) const;

    //!\brief Constructor from a node which is used as root node.
    Param(const Param::ParamNode& node);

    //!\brief Invisible root node that stores all the data.
    mutable Param::ParamNode root_{"ROOT", ""};
  };

  //!\brief Formatted output.
  TDL_DLLAPI std::ostream& operator<<(std::ostream& os, const Param& param);

  //********************************* ParamEntry **************************************

  Param::ParamEntry::ParamEntry(const std::string& n, const ParamValue& v, const std::string& d, const std::vector<std::string>& t) :
    name(n),
    description(d),
    value(v),
    tags(),
    min_float(-std::numeric_limits<double>::max()),
    max_float(std::numeric_limits<double>::max()),
    min_int(-std::numeric_limits<int>::max()),
    max_int(std::numeric_limits<int>::max()),
    valid_strings()
  {
    //add tags
    for (size_t i = 0; i < t.size(); ++i)
    {
      tags.insert(t[i]);
    }
    //check name
    if (name.find(':') != std::string::npos)
    {
      std::cerr << "Error ParamEntry name must not contain ':' characters!" << std::endl;
    }
  }


  bool Param::ParamEntry::isValid(std::string& message) const
  {
    if (value.valueType() == ParamValue::STRING_VALUE)
    {
      if (valid_strings.size() != 0)
      {
        bool ok = false;
        if (std::find(valid_strings.begin(), valid_strings.end(), value) != valid_strings.end())
        {
          ok = true;
        }
        else if (std::find(tags.begin(), tags.end(), "input file") != tags.end() || std::find(tags.begin(), tags.end(), "output file") != tags.end())
        {
          //do not check restrictions on file names for now
          ok = true;
        }

        if (!ok)
        {
          std::string valid = valid_strings.front();
          for (auto it = valid_strings.begin() + 1, end = valid_strings.end(); it != end; ++it) {
              valid += "," + *it;
          }
          message = "Invalid string parameter value '" + value.toString() + "' for parameter '" + name + "' given! Valid values are: '" + valid + "'.";
          return false;
        }
      }
    }
    else if (value.valueType() == ParamValue::STRING_LIST)
    {
      std::string str_value;
      std::vector<std::string> ls_value = value;
      for (size_t i = 0; i < ls_value.size(); ++i)
      {
        str_value = ls_value[i];

        if (valid_strings.size() != 0)
        {
          bool ok = false;
          if (std::find(valid_strings.begin(), valid_strings.end(), str_value) != valid_strings.end())
          {
            ok = true;
          }
          else if (std::find(tags.begin(), tags.end(), "input file") != tags.end() || std::find(tags.begin(), tags.end(), "output file") != tags.end())
          {
            //do not check restrictions on file names for now
            ok = true;
          }

          if (!ok)
          {
            std::string valid = valid_strings.front();
            for (auto it = valid_strings.begin() + 1, end = valid_strings.end(); it != end; ++it) {
                valid += "," + *it;
            }
            message = "Invalid string parameter value '" + str_value + "' for parameter '" + name + "' given! Valid values are: '" + valid + "'.";
            return false;
          }
        }
      }
    }
    else if (value.valueType() == ParamValue::INT_VALUE)
    {
      int tmp = value;
      if ((min_int != -std::numeric_limits<int>::max() && tmp < min_int) || (max_int != std::numeric_limits<int>::max() && tmp > max_int))
      {
        message = "Invalid integer parameter value '" + std::to_string(tmp) + "' for parameter '" + name + "' given! The valid range is: [" + std::to_string(min_int) + ":" + std::to_string(max_int) + "].";
        return false;
      }
    }
    else if (value.valueType() == ParamValue::INT_LIST)
    {
      int int_value;
      std::vector<int> ls_value = value;
      for (size_t i = 0; i < ls_value.size(); ++i)
      {
        int_value = ls_value[i];
        if ((min_int != -std::numeric_limits<int>::max() && int_value < min_int) || (max_int != std::numeric_limits<int>::max() && int_value > max_int))
        {
          message = "Invalid integer parameter value '" + std::to_string(int_value) + "' for parameter '" + name + "' given! The valid range is: [" + std::to_string(min_int) + ":" + std::to_string(max_int) + "].";
          return false;
        }
      }
    }
    else if (value.valueType() == ParamValue::DOUBLE_VALUE)
    {
      double tmp = value;
      if ((min_float != -std::numeric_limits<double>::max() && tmp < min_float) || (max_float != std::numeric_limits<double>::max() && tmp > max_float))
      {
        message = "Invalid double parameter value '" + std::to_string(tmp) + "' for parameter '" + name + "' given! The valid range is: [" + std::to_string(min_float)+ ":" + std::to_string(max_float) + "].";
        return false;
      }
    }
    else if (value.valueType() == ParamValue::DOUBLE_LIST)
    {
      std::vector<double> ls_value = value;
      for (size_t i = 0; i < ls_value.size(); ++i)
      {
        double dou_value = ls_value[i];
        if ((min_float != -std::numeric_limits<double>::max() && dou_value < min_float) || (max_float != std::numeric_limits<double>::max() && dou_value > max_float))
        {
          message = "Invalid double parameter value '" + std::to_string(dou_value) + "' for parameter '" + name + "' given! The valid range is: [" + std::to_string(min_float) + ":" + std::to_string(max_float) + "].";
          return false;
        }
      }
    }
    return true;
  }

  bool Param::ParamEntry::operator==(const ParamEntry& rhs) const
  {
    return name == rhs.name && value == rhs.value;
  }

  //********************************* ParamNode **************************************
  Param::ParamNode::ParamNode(const std::string& n, const std::string& d) :
    name(n),
    description(d),
    entries(),
    nodes()
  {
      if (name.find(':') != std::string::npos)
      {
        std::cerr << "Error ParamNode name must not contain ':' characters!" << std::endl;
      }
  }

  bool Param::ParamNode::operator==(const ParamNode& rhs) const
  {
    if (name != rhs.name || entries.size() != rhs.entries.size() || nodes.size() != rhs.nodes.size())
    {
      return false;
    }
    //order of sections / entries should not matter
    for (size_t i = 0; i < entries.size(); ++i)
    {
      if (find(rhs.entries.begin(), rhs.entries.end(), entries[i]) == rhs.entries.end())
      {
        return false;
      }
    }
    for (size_t i = 0; i < nodes.size(); ++i)
    {
      if (find(rhs.nodes.begin(), rhs.nodes.end(), nodes[i]) == rhs.nodes.end())
      {
        return false;
      }
    }

    return true;
  }

  Param::ParamNode::EntryIterator Param::ParamNode::findEntry(const std::string& local_name)
  {
    for (EntryIterator it = entries.begin(); it != entries.end(); ++it)
    {
      if (it->name == local_name)
      {
        return it;
      }
    }
    return entries.end();
  }

  Param::ParamNode::NodeIterator Param::ParamNode::findNode(const std::string& local_name)
  {
    for (NodeIterator it = nodes.begin(); it != nodes.end(); ++it)
    {
      if (it->name == local_name)
      {
        return it;
      }
    }
    return nodes.end();
  }

  Param::ParamNode* Param::ParamNode::findParentOf(const std::string& local_name)
  {
    //cout << "findParentOf nodename: " << this->name << " - nodes: " << this->nodes.size() << " - find: "<< name << std::endl;
    if (local_name.find(':') != std::string::npos) //several subnodes to browse through
    {
        size_t pos = local_name.find(':');
        std::string prefix = local_name.substr(0, pos);

        //cout << " - Prefix: '" << prefix << "'" << std::endl;
        NodeIterator it = findNode(prefix);
        if (it == nodes.end()) //subnode not found
        {
          return nullptr;
        }
        //recursively call findNode for the rest of the path
        std::string new_name = local_name.substr(it->name.size() + 1);
        //cout << " - Next name: '" << new_name << "'" << std::endl;
        return it->findParentOf(new_name);
    }
    else // we are in the right child
    {
        //check if a node or entry prefix match
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            if (nodes[i].name.compare(0, local_name.size(), local_name) == 0)
            {
              return this;
            }
        }
        for (size_t i = 0; i < entries.size(); ++i)
        {
            if (entries[i].name.compare(0, local_name.size(), local_name) == 0)
            {
            return this;
            }
        }
        return nullptr;
    }
  }

  Param::ParamEntry* Param::ParamNode::findEntryRecursive(const std::string& local_name)
  {
    ParamNode* parent = findParentOf(local_name);
    if (parent == nullptr)
    {
      return nullptr;
    }

    EntryIterator it = parent->findEntry(suffix(local_name));
    if (it == parent->entries.end())
    {
      return nullptr;
    }

    return &(*it);
  }

  void Param::ParamNode::insert(const ParamNode& node, const std::string& prefix)
  {
    //std::cerr << "INSERT NODE  " << node.name << " (" << prefix << ")" << std::endl;
    std::string prefix2 = prefix + node.name;

    ParamNode* insert_node = this;
    while (prefix2.find(':') != std::string::npos)
    {
      size_t pos = prefix2.find(':');
      std::string local_name = prefix2.substr(0, pos);
      //check if the node already exists
      NodeIterator it = insert_node->findNode(local_name);
      if (it != insert_node->nodes.end()) //exists
      {
        insert_node = &(*it);
      }
      else //create it
      {
        insert_node->nodes.push_back(ParamNode(local_name, ""));
        insert_node = &(insert_node->nodes.back());
        //std::cerr << " - Created new node: " << insert_node->name << std::endl;
      }
      //remove prefix
      prefix2 = prefix2.substr(local_name.size() + 1);
    }

    //check if the node already exists
    NodeIterator it = insert_node->findNode(prefix2);
    if (it != insert_node->nodes.end()) //append nodes and entries
    {
      for (ConstNodeIterator it2 = node.nodes.begin(); it2 != node.nodes.end(); ++it2)
      {
        it->insert(*it2);
      }
      for (ConstEntryIterator it2 = node.entries.begin(); it2 != node.entries.end(); ++it2)
      {
        it->insert(*it2);
      }
      if (it->description == "" || node.description != "") //replace description if not empty in new node
      {
        it->description = node.description;
      }
    }
    else //insert it
    {
      Param::ParamNode tmp(node);
      tmp.name = prefix2;
      insert_node->nodes.push_back(tmp);
    }
  }

  void Param::ParamNode::insert(const ParamEntry& entry, const std::string& prefix)
  {
    //std::cerr << "INSERT ENTRY " << entry.name << " (" << prefix << ")" << std::endl;
    std::string prefix2 = prefix + entry.name;
    //std::cerr << " - inserting: " << prefix2 << std::endl;

    ParamNode* insert_node = this;
    while (prefix2.find(':') != std::string::npos)
    {
      size_t pos = prefix2.find(':');
      std::string local_name = prefix2.substr(0, pos);
      //std::cerr << " - looking for node: " << name << std::endl;
      //look up if the node already exists
      NodeIterator it = insert_node->findNode(local_name);
      if (it != insert_node->nodes.end()) //exists
      {
        insert_node = &(*it);
      }
      else //create it
      {
        insert_node->nodes.push_back(ParamNode(local_name, ""));
        insert_node = &(insert_node->nodes.back());
        //std::cerr << " - Created new node: " << insert_node->name << std::endl;
      }
      //remove prefix
      prefix2 = prefix2.substr(local_name.size() + 1);
      //std::cerr << " - new prefix: " << prefix2 << std::endl;
    }

    //check if the entry already exists
    //std::cerr << " - final entry name: " << prefix2 << std::endl;
    EntryIterator it = insert_node->findEntry(prefix2);
    if (it != insert_node->entries.end()) //overwrite entry
    {
      it->value = entry.value;
      it->tags = entry.tags;
      if (it->description == "" || entry.description != "") //replace description if not empty in new entry
      {
        it->description = entry.description;
      }
    }
    else //insert it
    {
      Param::ParamEntry tmp(entry);
      tmp.name = prefix2;
      insert_node->entries.push_back(tmp);
    }
  }

  size_t Param::ParamNode::size() const
  {
    size_t subnode_size = 0;
    for (std::vector<ParamNode>::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
      subnode_size += it->size();
    }
    return entries.size() + subnode_size;
  }

  std::string Param::ParamNode::suffix(const std::string& key) const
  {
    size_t pos = key.rfind(':');
    if (pos != std::string::npos)
    {
      return key.substr(++pos);
    }
    return key;
  }

  //********************************* Param **************************************

  Param::Param(const ParamNode& node) :
    root_(node)
  {
    root_.name = "ROOT";
    root_.description = "";
  }

  bool Param::operator==(const Param& rhs) const
  {
    return root_ == rhs.root_;
  }

  void Param::setValue(const std::string& key, const ParamValue& value, const std::string& description, const std::vector<std::string>& tags)
  {
    root_.insert(ParamEntry("", value, description, tags), key);
  }

  void Param::setValidStrings(const std::string& key, const std::vector<std::string>& strings)
  {
    ParamEntry& entry = getEntry_(key);
    //check if correct parameter type
    if (entry.value.valueType() != ParamValue::STRING_VALUE && entry.value.valueType() != ParamValue::STRING_LIST)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ElementNotFound(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, key);
#else
      throw std::invalid_argument{key};
#endif
    }
    //check for commas
    for (size_t i = 0; i < strings.size(); ++i)
    {
      if (strings[i].find(',') != std::string::npos)
      {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
        throw Exception::InvalidParameter(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Comma characters in Param string restrictions are not allowed!");
#else
        throw std::invalid_argument{"Comma characters in Param string restrictions are not allowed!"};
#endif
      }
    }
    entry.valid_strings = strings;
  }

  void Param::setMinInt(const std::string& key, int min)
  {
    ParamEntry& entry = getEntry_(key);
    if (entry.value.valueType() != ParamValue::INT_VALUE && entry.value.valueType() != ParamValue::INT_LIST)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ElementNotFound(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, key);
#else
      throw std::invalid_argument{key};
#endif
    }
    entry.min_int = min;
  }

  void Param::setMaxInt(const std::string& key, int max)
  {
    ParamEntry& entry = getEntry_(key);
    if (entry.value.valueType() != ParamValue::INT_VALUE && entry.value.valueType() != ParamValue::INT_LIST)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ElementNotFound(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, key);
#else
      throw std::invalid_argument{key};
#endif
    }
    entry.max_int = max;
  }

  void Param::setMinFloat(const std::string& key, double min)
  {
    ParamEntry& entry = getEntry_(key);
    if (entry.value.valueType() != ParamValue::DOUBLE_VALUE && entry.value.valueType() != ParamValue::DOUBLE_LIST)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ElementNotFound(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, key);
#else
      throw std::invalid_argument{key};
#endif
    }
    entry.min_float = min;
  }

  void Param::setMaxFloat(const std::string& key, double max)
  {
    ParamEntry& entry = getEntry_(key);
    if (entry.value.valueType() != ParamValue::DOUBLE_VALUE && entry.value.valueType() != ParamValue::DOUBLE_LIST)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ElementNotFound(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, key);
#else
      throw std::invalid_argument{key};
#endif
    }
    entry.max_float = max;
  }

  const ParamValue& Param::getValue(const std::string& key) const
  {
    return getEntry_(key).value;
  }

  const std::string& Param::getSectionDescription(const std::string& key) const
  {
    //This variable is used instead of String::EMPTY as the method is used in
    //static initialization and thus cannot rely on String::EMPTY been initialized.
    static std::string empty;

    ParamNode* node = root_.findParentOf(key);
    if (node == nullptr)
    {
      return empty;
    }

    Param::ParamNode::NodeIterator it = node->findNode(node->suffix(key));
    if (it == node->nodes.end())
    {
      return empty;
    }

    return it->description;
  }

  void Param::insert(const std::string& prefix, const Param& param)
  {
    //std::cerr << "INSERT PARAM (" << prefix << ")" << std::endl;
    for (Param::ParamNode::NodeIterator it = param.root_.nodes.begin(); it != param.root_.nodes.end(); ++it)
    {
      root_.insert(*it, prefix);
    }
    for (Param::ParamNode::EntryIterator it = param.root_.entries.begin(); it != param.root_.entries.end(); ++it)
    {
      root_.insert(*it, prefix);
    }
  }

  void Param::setDefaults(const Param& defaults, const std::string& prefix, bool showMessage)
  {
    std::string prefix2 = prefix;
    if (prefix2 != "")
    {
      if (prefix2.back() != ':')
      {
        prefix2 += ':';
      }
    }

    std::string pathname;
    for (Param::ParamIterator it = defaults.begin(); it != defaults.end(); ++it)
    {
      if (!exists(prefix2 + it.getName()))
      {
        if (showMessage)
          std::cerr << "Setting " << prefix2 + it.getName() << " to " << it->value << std::endl;
        std::string name = prefix2 + it.getName();
        root_.insert(ParamEntry("", it->value, it->description), name);
        //copy tags
        for (std::set<std::string>::const_iterator tag_it = it->tags.begin(); tag_it != it->tags.end(); ++tag_it)
        {
          addTag(name, *tag_it);
        }
        //copy restrictions
        if (it->value.valueType() == ParamValue::STRING_VALUE || it->value.valueType() == ParamValue::STRING_LIST)
        {
          setValidStrings(name, it->valid_strings);
        }
        else if (it->value.valueType() == ParamValue::INT_VALUE || it->value.valueType() == ParamValue::INT_LIST)
        {
          setMinInt(name, it->min_int);
          setMaxInt(name, it->max_int);
        }
        else if (it->value.valueType() == ParamValue::DOUBLE_VALUE || it->value.valueType() == ParamValue::DOUBLE_LIST)
        {
          setMinFloat(name, it->min_float);
          setMaxFloat(name, it->max_float);
        }
      }

      //copy section descriptions
      const std::vector<ParamIterator::TraceInfo>& trace = it.getTrace();
      for (std::vector<ParamIterator::TraceInfo>::const_iterator it2 = trace.begin(); it2 != trace.end(); ++it2)
      {
        if (it2->opened)
        {
          pathname += it2->name + ":";
        }
        else
        {
          pathname.resize(pathname.size() - it2->name.size() - 1);
        }
        std::string real_pathname = pathname.substr(0, pathname.length() - 1); //remove ':' at the end
        if (real_pathname != "")
        {
          std::string description_old = getSectionDescription(prefix + real_pathname);
          std::string description_new = defaults.getSectionDescription(real_pathname);
          if (description_old == "")
          {
            //std::cerr << "## Setting description of " << prefix+real_pathname << " to"<< std::endl;
            //std::cerr << "## " << description_new << std::endl;
            setSectionDescription(prefix2 + real_pathname, description_new);
          }
        }
      }
    }
  }

  void Param::remove(const std::string& key)
  {
    std::string keyname = key;
    if (!key.empty() && key.back() == ':') // delete section
    {
      keyname = key.substr(0, key.length() - 1);

      ParamNode* node_parent = root_.findParentOf(keyname);
      if (node_parent != nullptr)
      {
        Param::ParamNode::NodeIterator it = node_parent->findNode(node_parent->suffix(keyname));
        if (it != node_parent->nodes.end())
        {
          std::string name = it->name;
          node_parent->nodes.erase(it); // will automatically delete subnodes
          if (node_parent->nodes.empty()  && node_parent->entries.empty())
          {
            // delete last section name (could be partial)
            remove(keyname.substr(0, keyname.size() - name.size())); // keep last ':' to indicate deletion of a section
          }
        }
      }
    }
    else
    {
      ParamNode* node = root_.findParentOf(keyname);
      if (node != nullptr)
      {
        std::string entryname = node->suffix(keyname); // get everything beyond last ':'
        Param::ParamNode::EntryIterator it = node->findEntry(entryname);
        if (it != node->entries.end())
        {
          node->entries.erase(it); // delete entry
          if (node->nodes.empty()  && node->entries.empty())
          {
            // delete if section is now empty
            remove(keyname.substr(0, keyname.length() - entryname.length())); // keep last ':' to indicate deletion of a section
          }
        }
      }
    }
  }

  void Param::removeAll(const std::string& prefix)
  {
    if (!prefix.empty() && prefix.back() == ':')//we have to delete one node only (and its subnodes)
    {
      ParamNode* node = root_.findParentOf(prefix.substr(0, prefix.length() - 1));
      if (node != nullptr)
      {
        Param::ParamNode::NodeIterator it = node->findNode(node->suffix(prefix.substr(0, prefix.length() - 1)));
        if (it != node->nodes.end())
        {
          std::string name = it->name;
          node->nodes.erase(it); // will automatically delete subnodes
          if (node->nodes.empty()  && node->entries.empty())
          {
            // delete last section name (could be partial)
            removeAll(prefix.substr(0, prefix.length() - name.length() - 1));// '-1' for the tailing ':'
          }
        }
      }
    }
    else //we have to delete all entries and nodes starting with the prefix
    {
      ParamNode* node = root_.findParentOf(prefix);
      if (node != nullptr)
      {
        std::string suffix = node->suffix(prefix); // name behind last ":"

        for (Param::ParamNode::NodeIterator it = node->nodes.begin(); it != node->nodes.end(); /*do nothing*/)
        {
          if (it->name.compare(0, suffix.length(), suffix) == 0)
          {
            it = node->nodes.erase(it);
          }
          else if (it != node->nodes.end())
          {
            ++it;
          }
        }
        for (Param::ParamNode::EntryIterator it = node->entries.begin(); it != node->entries.end(); /*do nothing*/)
        {
          if (it->name.compare(0, suffix.size(), suffix) == 0)
          {
            it = node->entries.erase(it);
          }
          else if (it != node->entries.end())
          {
            ++it;
          }
        }
        // the parent node might now be empty (delete it as well - otherwise the trace might be broken)
        if (node->nodes.empty() && node->entries.empty())
        {
          // delete last section name (could be partial)
          removeAll(prefix.substr(0, prefix.size() - suffix.size()));
        }
      }
    }
  }

  Param Param::copySubset(const Param& subset) const
  {
    ParamNode out("ROOT", "");

    for (const auto& entry : subset.root_.entries)
    {
      const auto& n = root_.findEntry(entry.name);
      if (n == root_.entries.end())
      {
        TDL_LOGSTREAM_WARN << "Warning: Trying to copy non-existent parameter entry " << entry.name << std::endl;
      }
      else
      {
        out.insert(*n);
      }
    }

    for (const auto& node : subset.root_.nodes)
    {
      const auto& n = root_.findNode(node.name);
      if (n == root_.nodes.end())
      {
        TDL_LOGSTREAM_WARN << "Warning: Trying to copy non-existent parameter node " << node.name << std::endl;
      }
      else
      {
        out.insert(*n);
      }
    }
    return Param(out);
  }

  Param Param::copy(const std::string& prefix, bool remove_prefix) const
  {
    ParamNode out("ROOT", "");

    ParamNode* node = root_.findParentOf(prefix);
    if (node == nullptr)
    {
      return Param();
    }
    //we have to copy this node only
    if (!prefix.empty() && prefix.back() == ':')
    {
      if (remove_prefix)
      {
        out = *node;
      }
      else
      {
        out.insert(*node, prefix.substr(0, prefix.size() - node->name.size() - 1));
      }
    }
    else //we have to copy all entries and nodes starting with the right suffix
    {
      std::string suffix = node->suffix(prefix);
      for (Param::ParamNode::NodeIterator it = node->nodes.begin(); it != node->nodes.end(); ++it)
      {
        if (it->name.compare(0, suffix.size(), suffix) == 0)
        {
          if (remove_prefix)
          {
            ParamNode tmp = *it;
            tmp.name = tmp.name.substr(suffix.size());
            out.insert(tmp);
          }
          else
          {
            out.insert(*it, prefix.substr(0, prefix.size() - suffix.size()));
          }
        }
      }
      for (Param::ParamNode::EntryIterator it = node->entries.begin(); it != node->entries.end(); ++it)
      {
        if (it->name.compare(0, suffix.size(), suffix) == 0)
        {
          if (remove_prefix)
          {
            ParamEntry tmp = *it;
            tmp.name = tmp.name.substr(suffix.size());
            out.insert(tmp);
          }
          else
          {
            out.insert(*it, prefix.substr(0, prefix.size() - suffix.size()));
          }
        }
      }
    }

    return Param(out);
  }

  void Param::parseCommandLine(const int argc, const char** argv, const std::string& prefix)
  {
    //determine prefix
    std::string prefix2 = prefix;
    if (prefix2 != "")
    {
      //prefix2.ensureLastChar(':');
      if (prefix2.back() != ':')
      {
        prefix2.append(1, ':');
      }
    }

    //parse arguments
    std::string arg, arg1;
    for (int i = 1; i < argc; ++i)
    {
      //load the current and next argument:  arg and arg1 ("" at the last argument)
      arg = argv[i];
      arg1 = "";
      if (i + 1 < argc)
      {
        arg1 = argv[i + 1];
      }

      //it is a option when it starts with a '-' and the second character is not a number
      bool arg_is_option = false;
      if (arg.size() >= 2 && arg[0] == '-' && arg[1] != '0' && arg[1] != '1' && arg[1] != '2' && arg[1] != '3' && arg[1] != '4' && arg[1] != '5' && arg[1] != '6' && arg[1] != '7' && arg[1] != '8' && arg[1] != '9')
      {
        arg_is_option = true;
      }
      bool arg1_is_option = false;
      if (arg1.size() >= 2 && arg1[0] == '-' && arg1[1] != '0' && arg1[1] != '1' && arg1[1] != '2' && arg1[1] != '3' && arg1[1] != '4' && arg1[1] != '5' && arg1[1] != '6' && arg1[1] != '7' && arg1[1] != '8' && arg1[1] != '9')
      {
        arg1_is_option = true;
      }
      //cout << "Parse: '"<< arg << "' '" << arg1 << "'" << std::endl;

      //flag (option without text argument)
      if (arg_is_option && arg1_is_option)
      {
        root_.insert(ParamEntry(arg, std::string(), ""), prefix2);
      }
      //option with argument
      else if (arg_is_option && !arg1_is_option)
      {
        root_.insert(ParamEntry(arg, arg1, ""), prefix2);
        ++i;
      }
      //just text arguments (not preceded by an option)
      else
      {

        ParamEntry* misc_entry = root_.findEntryRecursive(prefix2 + "misc");
        if (misc_entry == nullptr)
        {
          std::vector<std::string> sl;
          sl.push_back(arg);
          // create "misc"-Node:
          root_.insert(ParamEntry("misc", sl, ""), prefix2);
        }
        else
        {
          std::vector<std::string> sl = misc_entry->value;
          sl.push_back(arg);
          misc_entry->value = sl;
        }
      }
    }
  }

  void Param::parseCommandLine(const int argc, const char** argv, const std::map<std::string, std::string>& options_with_one_argument, const std::map<std::string, std::string>& options_without_argument, const std::map<std::string, std::string>& options_with_multiple_argument, const std::string& misc, const std::string& unknown)
  {
    //determine misc key
    std::string misc_key = misc;

    //determine unknown key
    std::string unknown_key = unknown;

    //parse arguments
    std::string arg, arg1;
    for (int i = 1; i < argc; ++i)
    {
      //load the current and next argument:  arg and arg1 ("" at the last argument)
      arg = argv[i];
      arg1 = "";
      if (i + 1 < argc)
      {
        arg1 = argv[i + 1];
      }

      //it is a option when it starts with a '-' and the second character is not a number
      bool arg_is_option = false;
      if (arg.size() >= 2 && arg[0] == '-' && arg[1] != '0' && arg[1] != '1' && arg[1] != '2' && arg[1] != '3' && arg[1] != '4' && arg[1] != '5' && arg[1] != '6' && arg[1] != '7' && arg[1] != '8' && arg[1] != '9')
      {
        arg_is_option = true;
      }
      bool arg1_is_option = false;
      if (arg1.size() >= 2 && arg1[0] == '-' && arg1[1] != '0' && arg1[1] != '1' && arg1[1] != '2' && arg1[1] != '3' && arg1[1] != '4' && arg1[1] != '5' && arg1[1] != '6' && arg1[1] != '7' && arg1[1] != '8' && arg1[1] != '9')
      {
        arg1_is_option = true;
      }

      //with multiple argument
      if (options_with_multiple_argument.find(arg) != options_with_multiple_argument.end())
      {
        //next argument is an option
        if (arg1_is_option)
        {
          root_.insert(ParamEntry("", std::vector<std::string>(), ""), options_with_multiple_argument.find(arg)->second);
        }
        //next argument is not an option
        else
        {
          std::vector<std::string> sl;
          int j = (i + 1);
          while (j < argc && !(arg1.size() >= 2 && arg1[0] == '-' && arg1[1] != '0' && arg1[1] != '1' && arg1[1] != '2' && arg1[1] != '3' && arg1[1] != '4' && arg1[1] != '5' && arg1[1] != '6' && arg1[1] != '7' && arg1[1] != '8' && arg1[1] != '9'))
          {
            sl.push_back(arg1);
            ++j;
            if (j < argc)
            {
              arg1 = argv[j];
            }
          }

          root_.insert(ParamEntry("", sl, ""), options_with_multiple_argument.find(arg)->second);
          i = j - 1;
        }
      }
      //without argument
      else if (options_without_argument.find(arg) != options_without_argument.end())
      {
        root_.insert(ParamEntry("", "true", ""), options_without_argument.find(arg)->second);
      }
      //with one argument
      else if (options_with_one_argument.find(arg) != options_with_one_argument.end())
      {
        //next argument is not an option
        if (!arg1_is_option)
        {
          root_.insert(ParamEntry("", arg1, ""), options_with_one_argument.find(arg)->second);
          ++i;
        }
        //next argument is an option
        else
        {

          root_.insert(ParamEntry("", std::string(), ""), options_with_one_argument.find(arg)->second);
        }
      }
      //unknown option
      else if (arg_is_option)
      {
        ParamEntry* unknown_entry = root_.findEntryRecursive(unknown);
        if (unknown_entry == nullptr)
        {
          std::vector<std::string> sl;
          sl.push_back(arg);
          root_.insert(ParamEntry("", sl, ""), unknown);
        }
        else
        {
          std::vector<std::string> sl = unknown_entry->value;
          sl.push_back(arg);
          unknown_entry->value = sl;
        }
      }
      //just text argument
      else
      {
        ParamEntry* misc_entry = root_.findEntryRecursive(misc);
        if (misc_entry == nullptr)
        {
          std::vector<std::string> sl;
          sl.push_back(arg);
          // create "misc"-Node:
          root_.insert(ParamEntry("", sl, ""), misc);
        }
        else
        {
          std::vector<std::string> sl = misc_entry->value;
          sl.push_back(arg);
          misc_entry->value = sl;
        }
      }
    }
  }

  std::ostream& operator<<(std::ostream& os, const Param& param)
  {
    for (Param::ParamIterator it = param.begin(); it != param.end(); ++it)
    {
      os << '"';
      if (it.getName().length() > it->name.length() + 1)
      {
        os << it.getName().substr(0, it.getName().length() - it->name.length() - 1) << "|";
      }
      os  << it->name << "\" -> \"" << it->value << '"';
      if (it->description != "")
      {
        os << " (" << it->description << ")";
      }
      os << std::endl;
    }
    return os;
  }

  size_t Param::size() const
  {
    return root_.size();
  }

  bool Param::empty() const
  {
    return size() == 0;
  }

  void Param::clear()
  {
    root_ = ParamNode("ROOT", "");
  }

  void Param::checkDefaults(const std::string& name, const Param& defaults, const std::string& prefix) const
  {
    //Extract right parameters
    std::string prefix2 = prefix;
    if (prefix2 != "")
    {
      if (prefix2.back() != ':')
      {
          prefix2 += ':';
      }
    }
    Param check_values = copy(prefix2, true);

    //check
    for (ParamIterator it = check_values.begin(); it != check_values.end(); ++it)
    {
      //unknown parameter
      if (!defaults.exists(it.getName()))
      {
        TDL_LOGSTREAM_WARN << "Warning: " << name << " received the unknown parameter '" << it.getName() << "'";
        if (!prefix2.empty())
        {
          TDL_LOGSTREAM_WARN << " in '" << prefix2 << "'";
        }
        TDL_LOGSTREAM_WARN << "!" << std::endl;
      }

      //different types
      ParamEntry* default_value = defaults.root_.findEntryRecursive(prefix2 + it.getName());
      if (default_value == nullptr)
      {
        continue;
      }
      if (default_value->value.valueType() != it->value.valueType())
      {
        std::string d_type;
        if (default_value->value.valueType() == ParamValue::STRING_VALUE)
        {
          d_type = "string";
        }
        if (default_value->value.valueType() == ParamValue::STRING_LIST)
        {
          d_type = "string list";
        }
        if (default_value->value.valueType() == ParamValue::EMPTY_VALUE)
        {
          d_type = "empty";
        }
        if (default_value->value.valueType() == ParamValue::INT_VALUE)
        {
          d_type = "integer";
        }
        if (default_value->value.valueType() == ParamValue::INT_LIST)
        {
          d_type = "integer list";
        }
        if (default_value->value.valueType() == ParamValue::DOUBLE_VALUE)
        {
          d_type = "float";
        }
        if (default_value->value.valueType() == ParamValue::DOUBLE_LIST)
        {
          d_type = "float list";
        }
        std::string p_type;
        if (it->value.valueType() == ParamValue::STRING_VALUE)
        {
          p_type = "string";
        }
        if (it->value.valueType() == ParamValue::STRING_LIST)
        {
          p_type = "string list";
        }
        if (it->value.valueType() == ParamValue::EMPTY_VALUE)
        {
          p_type = "empty";
        }
        if (it->value.valueType() == ParamValue::INT_VALUE)
        {
          p_type = "integer";
        }
        if (it->value.valueType() == ParamValue::INT_LIST)
        {
          p_type = "integer list";
        }
        if (it->value.valueType() == ParamValue::DOUBLE_VALUE)
        {
          p_type = "float";
        }
        if (it->value.valueType() == ParamValue::DOUBLE_LIST)
        {
          p_type = "float list";
        }

#ifdef OPENMS_EXCEPTIONS_AVAILABLE
        throw Exception::InvalidParameter(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, name + ": Wrong parameter type '" + p_type + "' for " + d_type + " parameter '" + it.getName() + "' given!");
#else
        throw std::invalid_argument{name + ": Wrong parameter type '" + p_type + "' for " + d_type + " parameter '" + it.getName() + "' given!"};
#endif
      }
      //parameter restrictions
      ParamEntry pe = *default_value;
      pe.value = it->value;
      std::string s;
      if (!pe.isValid(s))
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
        throw Exception::InvalidParameter(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, name + ": " + s);
#else
        throw std::invalid_argument{name + ": " + s};
#endif
    }
  }

  Param::ParamIterator Param::findFirst(const std::string& leaf) const
  {
    for (Param::ParamIterator it = this->begin(); it != this->end(); ++it)
    {
      std::string suffix = ":" + leaf;
      if (!(suffix.length() > it.getName().length()) &&
          it.getName().compare(it.getName().length() - suffix.length(), suffix.length(), suffix) == 0)
      {
        return it;
      }
    }
    return this->end();
  }

  Param::ParamIterator Param::findNext(const std::string& leaf, const ParamIterator& start_leaf) const
  {
    // start at NEXT entry
    Param::ParamIterator it = start_leaf;
    if (it != this->end()) ++it;

    for (; it != this->end(); ++it)
    {
      std::string suffix = ":" + leaf;
      if (!(suffix.length() > it.getName().length()) &&
          it.getName().compare(it.getName().length() - suffix.length(), suffix.length(), suffix) == 0)
      {
        return it;
      }
    }
    return this->end();
  }

  bool Param::update(const Param& p_outdated, const bool add_unknown)
  {
    return update(p_outdated, add_unknown, TDL_LOGSTREAM_WARN);
  }

  bool Param::update(const Param& p_outdated, const bool add_unknown, TDL_LOGSTREAM_TYPE& stream)
  {
    bool fail_on_invalid_values = false;
    bool fail_on_unknown_parameters = false;
    return update(p_outdated, true, add_unknown, fail_on_invalid_values, fail_on_unknown_parameters, stream);
  }


  bool Param::update(const Param& p_outdated, bool verbose, const bool add_unknown, bool fail_on_invalid_values, bool fail_on_unknown_parameters, TDL_LOGSTREAM_TYPE& stream)
  {
    bool is_update_success(true);
    // augment
    for (Param::ParamIterator it = p_outdated.begin(); it != p_outdated.end(); ++it)
    {
      Param::ParamEntry new_entry; // entry of new location (used to retain new description)
      std::string target_name; // fully qualified name in new param

      if (this->exists(it.getName()))
      {
        // param 'version': do not override!
        std::string suffix = ":version";
        if (!(suffix.length() > it.getName().length()) &&
            it.getName().compare(it.getName().length() - suffix.length(), suffix.length(), suffix) == 0)
        {
          if (this->getValue(it.getName()) != it->value)
          {
#ifdef OPENMS_THREAD_CRITICAL_AVAILABLE
OPENMS_THREAD_CRITICAL(oms_log)
#endif
            stream << "Warning: for ':version' entry, augmented and Default Ini-File differ in value. Default value will not be altered!\n";
          }
          continue;
        }
        // param 'type': do not override!
        else if (suffix = ":type",
                !(suffix.length() > it.getName().length()) &&
                it.getName().compare(it.getName().length() - suffix.length(), suffix.length(), suffix) == 0) // only for TOPP type (e.g. PeakPicker:1:type), any other 'type' param is ok
        {
          size_t first = it.getName().find(':');
          if (first != std::string::npos &&
              it.getName().find(':', first+1) != std::string::npos)
          {
            if (this->getValue(it.getName()) != it->value)
            {
                #ifdef OPENMS_THREAD_CRITICAL_AVAILABLE
                OPENMS_THREAD_CRITICAL(oms_log)
                #endif
                stream << "Warning: for ':type' entry, augmented and Default Ini-File differ in value. Default value will not be altered!\n";
            }
            continue;
          }
        }

        // all other parameters:
        new_entry = this->getEntry(it.getName());
        target_name = it.getName();

      }
      else // outdated param non-existent in new param
      {
        // search by suffix in new param. Only match complete names, e.g. myname will match newsection:myname, but not newsection:othermyname
        Param::ParamEntry l1_entry = p_outdated.getEntry(it.getName());
        // since the outdated param with full path does not exist within new param,
        // we will never find the new entry by using exists() as above, thus
        // its safe to modify it here

        ParamIterator it_match = this->findFirst(l1_entry.name);
        if (it_match != this->end())
        {
          // make sure the same leaf name does not exist at any other position
          if (this->findNext(l1_entry.name, it_match) == this->end())
          {
#ifdef OPENMS_THREAD_CRITICAL_AVAILABLE
OPENMS_THREAD_CRITICAL(oms_log)
#endif
            stream << "Found '" << it.getName() << "' as '" << it_match.getName() << "' in new param." << std::endl;
            new_entry = this->getEntry(it_match.getName());
            target_name = it_match.getName();
          }
        }

        if (target_name.empty()) // no mapping was found
        {
          if (fail_on_unknown_parameters)
          {
#ifdef OPENMS_THREAD_CRITICAL_AVAILABLE
OPENMS_THREAD_CRITICAL(oms_log)
#endif
            stream << "Unknown (or deprecated) Parameter '" << it.getName() << "' given in outdated parameter file!" << std::endl;
            is_update_success = false;
          }
          else if (add_unknown)
          {
#ifdef OPENMS_THREAD_CRITICAL_AVAILABLE
OPENMS_THREAD_CRITICAL(oms_log)
#endif
            stream << "Unknown (or deprecated) Parameter '" << it.getName() << "' given in outdated parameter file! Adding to current set." << std::endl;
            Param::ParamEntry local_entry = p_outdated.getEntry(it.getName());
            std::string prefix = "";
            if (it.getName().find(':') != std::string::npos)
            {
              prefix = it.getName().substr(0, 1 + it.getName().find_last_of(':'));
            }
            this->root_.insert(local_entry, prefix); //->setValue(it.getName(), local_entry.value, local_entry.description, local_entry.tags);
          }
          else if (verbose)
          {
#ifdef OPENMS_THREAD_CRITICAL_AVAILABLE
OPENMS_THREAD_CRITICAL(oms_log)
#endif
            stream << "Unknown (or deprecated) Parameter '" << it.getName() << "' given in outdated parameter file! Ignoring parameter. " << std::endl;
          }
          continue;
        }
      }

      // do the actual updating (we found a matching pair)
      if (new_entry.value.valueType() == it->value.valueType())
      {
        if (new_entry.value != it->value)
        {
          // check entry for consistency (in case restrictions have changed)
          ParamValue default_value = new_entry.value;
          new_entry.value = it->value;
          std::string validation_result;
          if (new_entry.isValid(validation_result))
          {
            // overwrite default value
            if (verbose)
            {
#ifdef OPENMS_THREAD_CRITICAL_AVAILABLE
OPENMS_THREAD_CRITICAL(oms_log)
#endif
                stream << "Default-Parameter '" << target_name << "' overridden: '" << default_value << "' --> '" << it->value << "'!" << std::endl;
            }
            this->setValue(target_name, it->value, new_entry.description, this->getTags(target_name));
          }
          else
          {
#ifdef OPENMS_THREAD_CRITICAL_AVAILABLE
OPENMS_THREAD_CRITICAL(oms_log)
#endif
            stream << validation_result;
            if (fail_on_invalid_values)
            {
#ifdef OPENMS_THREAD_CRITICAL_AVAILABLE
OPENMS_THREAD_CRITICAL(oms_log)
#endif
              stream << " Updating failed!" << std::endl;
              is_update_success = false;
            }
            else
            {
#ifdef OPENMS_THREAD_CRITICAL_AVAILABLE
OPENMS_THREAD_CRITICAL(oms_log)
#endif
              stream << " Ignoring invalid value (using new default '" << default_value << "')!" << std::endl;
              new_entry.value = default_value;
            }
          }
        }
        else
        {
          // value stayed the same .. nothing to be done
        }
      }
      else
      {
#ifdef OPENMS_THREAD_CRITICAL_AVAILABLE
OPENMS_THREAD_CRITICAL(oms_log)
#endif
        stream << "Parameter '" << it.getName() << "' has changed value type!\n";
        if (fail_on_invalid_values)
        {
#ifdef OPENMS_THREAD_CRITICAL_AVAILABLE
OPENMS_THREAD_CRITICAL(oms_log)
#endif
          stream << " Updating failed!" << std::endl;
          is_update_success = false;
        }
        else
        {
#ifdef OPENMS_THREAD_CRITICAL_AVAILABLE
OPENMS_THREAD_CRITICAL(oms_log)
#endif
          stream << " Ignoring invalid value (using new default)!" << std::endl;
        }
      }

    } // next param in outdated tree

    return is_update_success;
  }

  void Param::merge(const tdl::Param& toMerge)
  {
    // keep track of the path inside the param tree
    std::string pathname;

    // augment
    for (Param::ParamIterator it = toMerge.begin(); it != toMerge.end(); ++it)
    {
      std::string prefix = "";
      if (it.getName().find(':') != std::string::npos)
        prefix = it.getName().substr(0, 1 + it.getName().find_last_of(':'));

      // we care only about values that do not exist already
      if (!this->exists(it.getName()))
      {
        Param::ParamEntry entry = *it;
        TDL_LOGSTREAM_DEBUG << "[Param::merge] merging " << it.getName() << std::endl;
        this->root_.insert(entry, prefix);
      }

      //copy section descriptions
      const std::vector<ParamIterator::TraceInfo>& trace = it.getTrace();
      for (std::vector<ParamIterator::TraceInfo>::const_iterator traceIt = trace.begin(); traceIt != trace.end(); ++traceIt)
      {
        if (traceIt->opened)
        {
          TDL_LOGSTREAM_DEBUG << "[Param::merge] extending param trace " << traceIt->name << " (" << pathname << ")" << std::endl;
          pathname += traceIt->name + ":";
        }
        else
        {
          TDL_LOGSTREAM_DEBUG << "[Param::merge] reducing param trace " << traceIt->name << " (" << pathname << ")" << std::endl;
          std::string suffix = traceIt->name + ":";
          if (suffix.size() <= pathname.size() && pathname.compare(pathname.size() - suffix.size(), suffix.size(), suffix) == 0)
            pathname.resize(pathname.size() - traceIt->name.size() - 1);
        }
        std::string real_pathname = pathname.substr(0, pathname.size() - 1);//remove ':' at the end
        if (real_pathname != "")
        {
          std::string description_old = getSectionDescription(prefix + real_pathname);
          std::string description_new = toMerge.getSectionDescription(real_pathname);
          if (description_old == "")
          {
            setSectionDescription(real_pathname, description_new);
          }
        }
      }

    }
  }

  void Param::setSectionDescription(const std::string& key, const std::string& description)
  {
    ParamNode* node = root_.findParentOf(key);
    if (node == nullptr)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ElementNotFound(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, key);
#else
      throw std::invalid_argument{key};
#endif
    }

    Param::ParamNode::NodeIterator it = node->findNode(node->suffix(key));
    if (it == node->nodes.end())
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ElementNotFound(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, key);
#else
      throw std::invalid_argument{key};
#endif
    }
    it->description = description;
  }

  void Param::addSection(const std::string& key, const std::string& description)
  {
    root_.insert(ParamNode("",description),key);
  }

  Param::ParamIterator Param::begin() const
  {
    return ParamIterator(root_);
  }

  Param::ParamIterator Param::end() const
  {
    return ParamIterator();
  }

  Param::ParamIterator::ParamIterator(const Param::ParamNode& root) :
    root_(&root),
    current_(-1),
    stack_(),
    trace_()
  {
    //Empty Param => begin == end iterator
    if (root_->entries.empty() && root_->nodes.empty())
    {
      root_ = nullptr;
      return;
    }

    //find first entry
    stack_.push_back(root_);
    operator++();
  }

  const Param::ParamEntry& Param::ParamIterator::operator*()
  {
    return stack_.back()->entries[current_];
  }

  const Param::ParamEntry* Param::ParamIterator::operator->()
  {
    return &(stack_.back()->entries[current_]);
  }

  Param::ParamIterator Param::ParamIterator::operator++(int)
  {
    ParamIterator tmp(*this);
    ++(*this);
    return tmp;
  }

  Param::ParamIterator& Param::ParamIterator::operator++()
  {
    if (root_ == nullptr)
    {
      return *this;
    }
    trace_.clear();
    while (true)
    {
      const Param::ParamNode* node = stack_.back();

      //cout << "############ operator++ #### " << node->name << " ## " << current_ <<endl;

      //check if there is a next entry in the current node
      if (current_ + 1 < (int)node->entries.size())
      {
        //cout << " - next entry" <<endl;
        ++current_;
        return *this;
      }
      //visit subnodes after entries
      else if (!node->nodes.empty())
      {
        current_ = -1;
        stack_.push_back(&(node->nodes[0]));
        //cout << " - entering into: " << node->nodes[0].name <<endl;
        //track changes (enter a node)
        trace_.push_back(TraceInfo(node->nodes[0].name, node->nodes[0].description, true));

        continue;
      }
      //go back in tree until the node we came from is not the last subnode
      //of the current node. Go into the next subnode.
      else
      {
        while (true)
        {
          const Param::ParamNode* last = node;
          stack_.pop_back();
          //cout << " - stack size: " << stack_.size() << endl;
          //we have reached the end
          if (stack_.empty())
          {
            //cout << " - reached the end" << endl;
            root_ = nullptr;
            return *this;
          }
          node = stack_.back();

          //cout << " - last was: " << last->name << endl;
          //cout << " - descended to: " << node->name << endl;

          //track changes (leave a node)
          trace_.push_back(TraceInfo(last->name, last->description, false));

          //check of new subtree is accessible
          unsigned int next_index = (last - &(node->nodes[0])) + 1;
          if (next_index < node->nodes.size())
          {
            current_ = -1;
            stack_.push_back(&(node->nodes[next_index]));
            //cout << " - entering into: " << node->nodes[next_index].name  << endl;
            //track changes (enter a node)
            trace_.push_back(TraceInfo(node->nodes[next_index].name, node->nodes[next_index].description, true));
            break;
          }
        }
      }
    }
  }

  bool Param::ParamIterator::operator==(const ParamIterator& rhs) const
  {
    return (root_ == nullptr && rhs.root_ == nullptr) || (stack_ == rhs.stack_ && current_ == rhs.current_);
  }

  bool Param::ParamIterator::operator!=(const ParamIterator& rhs) const
  {
    return !operator==(rhs);
  }

  std::string Param::ParamIterator::getName() const
  {
    std::string tmp;
    for (std::vector<const Param::ParamNode*>::const_iterator it = stack_.begin() + 1; it != stack_.end(); ++it)
    {
      tmp += (*it)->name + ':';
    }
    return tmp + stack_.back()->entries[current_].name;
  }

  const std::vector<Param::ParamIterator::TraceInfo>& Param::ParamIterator::getTrace() const
  {
    return trace_;
  }

  const Param::ParamEntry& Param::getEntry(const std::string& key) const
  {
    return getEntry_(key);
  }

  ParamValue::ValueType Param::getValueType(const std::string& key) const
  {
    return getEntry_(key).value.valueType();
  }

  const std::string& Param::getDescription(const std::string& key) const
  {
    return getEntry_(key).description;
  }

  void Param::addTag(const std::string& key, const std::string& tag)
  {
    if (tag.find(',') != std::string::npos)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::InvalidValue(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Param tags may not contain comma characters", tag);
#else
      throw std::invalid_argument{"Param tags may not contain comma characters" + tag};
#endif
    }
    getEntry_(key).tags.insert(tag);
  }

  void Param::addTags(const std::string& key, const std::vector<std::string>& tags)
  {
    ParamEntry& entry = getEntry_(key);
    for (size_t i = 0; i != tags.size(); ++i)
    {
      if (tags[i].find(',') != std::string::npos)
      {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
        throw Exception::InvalidValue(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Param tags may not contain comma characters", tags[i]);
#else
        throw std::invalid_argument{"Param tags may not contain comma characters" + tags[i]};
#endif
      }
      entry.tags.insert(tags[i]);
    }
  }

  std::vector<std::string> Param::getTags(const std::string& key) const
  {
    ParamEntry& entry = getEntry_(key);
    std::vector<std::string> list;
    for (std::set<std::string>::const_iterator it = entry.tags.begin(); it != entry.tags.end(); ++it)
    {
      list.push_back(*it);
    }
    return list;
  }

  void Param::clearTags(const std::string& key)
  {
    getEntry_(key).tags.clear();
  }

  bool Param::hasTag(const std::string& key, const std::string& tag) const
  {
    return getEntry_(key).tags.count(tag);
  }

  bool Param::exists(const std::string& key) const
  {
    return root_.findEntryRecursive(key);
  }

  bool Param::hasSection(const std::string &key) const
  {
    if (key.back() == ':')
    {
      // Remove trailing colon from key
      return root_.findParentOf(key.substr(0, key.size() - 1)) != nullptr;
    }
    else
    {
      return root_.findParentOf(key) != nullptr;
    }
  }

  Param::ParamEntry& Param::getEntry_(const std::string& key) const
  {
    ParamEntry* entry = root_.findEntryRecursive(key);
    if (entry == nullptr)
    {
#ifdef OPENMS_EXCEPTIONS_AVAILABLE
      throw Exception::ElementNotFound(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, key);
#else
      throw std::invalid_argument{key};
#endif
    }

    return *entry;
  }
} // namespace tdl

