#include <gtest/gtest.h>

#include <tdl/ParamCTDFile.h>

TEST(ParamCTDFile_test, default_construction)
{
  auto ptr = new tdl::ParamCTDFile();
  EXPECT_NE(ptr, nullptr);
  delete ptr;
}

class ParamCTDFileF : public ::testing::Test
{
protected:
  tdl::Param p;

  void SetUp() override
  {
    p.setValue("test:float",17.4f,"floatdesc");
    p.setValue("test:string","test,test,test","stringdesc");
    p.setValue("test:int",17,"intdesc");
    p.setValue("test2:float",17.5f);
    p.setValue("test2:string","test2");
    p.setValue("test2:int",18);
    p.setSectionDescription("test","sectiondesc");
    p.addTags("test:float", {"a","b","c"});
  }
};

TEST_F(ParamCTDFileF, store) {
  tdl::ParamCTDFile paramFile;

  //exception
  tdl::Param p300;
  tdl::ToolInfo info = {"a", "a", "a", "a", "a", std::vector<std::string>()};
  EXPECT_THROW(paramFile.store("/does/not/exist/FileDoesNotExist.ctd", p300, info), std::ios::failure);


  {
    tdl::Param p2(p);
    p2.setValue("test:a:a1", 47.1,"a1desc\"<>\nnewline");
    p2.setValue("test:b:b1", 47.1);
    p2.setSectionDescription("test:b","bdesc\"<>\nnewline");
    p2.setValue("test2:a:a1", 47.1);
    p2.setValue("test2:b:b1", 47.1,"",{"advanced"});
    p2.setSectionDescription("test2:a","adesc");

    std::ostringstream buffer;
    paramFile.writeCTDToStream(&buffer, p2, info);
    EXPECT_EQ(buffer.str(), R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7" version="a" name="a" docurl="a" category="a" >
  <description><![CDATA[a]]></description>
  <manual><![CDATA[a]]></manual>
  <citations>
  </citations>
  <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
    <NODE name="test" description="sectiondesc">
      <ITEM name="float" value="17.399999618530273" type="double" description="floatdesc" required="false" advanced="false" tags="a,b,c" />
      <ITEM name="string" value="test,test,test" type="string" description="stringdesc" required="false" advanced="false" />
      <ITEM name="int" value="17" type="int" description="intdesc" required="false" advanced="false" />
      <NODE name="a" description="">
        <ITEM name="a1" value="47.100000000000001" type="double" description="a1desc&quot;&lt;&gt;#br#newline" required="false" advanced="false" />
      </NODE>
      <NODE name="b" description="bdesc&quot;&lt;&gt;#br#newline">
        <ITEM name="b1" value="47.100000000000001" type="double" description="" required="false" advanced="false" />
      </NODE>
    </NODE>
    <NODE name="test2" description="">
      <ITEM name="float" value="17.5" type="double" description="" required="false" advanced="false" />
      <ITEM name="string" value="test2" type="string" description="" required="false" advanced="false" />
      <ITEM name="int" value="18" type="int" description="" required="false" advanced="false" />
      <NODE name="a" description="adesc">
        <ITEM name="a1" value="47.100000000000001" type="double" description="" required="false" advanced="false" />
      </NODE>
      <NODE name="b" description="">
        <ITEM name="b1" value="47.100000000000001" type="double" description="" required="false" advanced="true" />
      </NODE>
    </NODE>
  </PARAMETERS>
</tool>
)");
  }


  //advanced
  {
    tdl::Param p7;
    p7.setValue("true",5,"",{"advanced"});
    p7.setValue("false",5,"");
    std::ostringstream buffer;
    paramFile.writeCTDToStream(&buffer, p7, info);
    EXPECT_EQ(buffer.str(), R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7" version="a" name="a" docurl="a" category="a" >
  <description><![CDATA[a]]></description>
  <manual><![CDATA[a]]></manual>
  <citations>
  </citations>
  <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
    <ITEM name="true" value="5" type="int" description="" required="false" advanced="true" />
    <ITEM name="false" value="5" type="int" description="" required="false" advanced="false" />
  </PARAMETERS>
</tool>
)");
  }

  {
    //restrictions
    tdl::Param p5;
    p5.setValue("int",5);
    p5.setValue("int_min",5);
    p5.setMinInt("int_min",4);
    p5.setValue("int_max",5);
    p5.setMaxInt("int_max",6);
    p5.setValue("int_min_max",5);
    p5.setMinInt("int_min_max",0);
    p5.setMaxInt("int_min_max",10);

    p5.setValue("float",5.1);
    p5.setValue("float_min",5.1);
    p5.setMinFloat("float_min",4.1);
    p5.setValue("float_max",5.1);
    p5.setMaxFloat("float_max",6.1);
    p5.setValue("float_min_max",5.1);
    p5.setMinFloat("float_min_max",0.1);
    p5.setMaxFloat("float_min_max",10.1);

    std::vector<std::string> strings;
    p5.setValue("string","bli");
    strings.push_back("bla");
    strings.push_back("bluff");
    p5.setValue("string_2","bla");
    p5.setValidStrings("string_2",strings);

    //list restrictions
    std::vector<std::string> strings2 = {"xml", "txt"};
    p5.setValue("stringlist2",std::vector<std::string>{"a.txt","b.xml","c.pdf"});
    p5.setValue("stringlist",std::vector<std::string>{"aa.C","bb.h","c.doxygen"});
    p5.setValidStrings("stringlist2",strings2);

    p5.setValue("intlist",  std::vector<int>{2,5,10});
    p5.setValue("intlist2", std::vector<int>{2,5,10});
    p5.setValue("intlist3", std::vector<int>{2,5,10});
    p5.setValue("intlist4", std::vector<int>{2,5,10});
    p5.setMinInt("intlist2",1);
    p5.setMaxInt("intlist3",11);
    p5.setMinInt("intlist4",0);
    p5.setMaxInt("intlist4",15);

    p5.setValue("doublelist",  std::vector<double>{1.2,3.33,4.44});
    p5.setValue("doublelist2", std::vector<double>{1.2,3.33,4.44});
    p5.setValue("doublelist3", std::vector<double>{1.2,3.33,4.44});
    p5.setValue("doublelist4", std::vector<double>{1.2,3.33,4.44});

    p5.setMinFloat("doublelist2",1.1);
    p5.setMaxFloat("doublelist3",4.45);
    p5.setMinFloat("doublelist4",0.1);
    p5.setMaxFloat("doublelist4",5.8);

    std::ostringstream buffer;
    paramFile.writeCTDToStream(&buffer, p5, info);
    EXPECT_EQ(buffer.str(), R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7" version="a" name="a" docurl="a" category="a" >
  <description><![CDATA[a]]></description>
  <manual><![CDATA[a]]></manual>
  <citations>
  </citations>
  <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
    <ITEM name="int" value="5" type="int" description="" required="false" advanced="false" />
    <ITEM name="int_min" value="5" type="int" description="" required="false" advanced="false" restrictions="4:" />
    <ITEM name="int_max" value="5" type="int" description="" required="false" advanced="false" restrictions=":6" />
    <ITEM name="int_min_max" value="5" type="int" description="" required="false" advanced="false" restrictions="0:10" />
    <ITEM name="float" value="5.1" type="double" description="" required="false" advanced="false" />
    <ITEM name="float_min" value="5.1" type="double" description="" required="false" advanced="false" restrictions="4.100000:" />
    <ITEM name="float_max" value="5.1" type="double" description="" required="false" advanced="false" restrictions=":6.100000" />
    <ITEM name="float_min_max" value="5.1" type="double" description="" required="false" advanced="false" restrictions="0.100000:10.100000" />
    <ITEM name="string" value="bli" type="string" description="" required="false" advanced="false" />
    <ITEM name="string_2" value="bla" type="string" description="" required="false" advanced="false" restrictions="bla,bluff" />
    <ITEMLIST name="stringlist2" type="string" description="" required="false" advanced="false" restrictions="xml,txt" >
      <LISTITEM value="a.txt"/>
      <LISTITEM value="b.xml"/>
      <LISTITEM value="c.pdf"/>
    </ITEMLIST>
    <ITEMLIST name="stringlist" type="string" description="" required="false" advanced="false" >
      <LISTITEM value="aa.C"/>
      <LISTITEM value="bb.h"/>
      <LISTITEM value="c.doxygen"/>
    </ITEMLIST>
    <ITEMLIST name="intlist" type="int" description="" required="false" advanced="false" >
      <LISTITEM value="2"/>
      <LISTITEM value="5"/>
      <LISTITEM value="10"/>
    </ITEMLIST>
    <ITEMLIST name="intlist2" type="int" description="" required="false" advanced="false" restrictions="1:" >
      <LISTITEM value="2"/>
      <LISTITEM value="5"/>
      <LISTITEM value="10"/>
    </ITEMLIST>
    <ITEMLIST name="intlist3" type="int" description="" required="false" advanced="false" restrictions=":11" >
      <LISTITEM value="2"/>
      <LISTITEM value="5"/>
      <LISTITEM value="10"/>
    </ITEMLIST>
    <ITEMLIST name="intlist4" type="int" description="" required="false" advanced="false" restrictions="0:15" >
      <LISTITEM value="2"/>
      <LISTITEM value="5"/>
      <LISTITEM value="10"/>
    </ITEMLIST>
    <ITEMLIST name="doublelist" type="double" description="" required="false" advanced="false" >
      <LISTITEM value="1.2"/>
      <LISTITEM value="3.33"/>
      <LISTITEM value="4.44"/>
    </ITEMLIST>
    <ITEMLIST name="doublelist2" type="double" description="" required="false" advanced="false" restrictions="1.100000:" >
      <LISTITEM value="1.2"/>
      <LISTITEM value="3.33"/>
      <LISTITEM value="4.44"/>
    </ITEMLIST>
    <ITEMLIST name="doublelist3" type="double" description="" required="false" advanced="false" restrictions=":4.450000" >
      <LISTITEM value="1.2"/>
      <LISTITEM value="3.33"/>
      <LISTITEM value="4.44"/>
    </ITEMLIST>
    <ITEMLIST name="doublelist4" type="double" description="" required="false" advanced="false" restrictions="0.100000:5.800000" >
      <LISTITEM value="1.2"/>
      <LISTITEM value="3.33"/>
      <LISTITEM value="4.44"/>
    </ITEMLIST>
  </PARAMETERS>
</tool>
)");

  }
}

