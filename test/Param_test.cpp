#include <gtest/gtest.h>

#include <tdl/Param.h>


//////////////////// tdl::Param::ParamEntry /////////////////////////////
////////////////////////////////////////////////////////////////////

TEST(ParamEntry_construction_test, default_construction)
{
  tdl::Param::ParamEntry* pe_ptr = nullptr;

  pe_ptr = new tdl::Param::ParamEntry();

  EXPECT_NE(pe_ptr, nullptr);
}

TEST(ParamEntry_listOfStrings,  ctor_and_copy_operator) {
  tdl::Param::ParamEntry pe("n", "v", "d", {"advanced"});
  EXPECT_EQ(pe.name, "n");
  EXPECT_EQ(pe.description, "d");
  EXPECT_EQ(pe.value, "v");
  EXPECT_EQ(pe.tags.count("advanced")  ==  1, true);

  pe = tdl::Param::ParamEntry("n1", "v1", "d1");
  EXPECT_EQ(pe.name, "n1");
  EXPECT_EQ(pe.description, "d1");
  EXPECT_EQ(pe.value, "v1");
  EXPECT_EQ(pe.tags.count("advanced")  ==  1, false);
}

TEST(ParamEntry, isValid) {
  tdl::Param p;
  std::string m;
  p.setValue("int", 5);
  EXPECT_EQ(p.getEntry("int").isValid(m), true);
  p.setMinInt("int", 5);
  EXPECT_EQ(p.getEntry("int").isValid(m), true);
  p.setMaxInt("int", 8);
  EXPECT_EQ(p.getEntry("int").isValid(m), true);
  p.setValue("int", 10);
  EXPECT_EQ(p.getEntry("int").isValid(m), false);

  p.setValue("float", 5.1);
  EXPECT_EQ(p.getEntry("float").isValid(m), true);
  p.setMinFloat("float", 5.1);
  EXPECT_EQ(p.getEntry("float").isValid(m), true);
  p.setMaxFloat("float", 8.1);
  EXPECT_EQ(p.getEntry("float").isValid(m), true);
  p.setValue("float", 10.1);
  EXPECT_EQ(p.getEntry("float").isValid(m), false);

  p.setValue("float", 5.1);
  EXPECT_EQ(p.getEntry("float").isValid(m), true);
  p.setMinFloat("float", 5.1);
  EXPECT_EQ(p.getEntry("float").isValid(m), true);
  p.setMaxFloat("float", 8.1);
  EXPECT_EQ(p.getEntry("float").isValid(m), true);
  p.setValue("float", 10.1);
  EXPECT_EQ(p.getEntry("float").isValid(m), false);


  std::vector<std::string> strings;
  strings.push_back("bla");
  strings.push_back("bluff");
  p.setValue("string", "bli");
  EXPECT_EQ(p.getEntry("string").isValid(m), true);
  p.setValidStrings("string",strings);
  EXPECT_EQ(p.getEntry("string").isValid(m), false);

  p.setValue("string_2", "bla");
  EXPECT_EQ(p.getEntry("string_2").isValid(m), true);
  p.setValidStrings("string_2",strings);
  EXPECT_EQ(p.getEntry("string_2").isValid(m), true);
}

TEST(ParamEntry, compare_operator) {
  tdl::Param::ParamEntry n1("n",  "d", "v", {"advanced"});
  tdl::Param::ParamEntry n2("n",  "d", "v", {"advanced"});

  EXPECT_TRUE(n1  ==  n2);

  n2.name = "name";
  EXPECT_FALSE(n1  ==  n2);
  n2 = n1;

  n2.value = "bla";
  EXPECT_FALSE(n1  ==  n2);
  n2 = n1;

  n2.description = "bla";
  EXPECT_TRUE(n1  ==  n2);

  n2.tags.clear();
  EXPECT_TRUE(n1  ==  n2);
}

////////////////// tdl::Param::ParamNode ////////////////////////////////
////////////////////////////////////////////////////////////////////

tdl::Param::ParamNode* pn_ptr = nullptr;
TEST(ParamParamNode, defaultConstructor) {
  pn_ptr = new tdl::Param::ParamNode();
  EXPECT_NE(pn_ptr, nullptr);
  delete pn_ptr;
}

TEST(ParamParamNode, advancedConstructor) {
  tdl::Param::ParamNode n("n", "d");
  EXPECT_EQ(n.name, "n");
  EXPECT_EQ(n.description, "d");

  n = tdl::Param::ParamNode("n1", "d1");
  EXPECT_EQ(n.name, "n1");
  EXPECT_EQ(n.description, "d1");
}

TEST(ParamParamNode, compareOperator) {
  tdl::Param::ParamNode n1("n", "d");
  tdl::Param::ParamNode n2("n", "d");

  EXPECT_EQ(n1  ==  n2, true);

  n2.name = "name";
  EXPECT_EQ(n1  ==  n2, false);
  n2 = n1;

  n2.description = "bla";
  EXPECT_EQ(n1  ==  n2, true);
  n2 = n1;

  n2.nodes.resize(5);
  EXPECT_EQ(n1  ==  n2, false);
  n2 = n1;

  n2.entries.resize(5);
  EXPECT_EQ(n1  ==  n2, false);
  n2 = n1;

  n2.entries.push_back(tdl::Param::ParamEntry("a", "x", ""));
  n2.entries.push_back(tdl::Param::ParamEntry("b", "y", ""));
  n1.entries.push_back(tdl::Param::ParamEntry("b", "y", ""));
  n1.entries.push_back(tdl::Param::ParamEntry("a", "x", ""));
  EXPECT_EQ(n1  ==  n2, true);

  n2.nodes.push_back(tdl::Param::ParamNode("a", "x"));
  n2.nodes.push_back(tdl::Param::ParamNode("b", "y"));
  n1.nodes.push_back(tdl::Param::ParamNode("b", "y"));
  n1.nodes.push_back(tdl::Param::ParamNode("a", "x"));
  EXPECT_EQ(n1  ==  n2, true);
}

TEST(ParamParamNode, suffix) {
  tdl::Param::ParamNode node;
  EXPECT_EQ(node.suffix(""), "");
  EXPECT_EQ(node.suffix("A"), "A");
  EXPECT_EQ(node.suffix("A:A"), "A");
  EXPECT_EQ(node.suffix("A:AB"), "AB");
  EXPECT_EQ(node.suffix("AB:A"), "A");
  EXPECT_EQ(node.suffix(":A"), "A");
}


class ParamParamNodeF : public ::testing::Test
{
protected:
  //Dummy Tree:
  // A
  // |-B(1)
  // |-C
  // | |-D(2)
  // | |-E(3)
  // |-B
  //   |-G(4)
  tdl::Param::ParamNode pn, n;
  tdl::Param::ParamEntry e;

  void SetUp() override
  {
    pn.name="A";
    e.name="B"; e.value=1; pn.entries.push_back(e);
    n.name="C"; pn.nodes.push_back(n);
    e.name="D"; e.value=1; pn.nodes[0].entries.push_back(e);
    e.name="E"; e.value=1; pn.nodes[0].entries.push_back(e);
    n.name="B"; pn.nodes.push_back(n);
    e.name="G"; e.value=1; pn.nodes[1].entries.push_back(e);
  }
};


TEST_F(ParamParamNodeF, size) {
  EXPECT_EQ(pn.size(), 4);
  EXPECT_EQ(pn.nodes[0].size(), 2);
  EXPECT_EQ(pn.nodes[1].size(), 1);
}

TEST_F(ParamParamNodeF, findEntry) {
  EXPECT_EQ(pn.findEntry("A") == pn.entries.end(), true);
  EXPECT_EQ(pn.findEntry("B") != pn.entries.end(), true);
  EXPECT_EQ(pn.findEntry("C") == pn.entries.end(), true);
  EXPECT_EQ(pn.findEntry("D") == pn.entries.end(), true);
  EXPECT_EQ(pn.findEntry("E") == pn.entries.end(), true);
  EXPECT_EQ(pn.findEntry("F") == pn.entries.end(), true);
  EXPECT_EQ(pn.findEntry("G") == pn.entries.end(), true);
  EXPECT_EQ(pn.findEntry("H") == pn.entries.end(), true);
}

TEST_F(ParamParamNodeF, findNode) {
  EXPECT_EQ(pn.findNode("A") == pn.nodes.end(), true);
  EXPECT_EQ(pn.findNode("B") != pn.nodes.end(), true);
  EXPECT_EQ(pn.findNode("C") != pn.nodes.end(), true);
  EXPECT_EQ(pn.findNode("D") == pn.nodes.end(), true);
  EXPECT_EQ(pn.findNode("E") == pn.nodes.end(), true);
  EXPECT_EQ(pn.findNode("F") == pn.nodes.end(), true);
  EXPECT_EQ(pn.findNode("G") == pn.nodes.end(), true);
  EXPECT_EQ(pn.findNode("H") == pn.nodes.end(), true);
}