TEST_F(ParamCTDFileF, writeCTDToStream)
{
  tdl::Param p;
  p.setValue("stringlist", std::vector<std::string>{"a","bb","ccc"}, "StringList Description");
  p.setValue("intlist", std::vector<int>{1,22,333});
  p.setValue("item", std::string{"bla"});
  p.setValue("stringlist2", std::vector<std::string>());
  p.setValue("intlist2", std::vector<int>{});
  p.setValue("item1", 7);
  p.setValue("intlist3", std::vector<int>{1});
  p.setValue("stringlist3", std::vector<std::string>{"1"});
  p.setValue("item3", 7.6);
  p.setValue("doublelist", std::vector<double>{1.22,2.33,4.55});
  p.setValue("doublelist3", std::vector<double>{1.4});
  p.setValue("file_parameter", "", "This is a file parameter.");
  p.addTag("file_parameter", "input file");
  p.setValidStrings("file_parameter", std::vector<std::string>{"*.mzML","*.mzXML"});
  p.setValue("advanced_parameter", "", "This is an advanced parameter.", {"advanced"});
  p.setValue("flag", "false", "This is a flag i.e. in a command line input it does not need a value.");
  p.setValidStrings("flag",{"true","false"});
  p.setValue("noflagJustTrueFalse", "true", "This is not a flag but has a boolean meaning.");
  p.setValidStrings("noflagJustTrueFalse", {"true","false"});

  tdl::ParamCTDFile paramFile;
  tdl::ToolInfo info = {"2.6.0-pre-STL-ParamCTD-2021-06-02",
                        "AccurateMassSearch",
                        "http://www.openms.de/doxygen/nightly/html/UTILS_AccurateMassSearch.html",
                        "Utilities",
                        "Match MS signals to molecules from a database by mass.",
                        {"10.1038/nmeth.3959"}};
  std::ostringstream buffer;
  paramFile.writeCTDToStream(&buffer, p, info);
  EXPECT_EQ(buffer.str(), R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7" version="2.6.0-pre-STL-ParamCTD-2021-06-02" name="AccurateMassSearch" docurl="http://www.openms.de/doxygen/nightly/html/UTILS_AccurateMassSearch.html" category="Utilities" >
  <description><![CDATA[Match MS signals to molecules from a database by mass.]]></description>
  <manual><![CDATA[Match MS signals to molecules from a database by mass.]]></manual>
  <citations>
    <citation doi="10.1038/nmeth.3959" url="" />
  </citations>
  <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
    <ITEMLIST name="stringlist" type="string" description="StringList Description" required="false" advanced="false" >
      <LISTITEM value="a"/>
      <LISTITEM value="bb"/>
      <LISTITEM value="ccc"/>
    </ITEMLIST>
    <ITEMLIST name="intlist" type="int" description="" required="false" advanced="false" >
      <LISTITEM value="1"/>
      <LISTITEM value="22"/>
      <LISTITEM value="333"/>
    </ITEMLIST>
    <ITEM name="item" value="bla" type="string" description="" required="false" advanced="false" />
    <ITEMLIST name="stringlist2" type="string" description="" required="false" advanced="false" >
    </ITEMLIST>
    <ITEMLIST name="intlist2" type="int" description="" required="false" advanced="false" >
    </ITEMLIST>
    <ITEM name="item1" value="7" type="int" description="" required="false" advanced="false" />
    <ITEMLIST name="intlist3" type="int" description="" required="false" advanced="false" >
      <LISTITEM value="1"/>
    </ITEMLIST>
    <ITEMLIST name="stringlist3" type="string" description="" required="false" advanced="false" >
      <LISTITEM value="1"/>
    </ITEMLIST>
    <ITEM name="item3" value="7.6" type="double" description="" required="false" advanced="false" />
    <ITEMLIST name="doublelist" type="double" description="" required="false" advanced="false" >
      <LISTITEM value="1.22"/>
      <LISTITEM value="2.33"/>
      <LISTITEM value="4.55"/>
    </ITEMLIST>
    <ITEMLIST name="doublelist3" type="double" description="" required="false" advanced="false" >
      <LISTITEM value="1.4"/>
    </ITEMLIST>
    <ITEM name="file_parameter" value="" type="input-file" description="This is a file parameter." required="false" advanced="false" supported_formats="*.mzML,*.mzXML" />
    <ITEM name="advanced_parameter" value="" type="string" description="This is an advanced parameter." required="false" advanced="true" />
    <ITEM name="flag" value="false" type="bool" description="This is a flag i.e. in a command line input it does not need a value." required="false" advanced="false" />
    <ITEM name="noflagJustTrueFalse" value="true" type="string" description="This is not a flag but has a boolean meaning." required="false" advanced="false" restrictions="true,false" />
  </PARAMETERS>
</tool>
)");
}