TEST_F(ParamParamNodeF, findParentOf) {
  EXPECT_EQ(pn.findParentOf("A"), nullptr);
  EXPECT_EQ(pn.findParentOf("B"), &pn);
  EXPECT_EQ(pn.findParentOf("C"), &pn);
  EXPECT_EQ(pn.findParentOf("C:D"), &(pn.nodes[0]));
  EXPECT_EQ(pn.findParentOf("C:E"), &(pn.nodes[0]));
  EXPECT_EQ(pn.findParentOf("F"), nullptr);
  EXPECT_EQ(pn.findParentOf("B:G"), &(pn.nodes[1]));
  EXPECT_EQ(pn.findParentOf("X"), nullptr);
  EXPECT_EQ(pn.findParentOf("H:X"), nullptr);
  EXPECT_EQ(pn.findParentOf("H:C:X"), nullptr);
  EXPECT_EQ(pn.findParentOf("H:C:"), nullptr);
}

TEST_F(ParamParamNodeF, findEntryRecursive) {
  EXPECT_EQ(pn.findEntryRecursive("A"), nullptr);
  EXPECT_EQ(pn.findEntryRecursive("B"), &(pn.entries[0]));
  EXPECT_EQ(pn.findEntryRecursive("C"), nullptr);
  EXPECT_EQ(pn.findEntryRecursive("C:D"), &(pn.nodes[0].entries[0]));
  EXPECT_EQ(pn.findEntryRecursive("C:E"), &(pn.nodes[0].entries[1]));
  EXPECT_EQ(pn.findEntryRecursive("F"), nullptr);
  EXPECT_EQ(pn.findEntryRecursive("B:G"), &(pn.nodes[1].entries[0]));
  EXPECT_EQ(pn.findEntryRecursive("X"), nullptr);
  EXPECT_EQ(pn.findEntryRecursive("H:X"), nullptr);
  EXPECT_EQ(pn.findEntryRecursive("H:C:X"), nullptr);
  EXPECT_EQ(pn.findEntryRecursive("H:C:"), nullptr);
}

//Dummy Tree:
// A
// |-B(1)
// |-C
// | |-D(2)
// | |-E(3)
// |-B
// | |-G(4)
// |-F
//   |-H(5)

TEST_F(ParamParamNodeF, insert_ParamNode) {
  tdl::Param::ParamNode node("", "");
  node.entries.push_back(tdl::Param::ParamEntry("H", 5, "", {"advanced"}));
  pn.insert(node, "F");
  EXPECT_NE(pn.findEntryRecursive("F:H"), nullptr);

  pn.insert(node, "F:Z");
  EXPECT_NE(pn.findEntryRecursive("F:Z:H"), nullptr);

  pn.insert(node, "F:Z:");
  EXPECT_NE(pn.findEntryRecursive("F:Z::H"), nullptr);

  pn.insert(node, "FD:ZD:D");
  EXPECT_NE(pn.findEntryRecursive("FD:ZD:D:H"), nullptr);

  node.name = "W";
  pn.insert(node);
  EXPECT_NE(pn.findEntryRecursive("W:H"), nullptr);

  pn.insert(node, "Q");
  EXPECT_NE(pn.findEntryRecursive("QW:H"), nullptr);
}

TEST_F(ParamParamNodeF, insert_ParamEntry) {
  tdl::Param::ParamEntry entry("H", "", "5", {"advanced"});

  pn.insert(entry);
  EXPECT_NE(pn.findEntryRecursive("H"), nullptr);

  pn.insert(entry, "F");
  EXPECT_NE(pn.findEntryRecursive("FH"), nullptr);

  pn.insert(entry, "G:");
  EXPECT_NE(pn.findEntryRecursive("G:H"), nullptr);

  pn.insert(entry, "FD:ZD:D");
  EXPECT_NE(pn.findEntryRecursive("FD:ZD:DH"), nullptr);
}

////////////////// tdl::Param::ParamIterator ////////////////////////////
////////////////////////////////////////////////////////////////////


TEST(ParamParamIterator, constructor) {
  auto pi_ptr = new tdl::Param::ParamIterator();
  EXPECT_NE(pi_ptr, nullptr);
  delete pi_ptr;
}

TEST(ParamParamIterator, construct_from_ParamNode) {
  tdl::Param::ParamNode node;
  auto pi_ptr = new tdl::Param::ParamIterator(node);
  EXPECT_NE(pi_ptr, nullptr);
  delete pi_ptr;
}

TEST(ParamParamIterator, dereferenceOperator) {
  tdl::Param::ParamNode node;
  node.entries.push_back(tdl::Param::ParamEntry("name", "value", "description", {"advanced"}));
  tdl::Param::ParamIterator it(node);
  EXPECT_EQ((*it).name, "name");
  EXPECT_EQ((*it).value, "value");
  EXPECT_EQ((*it).description, "description");
  EXPECT_EQ((*it).tags.count("advanced") == 1, true);
}

TEST(ParamParamIterator, arrowOperator) {
  tdl::Param::ParamNode node;
  node.entries.push_back(tdl::Param::ParamEntry("name", "value", "description", {"advanced"}));
  tdl::Param::ParamIterator it(node);
  EXPECT_EQ(it->name, "name");
  EXPECT_EQ(it->value, "value");
  EXPECT_EQ(it->description, "description");
  EXPECT_EQ(it->tags.count("advanced") == 1, true);
}



class ParamParamIteratorF : public ::testing::Test
{
protected:
//complicated subtree
// Root
//  |-A=1
//  |-R
//  | |
//  | S
//  | |-B=2
//  | |-C=3
//  |-T
//    |-D=4
  tdl::Param::ParamNode root,  r,  s,  t;

  void SetUp() override
  {
    root.name="root";
    r.name="r";
    s.name="s";
    t.name="t";
    root.entries.push_back(tdl::Param::ParamEntry("A", "1", ""));
    s.entries.push_back(tdl::Param::ParamEntry("B", "2", ""));
    s.description="s_desc";
    s.entries.push_back(tdl::Param::ParamEntry("C", "3", ""));
    t.entries.push_back(tdl::Param::ParamEntry("D", "4", ""));
    r.nodes.push_back(s);
    root.nodes.push_back(r);
    root.nodes.push_back(t);
  }
};

TEST_F(ParamParamIteratorF, prefixIncrement) {
  tdl::Param::ParamNode node;
  node.entries.push_back(tdl::Param::ParamEntry("name", "value", "description", {"advanced"}));
  node.entries.push_back(tdl::Param::ParamEntry("name2", "value2", "description2"));
  node.entries.push_back(tdl::Param::ParamEntry("name3", "value3", "description3", {"advanced"}));

  //linear list
  tdl::Param::ParamIterator it(node);
  ++it;
  EXPECT_EQ(it->name, "name2");
  EXPECT_EQ(it->value, "value2");
  EXPECT_EQ(it->description, "description2");
  EXPECT_EQ(it->tags.count("advanced") == 1, false);

  ++it;
  EXPECT_EQ(it->name, "name3");
  EXPECT_EQ(it->value, "value3");
  EXPECT_EQ(it->description, "description3");
  EXPECT_EQ(it->tags.count("advanced") == 1, true);

  ++it;

  //subtree
  node.name = "root";
  node.nodes.push_back(node);
  node.nodes[0].name = "tree";
  node.nodes[0].entries[0].name = "name4";
  node.nodes[0].entries[1].name = "name5";
  node.nodes[0].entries[2].name = "name6";

  it = tdl::Param::ParamIterator(node);
  EXPECT_EQ(it->name, "name");
  EXPECT_EQ(it->value, "value");
  EXPECT_EQ(it->description, "description");
  EXPECT_EQ(it->tags.count("advanced") == 1, true);

  ++it;
  EXPECT_EQ(it->name, "name2");
  EXPECT_EQ(it->value, "value2");
  EXPECT_EQ(it->description, "description2");
  EXPECT_EQ(it->tags.count("advanced") == 1, false);

  ++it;
  EXPECT_EQ(it->name, "name3");
  EXPECT_EQ(it->value, "value3");
  EXPECT_EQ(it->description, "description3");
  EXPECT_EQ(it->tags.count("advanced") == 1, true);

  ++it;
  EXPECT_EQ(it->name, "name4");
  EXPECT_EQ(it->value, "value");
  EXPECT_EQ(it->description, "description");
  EXPECT_EQ(it->tags.count("advanced") == 1, true);

  ++it;
  EXPECT_EQ(it->name, "name5");
  EXPECT_EQ(it->value, "value2");
  EXPECT_EQ(it->description, "description2");
  EXPECT_EQ(it->tags.count("advanced") == 1, false);

  ++it;
  EXPECT_EQ(it->name, "name6");
  EXPECT_EQ(it->value, "value3");
  EXPECT_EQ(it->description, "description3");
  EXPECT_EQ(it->tags.count("advanced") == 1, true);

  ++it;

  //complicated subtree
  tdl::Param::ParamIterator it2(root);

  EXPECT_EQ(it2->name, "A");
  EXPECT_EQ(it2->value, "1");
  ++it2;

  EXPECT_EQ(it2->name, "B");
  EXPECT_EQ(it2->value, "2");
  ++it2;

  EXPECT_EQ(it2->name, "C");
  EXPECT_EQ(it2->value, "3");
  ++it2;

  EXPECT_EQ(it2->name, "D");
  EXPECT_EQ(it2->value, "4");
  ++it2;
}