TEST_F(ParamCTDFileF, storing_of_lists) {
  tdl::ParamCTDFile paramFile;

  tdl::Param p;
  p.setValue("stringlist", std::vector<std::string>{"a","bb","ccc"});
  p.setValue("intlist", std::vector<int>{1,22,333});
  p.setValue("item", std::string("bla"));
  p.setValue("stringlist2", std::vector<std::string>());
  p.setValue("intlist2", std::vector<int>());
  p.setValue("item1", 7);
  p.setValue("intlist3", std::vector<int>(1));
  p.setValue("stringlist3", std::vector<std::string>{"1"});
  p.setValue("item3", 7.6);
  p.setValue("doublelist", std::vector<double>{1.22,2.33,4.55});
  p.setValue("doublelist2", std::vector<double>{});
  p.setValue("doublelist3", std::vector<double>{1.4});
  //store
  tdl::ToolInfo info = {"a", "b", "c", "d", "e", {"f"}};
  std::ostringstream buffer;
  paramFile.writeCTDToStream(&buffer, p, info);
  EXPECT_EQ(buffer.str(), R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7" version="a" name="b" docurl="c" category="d" >
  <description><![CDATA[e]]></description>
  <manual><![CDATA[e]]></manual>
  <citations>
    <citation doi="f" url="" />
  </citations>
  <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
    <ITEMLIST name="stringlist" type="string" description="" required="false" advanced="false" >
      <LISTITEM value="a"/>
      <LISTITEM value="bb"/>
      <LISTITEM value="ccc"/>
    </ITEMLIST>
    <ITEMLIST name="intlist" type="int" description="" required="false" advanced="false" >
      <LISTITEM value="1"/>
      <LISTITEM value="22"/>
      <LISTITEM value="333"/>
    </ITEMLIST>
    <ITEM name="item" value="bla" type="string" description="" required="false" advanced="false" />
    <ITEMLIST name="stringlist2" type="string" description="" required="false" advanced="false" >
    </ITEMLIST>
    <ITEMLIST name="intlist2" type="int" description="" required="false" advanced="false" >
    </ITEMLIST>
    <ITEM name="item1" value="7" type="int" description="" required="false" advanced="false" />
    <ITEMLIST name="intlist3" type="int" description="" required="false" advanced="false" >
      <LISTITEM value="0"/>
    </ITEMLIST>
    <ITEMLIST name="stringlist3" type="string" description="" required="false" advanced="false" >
      <LISTITEM value="1"/>
    </ITEMLIST>
    <ITEM name="item3" value="7.6" type="double" description="" required="false" advanced="false" />
    <ITEMLIST name="doublelist" type="double" description="" required="false" advanced="false" >
      <LISTITEM value="1.22"/>
      <LISTITEM value="2.33"/>
      <LISTITEM value="4.55"/>
    </ITEMLIST>
    <ITEMLIST name="doublelist2" type="double" description="" required="false" advanced="false" >
    </ITEMLIST>
    <ITEMLIST name="doublelist3" type="double" description="" required="false" advanced="false" >
      <LISTITEM value="1.4"/>
    </ITEMLIST>
  </PARAMETERS>
</tool>
)");
}