TEST_F(ParamParamIteratorF, PostfixIncrement) {
  tdl::Param::ParamNode node;
  node.entries.push_back(tdl::Param::ParamEntry("name", "value", "description", {"advanced"}));
  node.entries.push_back(tdl::Param::ParamEntry("name2", "value2", "description2"));
  node.entries.push_back(tdl::Param::ParamEntry("name3", "value3", "description3", {"advanced"}));

  //linear list
  tdl::Param::ParamIterator it(node),  it2(node);

  it2 = it++;
  EXPECT_EQ(it->name, "name2");
  EXPECT_EQ(it->value, "value2");
  EXPECT_EQ(it->description, "description2");
  EXPECT_EQ(it->tags.count("advanced") == 1, false);
  EXPECT_EQ(it2->name, "name");
  EXPECT_EQ(it2->value, "value");
  EXPECT_EQ(it2->description, "description");
  EXPECT_EQ(it2->tags.count("advanced") == 1, true);
}

TEST_F(ParamParamIteratorF, getName) {
  tdl::Param::ParamIterator it(root);

  EXPECT_EQ(it.getName(), "A");
  ++it;

  EXPECT_EQ(it.getName(), "r:s:B");
  ++it;

  EXPECT_EQ(it.getName(), "r:s:C");
  ++it;

  EXPECT_EQ(it.getName(), "t:D");
  ++it;
}


TEST_F(ParamParamIteratorF, compareOperator) {
  tdl::Param::ParamIterator begin(root),  begin2(root),  end;
  EXPECT_EQ(begin == end,  false);
  EXPECT_EQ(begin == begin,  true);
  EXPECT_EQ(begin == begin2,  true);
  EXPECT_EQ(end == end,  true);

  ++begin;
  EXPECT_EQ(begin == begin2,  false);
  EXPECT_EQ(begin == end,  false);
  EXPECT_EQ(begin2 == end,  false);

  ++begin2;
  EXPECT_EQ(begin == begin2,  true);
  EXPECT_EQ(begin == end,  false);
  EXPECT_EQ(begin2 == end,  false);

  ++begin;
  EXPECT_EQ(begin == begin2,  false);
  EXPECT_EQ(begin == end,  false);
  EXPECT_EQ(begin2 == end,  false);

  ++begin2;
  EXPECT_EQ(begin == begin2,  true);
  EXPECT_EQ(begin == end,  false);
  EXPECT_EQ(begin2 == end,  false);

  ++begin;
  EXPECT_EQ(begin == begin2,  false);
  EXPECT_EQ(begin == end,  false);
  EXPECT_EQ(begin2 == end,  false);

  ++begin2;
  EXPECT_EQ(begin == begin2,  true);
  EXPECT_EQ(begin == end,  false);
  EXPECT_EQ(begin2 == end,  false);

  ++begin;
  EXPECT_EQ(begin == begin2,  false);
  EXPECT_EQ(begin == end,  true);
  EXPECT_EQ(begin2 == end,  false);

  ++begin2;
  EXPECT_EQ(begin == begin2,  true);
  EXPECT_EQ(begin == end,  true);
  EXPECT_EQ(begin2 == end,  true);
}

TEST_F(ParamParamIteratorF, inequalityCompareOperator) {
  tdl::Param::ParamIterator begin(root),  begin2(root),  end;
  EXPECT_EQ(begin == end,  false);
  EXPECT_EQ(begin2 == end,  false);
  EXPECT_EQ(begin == begin2,  true);
  EXPECT_EQ(begin == begin,  true);
  EXPECT_EQ(begin2 == begin2,  true);
  EXPECT_EQ(end == end,  true);
}


TEST_F(ParamParamIteratorF, getTrace) {

  //A
  tdl::Param::ParamIterator it(root);
  EXPECT_EQ(it.getTrace().size(), 0);
  ++it;

  //r:s:B
  EXPECT_EQ(it.getTrace().size(), 2);
  EXPECT_EQ(it.getTrace()[0].name, "r");
  EXPECT_EQ(it.getTrace()[0].opened, true);
  EXPECT_EQ(it.getTrace()[1].name, "s");
  EXPECT_EQ(it.getTrace()[1].opened, true);
  EXPECT_EQ(it.getTrace()[1].description, "s_desc");
  ++it;

  //r:s:C
  EXPECT_EQ(it.getTrace().size(), 0);
  ++it;

  //t:D
  EXPECT_EQ(it.getTrace().size(), 3);
  EXPECT_EQ(it.getTrace()[0].name, "s");
  EXPECT_EQ(it.getTrace()[0].opened, false);
  EXPECT_EQ(it.getTrace()[1].name, "r");
  EXPECT_EQ(it.getTrace()[1].opened, false);
  EXPECT_EQ(it.getTrace()[2].name, "t");
  EXPECT_EQ(it.getTrace()[2].opened, true);
  ++it;

  //end()
  EXPECT_EQ(it.getTrace().size(), 1);
  EXPECT_EQ(it.getTrace()[0].name, "t");
  EXPECT_EQ(it.getTrace()[0].opened, false);
}

///////////////////////// Param ///////////////////////////////
///////////////////////////////////////////////////////////////

TEST(Param, constructor) {
  auto d10_ptr = new tdl::Param();
  EXPECT_NE(d10_ptr,  nullptr);
  delete d10_ptr;
}

TEST(Param, exists) {
  tdl::Param p;
  EXPECT_EQ(p.exists(""),  false);
  EXPECT_EQ(p.exists("key"),  false);
  EXPECT_EQ(p.exists("key:value"),  false);
}

TEST(Param, hasSection) {
  tdl::Param p;
  p.addSection("test",  "");
  p.addSection("test:test",  "");
  EXPECT_EQ(p.hasSection("test"),  true);
  EXPECT_EQ(p.hasSection("test:test"),  true);
  EXPECT_EQ(p.hasSection("test:"),  true);
  EXPECT_EQ(p.hasSection("test:test:"),  true);
  EXPECT_EQ(p.hasSection("sectionThatDoesNotExist"),  false);
  EXPECT_EQ(p.hasSection("AnotherSection:"),  false);
  EXPECT_EQ(p.hasSection("Section:WithinSection"),  false);
  EXPECT_EQ(p.hasSection("Section:WithinSection:"),  false);
}

TEST(Param, getValue) {
  tdl::Param p;
  //EXPECT_THROW(p.getValue(""), Exception::ElementNotFound);
  //EXPECT_THROW(p.getValue("key"), Exception::ElementNotFound);
  //EXPECT_THROW(p.getValue("key:value"), Exception::ElementNotFound);
}

TEST(Param, getSectionDescription) {
  tdl::Param p;
  EXPECT_EQ(p.getSectionDescription(""), "");
  EXPECT_EQ(p.getSectionDescription("key"), "");
  EXPECT_EQ(p.getSectionDescription("key:value"), "");
}

TEST(Param, getDescription) {
  tdl::Param p;
  //EXPECT_THROW(p.getDescription(""), Exception::ElementNotFound);
  //EXPECT_THROW(p.getDescription("key"), Exception::ElementNotFound);
  //EXPECT_THROW(p.getDescription("key:value"), Exception::ElementNotFound);
}

TEST(Param, getEntry) {
  tdl::Param p;
  //EXPECT_THROW(p.getEntry(""), Exception::ElementNotFound);
  //EXPECT_THROW(p.getEntry("key"), Exception::ElementNotFound);
  //EXPECT_THROW(p.getEntry("key:value"), Exception::ElementNotFound);
}

TEST(Param, setValue) {
  tdl::Param p;
  p.setValue("key", "value");
  EXPECT_EQ(p.exists("key"),  true);
  EXPECT_EQ(p.getValue("key"),  "value");
  EXPECT_EQ(p.getDescription("key"),  "");
  EXPECT_EQ(p.hasTag("key", "advanced"),  false);

  p.setValue("key", "value", "description", {"advanced"});
  EXPECT_EQ(p.exists("key"),  true);
  EXPECT_EQ(p.getValue("key"),  "value");
  EXPECT_EQ(p.getDescription("key"),  "description");
  EXPECT_EQ(p.hasTag("key", "advanced"),  true);

  p.setValue("key:key", "value2", "description2");
  EXPECT_EQ(p.exists("key"),  true);
  EXPECT_EQ(p.getValue("key"),  "value");
  EXPECT_EQ(p.getDescription("key"),  "description");
  EXPECT_EQ(p.hasTag("key", "advanced"),  true);
  EXPECT_EQ(p.exists("key:key"),  true);
  EXPECT_EQ(p.getValue("key:key"),  "value2");
  EXPECT_EQ(p.getDescription("key:key"),  "description2");
  EXPECT_EQ(p.hasTag("key:key", "advanced"),  false);
}

TEST(Param, getTags) {
  tdl::Param p;
  //EXPECT_THROW(p.getTags("key"), Exception::ElementNotFound);

  p.setValue("key", "value");
  EXPECT_EQ(p.getTags("key").size(), 0);
}