TEST_F(ParamCTDFileF, escaping_of_characters) {
  tdl::Param p;
  tdl::ParamCTDFile paramFile;

  p.setValue("string", std::string{"bla"},"string");
  p.setValue("string_with_ampersand", std::string{"bla2&blubb"}, "string with ampersand");
  p.setValue("string_with_ampersand_in_descr", std::string{"blaxx"}, "String with & in description");
  p.setValue("string_with_single_quote", std::string{"bla'xxx"}, "String with single quotes");
  p.setValue("string_with_single_quote_in_descr", std::string{"blaxxx"}, "String with ' quote in description");
  p.setValue("string_with_double_quote", std::string{"bla\"xxx"}, "String with double quote");
  p.setValue("string_with_double_quote_in_descr", std::string{"bla\"xxx"}, "String with \" description");
  p.setValue("string_with_greater_sign", std::string{"bla>xxx"}, "String with greater sign");
  p.setValue("string_with_greater_sign_in_descr", std::string{"bla greater xxx"}, "String with >");
  p.setValue("string_with_less_sign", std::string{"bla<xxx"}, "String with less sign");
  p.setValue("string_with_less_sign_in_descr", std::string{"bla less sign_xxx"}, "String with less sign <");

  tdl::ToolInfo info = {"a", "a", "a", "a", "a", {"a"}};
  std::ostringstream buffer;
  paramFile.writeCTDToStream(&buffer, p, info);
  EXPECT_EQ(buffer.str(), R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7" version="a" name="a" docurl="a" category="a" >
  <description><![CDATA[a]]></description>
  <manual><![CDATA[a]]></manual>
  <citations>
    <citation doi="a" url="" />
  </citations>
  <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
    <ITEM name="string" value="bla" type="string" description="string" required="false" advanced="false" />
    <ITEM name="string_with_ampersand" value="bla2&amp;blubb" type="string" description="string with ampersand" required="false" advanced="false" />
    <ITEM name="string_with_ampersand_in_descr" value="blaxx" type="string" description="String with &amp; in description" required="false" advanced="false" />
    <ITEM name="string_with_single_quote" value="bla&apos;xxx" type="string" description="String with single quotes" required="false" advanced="false" />
    <ITEM name="string_with_single_quote_in_descr" value="blaxxx" type="string" description="String with &apos; quote in description" required="false" advanced="false" />
    <ITEM name="string_with_double_quote" value="bla&quot;xxx" type="string" description="String with double quote" required="false" advanced="false" />
    <ITEM name="string_with_double_quote_in_descr" value="bla&quot;xxx" type="string" description="String with &quot; description" required="false" advanced="false" />
    <ITEM name="string_with_greater_sign" value="bla&gt;xxx" type="string" description="String with greater sign" required="false" advanced="false" />
    <ITEM name="string_with_greater_sign_in_descr" value="bla greater xxx" type="string" description="String with &gt;" required="false" advanced="false" />
    <ITEM name="string_with_less_sign" value="bla&lt;xxx" type="string" description="String with less sign" required="false" advanced="false" />
    <ITEM name="string_with_less_sign_in_descr" value="bla less sign_xxx" type="string" description="String with less sign &lt;" required="false" advanced="false" />
  </PARAMETERS>
</tool>
)");
}