TEST(Param, addTag) {
  tdl::Param p;
  //EXPECT_THROW(p.addTag("key", "bla"), Exception::ElementNotFound);
  std::vector<std::string> error_list;
  error_list.push_back("a, b");
  //EXPECT_THROW(p.addTags("key", error_list), Exception::ElementNotFound);

  p.setValue("key", "value");
  EXPECT_EQ(p.getTags("key").size(), 0);
  p.addTag("key", "advanced");
  EXPECT_EQ(p.getTags("key").size(), 1);
  p.addTag("key", "advanced");
  EXPECT_EQ(p.getTags("key").size(), 1);
  p.addTag("key", "advanced2");
  EXPECT_EQ(p.getTags("key").size(), 2);
}

TEST(Param, hasTag) {
  tdl::Param p;
  //EXPECT_THROW(p.hasTag("key", "bla"), Exception::ElementNotFound);

  p.setValue("key", "value");
  EXPECT_EQ(p.hasTag("key", "advanced"), false);
  EXPECT_EQ(p.hasTag("key", "advanced2"), false);
  p.addTag("key", "advanced");
  EXPECT_EQ(p.hasTag("key", "advanced"), true);
  EXPECT_EQ(p.hasTag("key", "advanced2"), false);
  p.addTag("key", "advanced2");
  EXPECT_EQ(p.hasTag("key", "advanced"), true);
  EXPECT_EQ(p.hasTag("key", "advanced2"), true);
}

TEST(Param, addTags) {
  tdl::Param p;
  //EXPECT_THROW(p.addTags("key",std::vector<std::string>()), Exception::ElementNotFound);
  std::vector<std::string> error_list;
  error_list.push_back("a, b");
  //EXPECT_THROW(p.addTags("key", error_list), Exception::ElementNotFound);

  p.setValue("key", "value");
  EXPECT_EQ(p.hasTag("key", "advanced"), false);
  EXPECT_EQ(p.hasTag("key", "advanced2"), false);
  p.addTags("key", {"advanced", "advanced2"});
  EXPECT_EQ(p.hasTag("key", "advanced"), true);
  EXPECT_EQ(p.hasTag("key", "advanced2"), true);
}

TEST(Param, clearTags) {
  tdl::Param p;
  //EXPECT_THROW(p.clearTags("key"), Exception::ElementNotFound);
  p.setValue("key", "value");
  p.addTag("key", "advanced");
  EXPECT_EQ(p.getTags("key").size(), 1);
  p.clearTags("key");
  EXPECT_EQ(p.getTags("key").size(), 0);
}

TEST(Param, empty) {
  tdl::Param p;
  EXPECT_EQ(p.empty(),  true);
  p.setValue("key", 17.4f);
  EXPECT_EQ(p.empty(),  false);

  tdl::Param p2;
  EXPECT_EQ(p2.empty(),  true);
  p2.setValue("a:key", 17.4f);
  EXPECT_EQ(p2.empty(),  false);
}

TEST(Param, clear) {
  tdl::Param p;
  p.setValue("key", 17.4, "keydesc");
  p.clear();
  EXPECT_EQ(p.empty(),  true);

  tdl::Param p2;
  p2.setValue("a:b:key", 17.4, "keydesc");
  p2.clear();
  EXPECT_EQ(p2.empty(),  true);
}

TEST(Param, size) {
  tdl::Param p;
  EXPECT_EQ(p.size(),  0);
  p.setValue("key", 17.4f);
  EXPECT_EQ(p.size(),  1);
  p.setValue("key", 17.4f);
  EXPECT_EQ(p.size(),  1);
  p.setValue("key:a", 17.5f);
  EXPECT_EQ(p.size(),  2);
  p.setValue("key:a", 18.5f);
  EXPECT_EQ(p.size(),  2);
  p.setValue("key:b", 18.5f);
  EXPECT_EQ(p.size(),  3);
  p.setValue("b", 18.5f);
  EXPECT_EQ(p.size(),  4);
}

TEST(Param, setSectionDescription) {
  tdl::Param p;

  p.setValue("test:test", 47.1);
  p.setValue("test2:test", 47.1);
  p.setValue("test:test2:test", 47.1);
  p.setValue("test:test:test", 47.1);
  p.setSectionDescription("test", "a");
  p.setSectionDescription("test2", "b");
  p.setSectionDescription("test:test", "c");
  p.setSectionDescription("test:test2", "d");
  EXPECT_EQ(p.getSectionDescription("test"),  "a");
  EXPECT_EQ(p.getSectionDescription("test2"),  "b");
  EXPECT_EQ(p.getSectionDescription("test:test"),  "c");
  EXPECT_EQ(p.getSectionDescription("test:test2"),  "d");
}

TEST(Param, shift_operator) {
  tdl::Param p;
  p.setValue("key",  17.5);
  std::stringstream ss;
  ss << p;
  EXPECT_EQ(ss.str(),  "\"key\" -> \"17.5\"\n");

  ss.str("");
  p.setValue("key",  17.5,  "thiskey");
  ss<<p;
  EXPECT_EQ(ss.str(),  "\"key\" -> \"17.5\" (thiskey)\n");

  ss.str("");
  p.clear();
  p.setValue("tree:key",  17.5);
  ss<<p;
  EXPECT_EQ(ss.str(),  "\"tree|key\" -> \"17.5\"\n");
}

TEST(Param, insert) {
  tdl::Param p;
  p.setValue("a", 17, "intdesc");
  p.setValue("n1:b", 17.4f, "floatdesc");
  p.setValue("n1:c", "test, test, test", "stringdesc");
  p.setValue("n2:d", 17.5f);
  p.setSectionDescription("n1", "sectiondesc");

  tdl::Param p2;

  p2.insert("prefix", p);
  EXPECT_EQ(p2.size(), 4);
  EXPECT_EQ(int(p2.getValue("prefixa")),  17);
  EXPECT_EQ(p2.getDescription("prefixa"),  "intdesc");
  EXPECT_FLOAT_EQ(float(p2.getValue("prefixn1:b")),  17.4);
  EXPECT_EQ(p2.getDescription("prefixn1:b"),  "floatdesc");
  EXPECT_EQ(p2.getValue("prefixn1:c"),  "test, test, test");
  EXPECT_EQ(p2.getDescription("prefixn1:c"),  "stringdesc");
  EXPECT_FLOAT_EQ(float(p2.getValue("prefixn2:d")),  17.5);
  EXPECT_EQ(p2.getDescription("prefixn2:d"),  "");
  EXPECT_EQ(p2.getSectionDescription("prefixn1"), "sectiondesc");

  p2.insert("", p);
  EXPECT_EQ(p2.size(), 8);
  EXPECT_EQ(int(p2.getValue("a")),  17);
  EXPECT_EQ(p2.getDescription("a"),  "intdesc");
  EXPECT_FLOAT_EQ(float(p2.getValue("n1:b")),  17.4);
  EXPECT_EQ(p2.getDescription("n1:b"),  "floatdesc");
  EXPECT_EQ(p2.getValue("n1:c"),  "test, test, test");
  EXPECT_EQ(p2.getDescription("n1:c"),  "stringdesc");
  EXPECT_FLOAT_EQ(float(p2.getValue("n2:d")),  17.5);
  EXPECT_EQ(p2.getDescription("n2:d"),  "");
  EXPECT_EQ(p2.getSectionDescription("n1"), "sectiondesc");

  p2.insert("n3:", p);
  EXPECT_EQ(p2.size(), 12);
  EXPECT_EQ(int(p2.getValue("n3:a")),  17);
  EXPECT_EQ(p2.getDescription("n3:a"),  "intdesc");
  EXPECT_FLOAT_EQ(float(p2.getValue("n3:n1:b")),  17.4);
  EXPECT_EQ(p2.getDescription("n3:n1:b"),  "floatdesc");
  EXPECT_EQ(p2.getValue("n3:n1:c"),  "test, test, test");
  EXPECT_EQ(p2.getDescription("n3:n1:c"),  "stringdesc");
  EXPECT_FLOAT_EQ(float(p2.getValue("n3:n2:d")),  17.5);
  EXPECT_EQ(p2.getDescription("n3:n2:d"),  "");
  EXPECT_EQ(p2.getSectionDescription("n3:n1"), "sectiondesc");

  p.clear();
  p.setValue("a", 18, "intdesc");
  p.setValue("n1:b", 17.7f, "floatdesc");
  p.setValue("n1:c", "test, test, test, test", "stringdesc");
  p.setValue("n2:d", 17.8f);

  p2.insert("", p);
  EXPECT_EQ(p2.size(), 12);
  EXPECT_EQ(int(p2.getValue("a")),  18);
  EXPECT_FLOAT_EQ(float(p2.getValue("n1:b")),  17.7);
  EXPECT_EQ(p2.getValue("n1:c"),  "test, test, test, test");
  EXPECT_FLOAT_EQ(float(p2.getValue("n2:d")),  17.8);
}


class ParamF1 : public ::testing::Test
{
protected:
  tdl::Param p_src;

  void SetUp() override
  {
    p_src.setValue("test:float", 17.4f, "floatdesc");
    p_src.setValue("test:string", "test, test, test", "stringdesc");
    p_src.setValue("test:int", 17, "intdesc");
    p_src.setValue("test2:float", 17.5f);
    p_src.setValue("test2:string", "test2");
    p_src.setValue("test2:int", 18);
    p_src.setSectionDescription("test", "sectiondesc");
    p_src.addTags("test:float",  {"a",  "b",  "c"});
  }
};

TEST_F(ParamF1, Param) {
  tdl::Param p2(p_src);
  EXPECT_FLOAT_EQ(float(p2.getValue("test:float")),  17.4);
  EXPECT_EQ(p_src.getDescription("test:float"),  "floatdesc");
  EXPECT_EQ(p2.getValue("test:string"),  "test, test, test");
  EXPECT_EQ(p_src.getDescription("test:string"),  "stringdesc");
  EXPECT_EQ(int(p2.getValue("test:int")),  17);
  EXPECT_EQ(p_src.getDescription("test:int"),  "intdesc");
  EXPECT_FLOAT_EQ(float(p2.getValue("test2:float")),  17.5);
  EXPECT_EQ(p2.getDescription("test2:float"),  "");
  EXPECT_EQ(p2.getValue("test2:string"),  "test2");
  EXPECT_EQ(p2.getDescription("test2:string"),  "");
  EXPECT_EQ(int(p2.getValue("test2:int")),  18);
  EXPECT_EQ(p2.getDescription("test2:int"),  "");
  EXPECT_EQ(p2.getSectionDescription("test"), "sectiondesc");
  EXPECT_EQ(p2.getTags("test:float").size(),  3);
//  EXPECT_EQ(p2.getTags("test:float")  ==  ListUtils::create<std::string>("a, b, c"),  true);
}

TEST_F(ParamF1, assignoperator) {
  tdl::Param p2;
  p2=p_src;
  EXPECT_FLOAT_EQ(float(p2.getValue("test:float")),  17.4);
  EXPECT_EQ(p_src.getDescription("test:float"),  "floatdesc");
  EXPECT_EQ(p2.getValue("test:string"),  "test, test, test");
  EXPECT_EQ(p_src.getDescription("test:string"),  "stringdesc");
  EXPECT_EQ(int(p2.getValue("test:int")),  17);
  EXPECT_EQ(p2.getDescription("test:int"),  "intdesc");
  EXPECT_FLOAT_EQ(float(p2.getValue("test2:float")),  17.5);
  EXPECT_EQ(p2.getDescription("test2:float"),  "");
  EXPECT_EQ(p2.getValue("test2:string"),  "test2");
  EXPECT_EQ(p2.getDescription("test2:string"),  "");
  EXPECT_EQ(int(p2.getValue("test2:int")),  18);
  EXPECT_EQ(p2.getDescription("test2:int"),  "");
  EXPECT_EQ(p2.getSectionDescription("test"), "sectiondesc");
  EXPECT_EQ(p2.getTags("test:float").size(),  3);
//  EXPECT_EQ(p2.getTags("test:float")  ==  ListUtils::create<std::string>("a, b, c"),  true);
}

TEST_F(ParamF1, copy) {
  tdl::Param p2;

  p2 = p_src.copy("notthere:");
  EXPECT_EQ((p2 == tdl::Param()), true);

  p2 = p_src.copy("test:");

  EXPECT_FLOAT_EQ(float(p2.getValue("test:float")),  17.4);
  EXPECT_EQ(p2.getDescription("test:float"),  "floatdesc");
  EXPECT_EQ(p2.getValue("test:string"),  "test, test, test");
  EXPECT_EQ(p2.getDescription("test:int"),  "intdesc");
  EXPECT_EQ(int(p2.getValue("test:int")),  17);
  EXPECT_EQ(p2.getDescription("test:string"),  "stringdesc");
  //EXPECT_THROW(p2.getValue("test2:float"), Exception::ElementNotFound);

  p2 = p_src.copy("test:", true);
  EXPECT_FLOAT_EQ(float(p2.getValue("float")),  17.4);
  EXPECT_EQ(p2.getDescription("float"),  "floatdesc");
  EXPECT_EQ(p2.getValue("string"),  "test, test, test");
  EXPECT_EQ(p2.getDescription("string"),  "stringdesc");

  p2 = p_src.copy("test");
  EXPECT_FLOAT_EQ(float(p2.getValue("test:float")),  17.4);
  EXPECT_EQ(p2.getDescription("test:float"),  "floatdesc");
  EXPECT_EQ(p2.getValue("test:string"),  "test, test, test");
  EXPECT_EQ(p2.getDescription("test:string"),  "stringdesc");
  EXPECT_EQ(int(p2.getValue("test:int")),  17);
  EXPECT_EQ(p2.getDescription("test:int"),  "intdesc");
  EXPECT_FLOAT_EQ(float(p2.getValue("test2:float")),  17.5);
  EXPECT_EQ(p2.getDescription("test2:float"),  "");
  EXPECT_EQ(p2.getValue("test2:string"),  "test2");
  EXPECT_EQ(p2.getDescription("test2:string"),  "");
  EXPECT_EQ(int(p2.getValue("test2:int")),  18);
  EXPECT_EQ(p2.getDescription("test2:int"),  "");
  EXPECT_EQ(p2.getSectionDescription("test"), "sectiondesc");
}

TEST_F(ParamF1, remove) {

  tdl::Param p2(p_src);
  p2.setValue("test:string2", "test, test");

  EXPECT_EQ(p2.size(), 7);

  p2.remove("test");
  EXPECT_EQ(p2.size(), 7);

  p2.remove("test2");
  EXPECT_EQ(p2.size(), 7);

  p2.remove("test:strin");
  EXPECT_EQ(p2.size(), 7);

  p2.remove("test:string");
  EXPECT_EQ(p2.size(), 6);

  p2.remove("test:string2");
  EXPECT_EQ(p2.size(), 5);

  p2.remove("test:float");
  EXPECT_EQ(p2.size(), 4);

  p2.remove("test:int");
  EXPECT_EQ(p2.size(), 3);

  // test deletion of nodes (when using a trailing ':')
  p2 = p_src;
  p2.setValue("test:string2", "an entry");
  p2.setValue("test:string2:e1", "subnode with entries");
  p2.setValue("test:string2:sn2", "subsubnode with entries");
  p2.setValue("test:string2:sn2:e1", "subsubnode with entries");
  p2.setValue("test:string2:sn2:e2", "subsubnode with entries");

  tdl::Param p3 = p2;

  EXPECT_EQ(p2.size(), 11);

  std::cout << "p2 is " << p2 << "\n";

  p2.remove("test:"); // test subtree removal
  EXPECT_EQ(p2.size(), 3);


  p3.remove("test:string2:sn2:e2:"); // nothing should happen
  EXPECT_EQ(p3.size(), 11);

  p3.remove("test:string2:sn2:e1");  // delete one,  the parent node is still populated
  EXPECT_EQ(p3.size(), 10);

  p3.remove("test:string2:sn2:e2");  // delete last entry in subnode sn2
  EXPECT_EQ(p3.size(), 9);


}

TEST_F(ParamF1, removeAll) {
  tdl::Param p2(p_src);

  p2.removeAll("test:float");
  //EXPECT_THROW(p2.getValue("test:float"), Exception::ElementNotFound);
  EXPECT_EQ(p2.getValue("test:string"),  "test, test, test");
  EXPECT_EQ(int(p2.getValue("test:int")),  17);
  EXPECT_FLOAT_EQ(float(p2.getValue("test2:float")),  17.5);
  EXPECT_EQ(p2.getValue("test2:string"),  "test2");
  EXPECT_EQ(int(p2.getValue("test2:int")),  18);
  EXPECT_EQ(p2.getSectionDescription("test"), "sectiondesc");

  p2.removeAll("test:");
  //EXPECT_THROW(p2.getValue("test:string"), Exception::ElementNotFound);
  //EXPECT_THROW(p2.getValue("test:int"), Exception::ElementNotFound);
  EXPECT_FLOAT_EQ(float(p2.getValue("test2:float")),  17.5);
  EXPECT_EQ(p2.getValue("test2:string"),  "test2");
  EXPECT_EQ(int(p2.getValue("test2:int")),  18);

  p2.removeAll("test");
  EXPECT_EQ(p2.empty(), true);

  std::cout << p2;
}


TEST_F(ParamF1, compareOperator) {
  tdl::Param p2(p_src);
  EXPECT_EQ(p_src == p2,  true);
  p2.setValue("test:float", 17.5f);
  EXPECT_EQ(p_src == p2,  false);
  p2 = p_src;
  p2.setValue("test:float3", 17.4f);
  EXPECT_EQ(p_src == p2,  false);
  p2 = p_src;
  p2.removeAll("test:float");
  EXPECT_EQ(p_src == p2,  false);

  //it should be independent of entry order
  tdl::Param p3, p4;
  p3.setValue("1", 1);
  p3.setValue("2", 2);
  p4.setValue("2", 2);
  p4.setValue("1", 1);
  EXPECT_EQ(p3 == p4,  true);

  //it should be independent of node order
  tdl::Param p5, p6;
  p5.setValue("1:1", 1);
  p5.setValue("2:1", 1);
  p6.setValue("2:1", 1);
  p6.setValue("1:1", 1);
  EXPECT_EQ(p5 == p6,  true);

}

TEST_F(ParamF1, setDefaults) {
  tdl::Param defaults;
  defaults.setValue("float", 1.0f, "float");
  defaults.setValue("float2", 2.0f, "float2");
  defaults.setValue("string", "default string1", "string");
  defaults.setValue("string2", "default string2", "string2");
  defaults.setValue("PATH:onlyfordescription", 45.2);

  defaults.setValue("stringlist",std::vector<std::string>{"a", "b", "c"}, "stringlist");
  defaults.setValue("stringlist2",std::vector<std::string>{"d", "e", "f"}, "stringlist2");
//  defaults.setValue("intlist", ListUtils::create<int>("1, 2, 3"), "intlist");
//  defaults.setValue("intlist2", ListUtils::create<int>("11, 22, 33"), "intlist2");
//  defaults.setValue("doublelist", ListUtils::create<double>("1.2, 2.3"), "doublelist");
//  defaults.setValue("doublelist2", ListUtils::create<double>("11.22, 22.33"), "doublelist2");
  defaults.setSectionDescription("PATH", "PATHdesc");
  tdl::Param p2;
  p2.setValue("PATH:float", -1.0f, "PATH:float");
  p2.setValue("PATH:string", "some string", "PATH:string");
  p2.setValue("float", -2.0f, "float");
  p2.setValue("string", "other string", "string");

  p2.setValue("PATH:stringlist",std::vector<std::string>{"d", "a", "v", "i", "d"}, "PATH:stringlist");
  p2.setValue("stringlist",std::vector<std::string>{"r", "o", "c", "k", "s"}, "stringlist");
//  p2.setValue("PATH:intlist2", ListUtils::create<int>("14, 9"), "PATH:intlist2");
//  p2.setValue("intlist",  ListUtils::create<int>("16, 9"), "intlist");
//  p2.setValue("PATH:doublelist2", ListUtils::create<double>("6.66, 6.16"), "PATH:doublelist2");
//  p2.setValue("doublelist", ListUtils::create<double>("1.2, 5.55"), "doublelist");

//  EXPECT_EQ(p2.size(), 10); Fails because ListUtils is missing
EXPECT_EQ(p2.size(), 6);


  p2.setDefaults(defaults);
//  EXPECT_EQ(p2.size(), 16);  Fails because ListUtils is missing
EXPECT_EQ(p2.size(), 10);
  EXPECT_FLOAT_EQ(float(p2.getValue("float")), -2.0);
  EXPECT_EQ(p2.getDescription("float"), "float");
  EXPECT_FLOAT_EQ(float(p2.getValue("float2")), 2.0);
  EXPECT_EQ(p2.getDescription("float2"), "float2");
  EXPECT_EQ(std::string(p2.getValue("string")), "other string");
  EXPECT_EQ(p2.getDescription("string"), "string");
  EXPECT_EQ(std::string(p2.getValue("string2")), "default string2");
  EXPECT_EQ(p2.getDescription("string2"), "string2");
  EXPECT_EQ(p2.getSectionDescription("PATH"), "PATHdesc");

//  EXPECT_EQ(p2.getValue("stringlist")  ==  ListUtils::create<std::string>("r, o, c, k,s"),  true);
//  EXPECT_EQ(p2.getValue("intlist")  ==  ListUtils::create<int>("16, 9"),  true);
//  EXPECT_EQ(p2.getValue("doublelist")  ==  ListUtils::create<double>("1.2, 5.55"),  true);
//  EXPECT_EQ(p2.getValue("stringlist2")  ==  ListUtils::create<std::string>("d, e, f"),  true);
//  EXPECT_EQ(p2.getValue("intlist2")  ==  ListUtils::create<int>("11, 22, 33"),  true);
//  EXPECT_EQ(p2.getValue("doublelist2")  ==  ListUtils::create<double>("11.22, 22.33"),  true);



  p2.setDefaults(defaults, "PATH");

//  EXPECT_EQ(p2.size(), 22); Fails because ListUtils is missing
    EXPECT_EQ(p2.size(), 14);
  EXPECT_FLOAT_EQ(float(p2.getValue("PATH:float")), -1.0);
  EXPECT_EQ(p2.getDescription("PATH:float"), "PATH:float");
  EXPECT_FLOAT_EQ(float(p2.getValue("PATH:float2")), 2.0);
  EXPECT_EQ(p2.getDescription("PATH:float2"), "float2");
  EXPECT_EQ(std::string(p2.getValue("PATH:string")), "some string");
  EXPECT_EQ(p2.getDescription("PATH:string"), "PATH:string");
  EXPECT_EQ(std::string(p2.getValue("PATH:string2")), "default string2");
  EXPECT_EQ(p2.getDescription("PATH:string2"), "string2");
  EXPECT_EQ(p2.getSectionDescription("PATH"), "PATHdesc");
  EXPECT_EQ(p2.getSectionDescription("PATH:PATH"), "PATHdesc");

//  EXPECT_EQ(p2.getValue("PATH:stringlist")  ==  ListUtils::create<std::string>("d, a, v, i, d"),  true);
//  EXPECT_EQ(p2.getValue("PATH:intlist")  ==  ListUtils::create<int>("1, 2, 3"),  true);
//  EXPECT_EQ(p2.getValue("PATH:doublelist")  ==  ListUtils::create<double>("1.2, 2.3"),  true);

}

class ParamF2 : public ::testing::Test
{
protected:
  const char* a1  = "executable";
  const char* a2  = "-a";
  const char* a3  = "av";
  const char* a4  = "-b";
  const char* a5  = "bv";
  const char* a6  = "-c";
  const char* a7  = "cv";
  const char* a8  = "rv1";
  const char* a9  = "rv2";
  const char* a10 = "-1.0";
  const char* command_line[9]; // "executable -a av -b bv -c cv rv1 rv2"
  const char* command_line2[6]; // "executable -a av -b -c cv"
  const char* command_line3[6]; // "executable -a -b -c cv rv1"
  const char* command_line4[10]; // "executable -a -1.0 -b bv -c cv rv1 rv2 -1.0"

  const char* m1 ="mult";
  const char* m2 ="-d";
  const char* m3 ="1.333";
  const char* m4 ="2.23";
  const char* m5 ="3";
  const char* m6 ="-e";
  const char* m7 ="4";
  const char* m8 ="-f";
  const char* m9 ="-g";

  const char* command_line_mult[9];  // "mult -d 1.333 2.23 3 -e 4 -f -g"

  void SetUp() override
  {
    command_line[0] = a1;
    command_line[1] = a2;
    command_line[2] = a3;
    command_line[3] = a4;
    command_line[4] = a5;
    command_line[5] = a6;
    command_line[6] = a7;
    command_line[7] = a8;
    command_line[8] = a9;

    command_line2[0] = a1;
    command_line2[1] = a2;
    command_line2[2] = a3;
    command_line2[3] = a4;
    command_line2[4] = a6;
    command_line2[5] = a7;

    command_line3[0] = a1;
    command_line3[1] = a2;
    command_line3[2] = a4;
    command_line3[3] = a6;
    command_line3[4] = a7;
    command_line3[5] = a8;

    command_line4[0] = a1;
    command_line4[1] = a2;
    command_line4[2] = a10;
    command_line4[3] = a4;
    command_line4[4] = a5;
    command_line4[5] = a6;
    command_line4[6] = a7;
    command_line4[7] = a8;
    command_line4[8] = a9;
    command_line4[9] = a10;

    command_line_mult[0] = m1;
    command_line_mult[1] = m2;
    command_line_mult[2] = m3;
    command_line_mult[3] = m4;
    command_line_mult[4] = m5;
    command_line_mult[5] = m6;
    command_line_mult[6] = m7;
    command_line_mult[7] = m8;
    command_line_mult[8] = m9;
  }
};



TEST_F(ParamF2, parseCommandLine) {
  tdl::Param p2, p3;
  p2.parseCommandLine(9, command_line, "test4");
  p3.setValue("test4:-a", "av");
  p3.setValue("test4:-b", "bv");
  p3.setValue("test4:-c", "cv");
  p3.setValue("test4:misc",std::vector<std::string>{"rv1", "rv2"});
  EXPECT_EQ(p2 == p3, true);

  tdl::Param p20, p30;
  p20.parseCommandLine(6, command_line2);
  p30.setValue("-a", "av");
  p30.setValue("-b", "");
  p30.setValue("-c", "cv");
  EXPECT_EQ(p20 == p30, true);

  tdl::Param p200, p300;
  p200.parseCommandLine(10, command_line4, "test4");
  p300.setValue("test4:-a", "-1.0");
  p300.setValue("test4:-b", "bv");
  p300.setValue("test4:-c", "cv");
  p300.setValue("test4:misc",std::vector<std::string>{"rv1", "rv2", "-1.0"});
  EXPECT_EQ(p200 == p300, true);

}


TEST_F(ParamF2, parseCommandLineMulti) {

  std::map<std::string,std::string> with_one, without, with_multiple;
  with_one["-a"]="a";
  with_one["-b"]="b";
  with_one["-c"]="c";

  with_multiple["-d"] = "d";
  with_multiple["-e"] = "e";
  with_multiple["-f"] = "f";
  with_multiple["-g"] = "g";

  tdl::Param p2, p3;
  p2.parseCommandLine(10, command_line4, with_one, without, with_multiple, "misc_", "unknown_");
  p3.setValue("a", "-1.0");
  p3.setValue("b", "bv");
  p3.setValue("c", "cv");
  p3.setValue("misc_",std::vector<std::string>{"rv1", "rv2", "-1.0"});
  EXPECT_EQ(p2 == p3, true);

  tdl::Param p4, p5;
  p4.parseCommandLine(9, command_line, with_one, without, with_multiple, "misc_", "unknown_");
  p5.setValue("a", "av");
  p5.setValue("b", "bv");
  p5.setValue("c", "cv");
  p5.setValue("misc_",std::vector<std::string>{"rv1", "rv2"});
  EXPECT_EQ(p4 == p5, true);

  with_one.clear();
  with_one["-a"]="a";
  without["-b"]="b";

  tdl::Param p40, p50;
  p40.parseCommandLine(9, command_line, with_one, without, with_multiple, "misc__", "unknown__");
  p50.setValue("a", "av");
  p50.setValue("b", "true");
  p50.setValue("misc__",std::vector<std::string>{"bv", "cv", "rv1", "rv2"});
  p50.setValue("unknown__",std::vector<std::string>{"-c"});
  EXPECT_EQ(p40 == p50, true);
  EXPECT_EQ(p40, p50);
  //"executable -a av -b -c cv"
  tdl::Param p400, p500;
  p400.parseCommandLine(6, command_line2, with_one, without, with_multiple, "misc__", "unknown__");
  p500.setValue("a", "av");
  p500.setValue("b", "true");
  p500.setValue("misc__",std::vector<std::string>{"cv"});
  p500.setValue("unknown__",std::vector<std::string>{"-c"});
  EXPECT_EQ(p400 == p500, true);

  //"executable -a -b -c cv rv1"
  tdl::Param p4000, p5000;
  p4000.parseCommandLine(6, command_line3, with_one, without, with_multiple, "misc__", "unknown__");
  p5000.setValue("a", "");
  p5000.setValue("b", "true");
  p5000.setValue("misc__",std::vector<std::string>{"cv", "rv1"});
  p5000.setValue("unknown__",std::vector<std::string>{"-c"});
  EXPECT_EQ(p4000 == p5000, true);

  // list options:
  tdl::Param p6, p7;
  p6.parseCommandLine(9, command_line_mult, with_one, without, with_multiple, "misc__", "unkown__");
  p7.setValue("d",std::vector<std::string>{"1.333", "2.23", "3"});
  p7.setValue("e",std::vector<std::string>{"4"});
  p7.setValue("f",std::vector<std::string>());
  p7.setValue("g",std::vector<std::string>());
  EXPECT_EQ(p6, p7);

  tdl::Param p8, p9;
  p9.parseCommandLine(4, command_line_mult, with_one, without, with_multiple, "misc__", "unkown__");
  p8.setValue("d",  std::vector<std::string>{"1.333", "2.23"});
  EXPECT_EQ(p9, p8);

}

TEST_F(ParamF2, update) {
  tdl::Param common;
  common.setValue("float", 1.0f, "float");
  common.setValue("float2", 2.0f, "float2");
  common.setValue("string", "default string1", "string");
  common.setValue("string2", "default string2", "string2");
  common.setValue("PATH:onlyfordescription", 45.2);

  common.setValue("stringlist",std::vector<std::string>{"a", "b", "c"}, "stringlist");
  common.setValue("stringlist2",std::vector<std::string>{"d", "e", "f"}, "stringlist2");
//  common.setValue("intlist", ListUtils::create<Int>("1, 2, 3"), "intlist");

  // copy and alter
  tdl::Param old = common;
  //old.setValue("recently_removed_float", 1.1f, "float");  // should not make it into new param
  old.setValue("old_type", "a string", "string");
  old.setValue("some:version", "1.2", "old version");
  old.setValue("some:1:type", "unlabeled", "type");
  old.setValue("some:type", "unlabeled", "type");
  old.setValue("stringlist2",std::vector<std::string>{"d", "e", "f", "altered"}, "stringlist2"); // change some values,  we expect them to show up after update()
//  old.setValue("intlist", ListUtils::create<Int>("3"), "intlist");

  tdl::Param defaults = common;
  defaults.setValue("old_type", 3, "old_type has evolved from string to int"); // as type has changed,  this value should be kept
  defaults.setValue("some:version", "1.9", "new version"); // this value should be kept (due to its reserved name)
  defaults.setValue("some:1:type", "information", "type");   // this value should be kept (due to its reserved name at depth 2)
  defaults.setValue("some:type", "information", "type");   // this value should NOT be kept (wrong depth)
  defaults.setValue("new_value", 3, "new param not present in old");

  tdl::Param expected = defaults;
  expected.setValue("stringlist2",std::vector<std::string>{"d", "e", "f", "altered"}, "stringlist2"); // change some values,  we expect them to show up after update()
//  expected.setValue("intlist", ListUtils::create<Int>("3"), "intlist");
  expected.setValue("some:type", "unlabeled", "type");

  // update()
  defaults.update(old);

  EXPECT_EQ(defaults, expected);
}

TEST_F(ParamF2, merge) {
{
  tdl::Param original;
  original.setValue("a",  2.0f,  "a value");
  original.setMinFloat("a",  0.0f);
  original.setValue("b",  "value",  "b value");

  tdl::Param toMerge;
  toMerge.setValue("b",  "value",  "a value");
  toMerge.setValue("section:a",  "a-value",  "section:a");
  toMerge.setSectionDescription("section",  "section description");
  toMerge.setValue("section:b",  "b-value",  "section:b");

  tdl::Param expected;
  expected.setValue("a",  2.0f,  "a value");
  expected.setMinFloat("a",  0.0f);
  expected.setValue("b",  "value",  "b value");
  expected.setValue("section:a",  "a-value",  "section:a");
  expected.setValue("section:b",  "b-value",  "section:b");
  expected.setSectionDescription("section",  "section description");

  original.merge(toMerge);
  EXPECT_EQ(original,  expected);
  EXPECT_EQ(original.getSectionDescription("section"), expected.getSectionDescription("section"));

  tdl::Param p1;
  p1.setValue("in",  "in-value",  "in-description");
  p1.setValue("out",  "out-value",  "out-description");
  p1.setValue("reference:index",  "reference:index value",  "reference:index description");
  p1.setSectionDescription("reference",  "reference description");
  p1.setValue("algorithm:sub_param",  "algorithm:sub_param value",  "algorithm:sub_param description");

  tdl::Param p2;
  p2.setValue("reference:index",  "reference:index value",  "reference:index description");
  p2.setSectionDescription("reference",  "reference description");
  p2.setValue("algorithm:sub_param",  "algorithm:sub_param value",  "algorithm:sub_param description");
  p2.setValue("algorithm:superimposer:mz_pair_max_distance",  "algorithm:superimposer:mz_pair_max_distance value",  "algorithm:superimposer:mz_pair_max_distance description");
  p2.setSectionDescription("algorithm",  "algorithm description");
  p2.setSectionDescription("algorithm:superimposer",  "algorithm:superimposer description");

  tdl::Param expected_2;
  expected_2.setValue("in",  "in-value",  "in-description");
  expected_2.setValue("out",  "out-value",  "out-description");
  expected_2.setValue("algorithm:sub_param",  "algorithm:sub_param value",  "algorithm:sub_param description");
  expected_2.setValue("reference:index",  "reference:index value",  "reference:index description");
  expected_2.setSectionDescription("reference",  "reference description");
  expected_2.setValue("algorithm:superimposer:mz_pair_max_distance",  "algorithm:superimposer:mz_pair_max_distance value",  "algorithm:superimposer:mz_pair_max_distance description");
  expected_2.setSectionDescription("algorithm",  "algorithm description");
  expected_2.setSectionDescription("algorithm:superimposer",  "algorithm:superimposer description");

  p1.merge(p2);
  EXPECT_EQ(p1,  expected_2);
  EXPECT_EQ(p1.getSectionDescription("algorithm"), expected_2.getSectionDescription("algorithm"));
  EXPECT_EQ(p1.getSectionDescription("algorithm:superimposer"), expected_2.getSectionDescription("algorithm:superimposer"));
  EXPECT_EQ(p1.getSectionDescription("reference"), expected_2.getSectionDescription("reference"));
}
}

TEST_F(ParamF2, findFirst) {
{
  tdl::Param p;
  p.setValue("a:b:leaf",  "leaf_val1",  "leaf 1");
  p.setValue("b:a:leaf",  "leaf_val2",  "leaf 2");
  p.setValue("a:c:leaf",  "leaf_val3",  "leaf 3");
  p.setValue("a:c:another-leaf",  "leaf_val4",  "leaf 3");

  tdl::Param::ParamIterator pI = p.findFirst("leaf");
  EXPECT_EQ(pI.getName(),  "a:b:leaf");

  p.remove("a:b:leaf");
  pI = p.findFirst("leaf");
  EXPECT_EQ(pI.getName(),  "a:c:leaf");

  p.remove("a:c:leaf");
  pI = p.findFirst("leaf");
  EXPECT_EQ(pI.getName(),  "b:a:leaf");

  p.remove("b:a:leaf");
  pI = p.findFirst("leaf");
  EXPECT_EQ(pI  ==  p.end(),  true);
}
}

TEST_F(ParamF2, findNext) {
{
  tdl::Param p;
  p.setValue("a:b:leaf",  "leaf_val1",  "leaf 1");
  p.setValue("b:a:leaf",  "leaf_val2",  "leaf 2");
  p.setValue("a:c:leaf",  "leaf_val3",  "leaf 3");
  p.setValue("a:c:another-leaf",  "leaf_val4",  "leaf 3");

  tdl::Param::ParamIterator pI = p.findFirst("leaf");
  EXPECT_EQ(pI.getName(),  "a:b:leaf");

  pI = p.findNext("leaf",  pI);
  EXPECT_EQ(pI.getName(),  "a:c:leaf");

  pI = p.findNext("leaf",  pI);
  EXPECT_EQ(pI.getName(),  "b:a:leaf");

  pI = p.findNext("leaf",  pI);
  EXPECT_EQ(pI  ==  p.end(),  true);
}
}

TEST_F(ParamF2, begin) {
//        NOT_TESTABLE;
}

TEST_F(ParamF2, end) {
  tdl::Param p;
  p.setValue("a", 5);
  p.setValue("b:a", 6);
  p.setValue("b:b", 7);
  p.setValue("c", 8);

  tdl::Param::ParamIterator it = p.begin();
  EXPECT_EQ(it->name,  "a");
  EXPECT_EQ(it.getName(),  "a");
  EXPECT_EQ((uint32_t)it->value,  5);

  ++it;
  EXPECT_EQ(it->name,  "c");
  EXPECT_EQ(it.getName(),  "c");
  EXPECT_EQ((uint32_t)it->value,  8);

  ++it;
  EXPECT_EQ(it->name,  "a");
  EXPECT_EQ(it.getName(),  "b:a");
  EXPECT_EQ((uint32_t)it->value,  6);

  ++it;
  EXPECT_EQ(it->name,  "b");
  EXPECT_EQ(it.getName(),  "b:b");
  EXPECT_EQ((uint32_t)it->value,  7);

  ++it;
  EXPECT_EQ(it == p.end(), true);
}

TEST_F(ParamF2, setValidStrings) {
  std::vector<std::string> strings;
  strings.push_back("bla");
  tdl::Param d;
  d.setValue("ok", "string");
  d.setValue("dummy", 5);

  d.setValidStrings("ok",strings);
  EXPECT_EQ(d.getEntry("ok").valid_strings == strings,  true);
  //EXPECT_THROW(d.setValidStrings("dummy",strings), Exception::ElementNotFound);
  strings.push_back("sdf,sdfd");
  //EXPECT_THROW(d.setValidStrings("ok",strings), Exception::InvalidParameter);
}

TEST_F(ParamF2, setMinInt) {
  tdl::Param d;
  d.setValue("ok", 4);
  d.setValue("dummy", 5.5);

  d.setMinInt("ok", 4);
  EXPECT_EQ(d.getEntry("ok").min_int, 4);
  //EXPECT_THROW(d.setMinInt("dummy", 4), Exception::ElementNotFound);
}

TEST_F(ParamF2, setMaxInt) {
  tdl::Param d;
  d.setValue("ok", 4);
  d.setValue("dummy", 5.5);

  d.setMaxInt("ok", 4);
  EXPECT_EQ(d.getEntry("ok").max_int, 4);
  //EXPECT_THROW(d.setMaxInt("dummy", 4), Exception::ElementNotFound);
}

TEST_F(ParamF2, setMinFloat) {
  tdl::Param d;
  d.setValue("ok", 4.5);
  d.setValue("dummy", 4);

  d.setMinFloat("ok", 4.0);
  EXPECT_FLOAT_EQ(d.getEntry("ok").min_float, 4.0);
  //EXPECT_THROW(d.setMinFloat("dummy", 4.5), Exception::ElementNotFound);
}

TEST_F(ParamF2, setMaxFloat) {
  tdl::Param d;
  d.setValue("ok", 4.5);
  d.setValue("dummy", 4);

  d.setMaxFloat("ok", 4.0);
  EXPECT_FLOAT_EQ(d.getEntry("ok").max_float, 4.0);
  //EXPECT_THROW(d.setMaxFloat("dummy", 4.5), Exception::ElementNotFound);
}

class ParamF3 : public ::testing::Test
{
protected:
  // warnings for unknown parameters
  // keep outside the scope of a single test to avoid destruction,  leaving
  // OpenMS_Log_warn in an undefined state
  std::ostringstream os;

  void SetUp() override
  {
    // checkDefaults sends its warnings to OPENMS_LOG_WARN so we register our own
    // listener here to check the output
    //OpenMS_Log_warn.remove(std::cout);
    //OpenMS_Log_warn.insert(std::os);
  }
};


TEST_F(ParamF3, checkDefaults) {
    tdl::Param p, d;
    p.setValue("string",std::string("bla"), "string");
    p.setValue("int", 5, "int");
    p.setValue("double", 47.11, "double");

    p.checkDefaults("Test", d, "");
    EXPECT_EQ(os.str() == "", false);

    d.setValue("int", 5, "int");
    d.setValue("double", 47.11, "double");
    os.str("");
  os.clear();
    p.checkDefaults("Test", d, "");
    EXPECT_EQ(os.str() == "", false);

    p.clear();
    p.setValue("pref:string",std::string("bla"), "pref:string");
    p.setValue("pref:int", 5, "pref:int");
    p.setValue("pref:double", 47.11, "pref:double");
    os.str("");
  os.clear();
    p.checkDefaults("Test", d, "pref");
    EXPECT_EQ(os.str() == "", false);

    os.str("");
  os.clear();
    p.checkDefaults("Test2", d, "pref:");
    EXPECT_EQ(os.str() == "", false);

    //check string restrictions
    std::vector<std::string> s_rest = {"a", "b", "c"};
    d.setValue("stringv", "bla", "desc");
    d.setValidStrings("stringv",  s_rest);
    p.clear();
    p.setValue("stringv", "a");
    p.checkDefaults("Param_test", d, "");
    p.setValue("stringv", "d");
    //EXPECT_THROW(p.checkDefaults("Param_test", d, ""), Exception::InvalidParameter);

    //check int restrictions
    d.setValue("intv", 4, "desc");
    d.setMinInt("intv", -4);
    p.clear();
    p.setValue("intv", -4);
    p.checkDefaults("Param_test", d, "");
    p.setValue("intv", 700);
    p.checkDefaults("Param_test", d, "");
    p.setValue("intv", -5);
    //EXPECT_THROW(p.checkDefaults("Param_test", d, ""), Exception::InvalidParameter);

    d.setValue("intv2", 4, "desc");
    d.setMaxInt("intv2", 4);
    p.clear();
    p.setValue("intv2", 4);
    p.checkDefaults("Param_test", d, "");
    p.setValue("intv2", -700);
    p.checkDefaults("Param_test", d, "");
    p.setValue("intv2", 5);
    //EXPECT_THROW(p.checkDefaults("Param_test", d, ""), Exception::InvalidParameter);

    //check double restrictions
    d.setValue("doublev", 4.0, "desc");
    d.setMinFloat("doublev", -4.0);
    p.clear();
    p.setValue("doublev", -4.0);
    p.checkDefaults("Param_test", d, "");
    p.setValue("doublev", 0.0);
    p.checkDefaults("Param_test", d, "");
    p.setValue("doublev", 7.0);
    p.checkDefaults("Param_test", d, "");
    p.setValue("doublev", -4.1);
    //EXPECT_THROW(p.checkDefaults("Param_test", d, ""), Exception::InvalidParameter);

    d.setValue("doublev2", 4.0, "desc");
    d.setMaxFloat("doublev2", 4.0);
    p.clear();
    p.setValue("doublev2", 4.0);
    p.checkDefaults("Param_test", d, "");
    p.setValue("doublev2", -700.0);
    p.checkDefaults("Param_test", d, "");
    p.setValue("doublev2", 4.1);
    //EXPECT_THROW(p.checkDefaults("Param_test", d, ""), Exception::InvalidParameter);

    //check list restrictions
    std::vector<std::string> s_rest1 = {"a", "b", "c"};
    d.setValue("stringlist",std::vector<std::string>{"aaa", "abc", "cab"}, "desc");
    d.setValidStrings("stringlist",  s_rest);
    p.clear();
    p.setValue("stringlist",std::vector<std::string>{"a", "c"});
    p.checkDefaults("Param_test", d, "");
    p.setValue("stringlist",std::vector<std::string>{"aa", "dd", "cc"});
    //EXPECT_THROW(p.checkDefaults("Param_test", d, ""), Exception::InvalidParameter);


    //wrong type
    p.clear();
    p.setValue("doublev", 4);
    //EXPECT_THROW(p.checkDefaults("Param_test", d, ""), Exception::InvalidParameter);
    p.clear();
    p.setValue("intv", "bla");
    //EXPECT_THROW(p.checkDefaults("Param_test", d, ""), Exception::InvalidParameter);
    p.clear();
    p.setValue("stringv", 4.5);
    //EXPECT_THROW(p.checkDefaults("Param_test", d, ""), Exception::InvalidParameter);
}

TEST_F(ParamF3, update) {
//  NOT_TESTABLE // see full implementation below
}
