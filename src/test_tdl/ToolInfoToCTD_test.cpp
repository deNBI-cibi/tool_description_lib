// SPDX-FileCopyrightText: 2006-2024, Knut Reinert & Freie Universität Berlin
// SPDX-FileCopyrightText: 2016-2024, Knut Reinert & MPI für molekulare Genetik
// SPDX-License-Identifier: CC0-1.0

#include "utils.h"

void testToolInfo() {
    { // Testing empty ToolInfo (making sure only minimum number of fields are being printed)
        auto output = convertToCTD(tdl::ToolInfo{DESINIT(.metaInfo =){
            DESINIT(.version =){},
            DESINIT(.name =){},
            DESINIT(.docurl =){},
            DESINIT(.category =){},
            DESINIT(.description =){},
            DESINIT(.executableName =){},
            DESINIT(.citations =){},
        }});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" />
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }

    { // Testing filling all fields (with exception of citations for better overview)
        auto output = convertToCTD(tdl::ToolInfo{DESINIT(.metaInfo =){
            DESINIT(.version =){"7.6.5"},
            DESINIT(.name =){"testApp"},
            DESINIT(.docurl =){"example.com"},
            DESINIT(.category =){"test-category"},
            DESINIT(.description =){"a demonstration of how this tool info works"},
            DESINIT(.executableName =){"test"},
            DESINIT(.citations =){},
        }});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7" version="7.6.5" name="testApp" docurl="example.com" category="test-category">
    <description><![CDATA[a demonstration of how this tool info works]]></description>
    <manual><![CDATA[a demonstration of how this tool info works]]></manual>
    <executableName><![CDATA[test]]></executableName>
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" />
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Testing citations list
        auto output = convertToCTD(tdl::ToolInfo{DESINIT(.metaInfo =){
            DESINIT(.version =){},
            DESINIT(.name =){},
            DESINIT(.docurl =){},
            DESINIT(.category =){},
            DESINIT(.description =){},
            DESINIT(.executableName =){},
            DESINIT(.citations =){{"doi:123", "https://en.wikipedia.org/wiki/Meaning_of_life"},
                                  {"doi:456", "https://en.wikipedia.org/wiki/Turing_completeness"}},
        }});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations>
        <citation doi="doi:123" url="https://en.wikipedia.org/wiki/Meaning_of_life" />
        <citation doi="doi:456" url="https://en.wikipedia.org/wiki/Turing_completeness" />
    </citations>
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" />
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
}

void testNodeSingleInt() {
    { // Single Int, no tags, no limits
        auto doc = tdl::ToolInfo{{},
                                 {tdl::Node{DESINIT(.name =) "foo",
                                            DESINIT(.description =) "testing a single int with no tags and no limits",
                                            DESINIT(.tags =){},
                                            DESINIT(.value =) tdl::IntValue{5}}}};

        auto output   = convertToCTD(doc);
        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="5" type="int" description="testing a single int with no tags and no limits" required="false" advanced="false" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Single Int, no tags, min limits
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){
                DESINIT(.version =){},
                DESINIT(.name =){},
                DESINIT(.docurl =){},
                DESINIT(.category =){},
                DESINIT(.description =){},
                DESINIT(.executableName =){},
                DESINIT(.citations =){},
            },
            DESINIT(.params =){{DESINIT(.name =) "foo",
                                DESINIT(.description =) "testing a single int with no tags and a min limit",
                                DESINIT(.tags =){},
                                DESINIT(.value =) tdl::IntValue{5, 1}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="5" type="int" description="testing a single int with no tags and a min limit" required="false" advanced="false" restrictions="1:" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Single Int, no tags, a max limits
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){
                {DESINIT(.name =) "foo",
                 DESINIT(.description =) "testing a single int with no tags and a max limit",
                 DESINIT(.tags =){},
                 DESINIT(.value =)
                     tdl::IntValue{DESINIT(.value =) 5, DESINIT(.minLimit =) std::nullopt, DESINIT(.maxLimit =) 9}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="5" type="int" description="testing a single int with no tags and a max limit" required="false" advanced="false" restrictions=":9" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Single Int, no tags, with limits
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){
                {DESINIT(.name =) "foo",
                 DESINIT(.description =) "testing a single int with no tags and with limits",
                 DESINIT(.tags =){},
                 DESINIT(.value =)
                     tdl::IntValue{DESINIT(.value =) 5, DESINIT(.minLimit =) 2, DESINIT(.maxLimit =) 11}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="5" type="int" description="testing a single int with no tags and with limits" required="false" advanced="false" restrictions="2:11" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
}

void testNodeSingleTypes() {
    { // Single double, no tags, with limits
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){
                {DESINIT(.name =) "foo",
                 DESINIT(.description =) "testing a single double with no tags and with limits",
                 DESINIT(.tags =){},
                 DESINIT(.value =)
                     tdl::DoubleValue{DESINIT(.value =) 3.5, DESINIT(.minLimit =) 1.25, DESINIT(.maxLimit =) 5.125}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="3.5" type="double" description="testing a single double with no tags and with limits" required="false" advanced="false" restrictions="1.25:5.125" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Single string, no tags, with validator list
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){
                {DESINIT(.name =) "foo",
                 DESINIT(.description =) "testing a single string with no tags and with a validator list",
                 DESINIT(.tags =){},
                 DESINIT(.value =) tdl::StringValue{"hallo", {{"a", "b", "c", "uiae", "dtrn", "hallo"}}}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="hallo" type="string" description="testing a single string with no tags and with a validator list" required="false" advanced="false" restrictions="a,b,c,uiae,dtrn,hallo" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
}

void testNodeListTypes() {
    { // List of ints, no tags, with limits
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){{DESINIT(.name =) "foo",
                                DESINIT(.description =) "testing a list of ints with no tags and with limits",
                                DESINIT(.tags =){},
                                DESINIT(.value =) tdl::IntValueList{DESINIT(.value =){7, 3, 4},
                                                                    DESINIT(.minLimit =) - 1,
                                                                    DESINIT(.maxLimit =) 99}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEMLIST name="foo" type="int" description="testing a list of ints with no tags and with limits" required="false" advanced="false" restrictions="-1:99">
            <LISTITEM value="7" />
            <LISTITEM value="3" />
            <LISTITEM value="4" />
        </ITEMLIST>
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }

    { // List of doubles, no tags, with limits
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){{DESINIT(.name =) "foo",
                                DESINIT(.description =) "testing a list of doubles with no tags and with limits",
                                DESINIT(.tags =){},
                                DESINIT(.value =) tdl::DoubleValueList{DESINIT(.value =){2.5, 3.5, 4.5},
                                                                       DESINIT(.minLimit =) 1.25,
                                                                       DESINIT(.maxLimit =) 5.125}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEMLIST name="foo" type="double" description="testing a list of doubles with no tags and with limits" required="false" advanced="false" restrictions="1.25:5.125">
            <LISTITEM value="2.5" />
            <LISTITEM value="3.5" />
            <LISTITEM value="4.5" />
        </ITEMLIST>
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // List of strings, no tags, with validator list
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){{DESINIT(.name =) "foo",
                                DESINIT(.description =) "testing a list of strings with no tags and with "
                                                        "a validator list",
                                DESINIT(.tags =){},
                                DESINIT(.value =) tdl::StringValueList{{"hallo", "b", "c"},
                                                                       {{"a", "b", "c", "uiae", "dtrn", "hallo"}}}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEMLIST name="foo" type="string" description="testing a list of strings with no tags and with a validator list" required="false" advanced="false" restrictions="a,b,c,uiae,dtrn,hallo">
            <LISTITEM value="hallo" />
            <LISTITEM value="b" />
            <LISTITEM value="c" />
        </ITEMLIST>
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // a single string with only 'false' and 'true' as options
        auto output = convertToCTD(tdl::ToolInfo{DESINIT(.metaInfo =){},
                                                 DESINIT(.params =){{DESINIT(.name =) "foo",
                                                                     DESINIT(.description =) "testing a single bool",
                                                                     DESINIT(.tags =){},
                                                                     DESINIT(.value =) tdl::BoolValue{false}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="false" type="bool" description="testing a single bool" required="false" advanced="false" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
}

void testNodeNestedTypes() {
    { // zero nested parameters
        auto output = convertToCTD(tdl::ToolInfo{DESINIT(.metaInfo =){},
                                                 DESINIT(.params =){{DESINIT(.name =) "foo",
                                                                     DESINIT(.description =) "some major command",
                                                                     DESINIT(.tags =){},
                                                                     DESINIT(.value =) tdl::Node::Children{}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <NODE name="foo" description="some major command" />
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }

    { // a single nested parameter
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){
                {DESINIT(.name =) "foo",
                 DESINIT(.description =) "some major command",
                 DESINIT(.tags =){},
                 DESINIT(.value =) tdl::Node::Children{
                     {DESINIT(.name =) "input",
                      DESINIT(.description =) "input file",
                      DESINIT(.tags =){},
                      DESINIT(.value =)
                          tdl::IntValue{DESINIT(.value =) 1, DESINIT(.minLimit =) 0, DESINIT(.maxLimit =) 63}}}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <NODE name="foo" description="some major command">
            <ITEM name="input" value="1" type="int" description="input file" required="false" advanced="false" restrictions="0:63" />
        </NODE>
    </PARAMETERS>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // a multi nested parameter
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){
                {DESINIT(.name =) "build",
                 DESINIT(.description =) "builds some index for search",
                 DESINIT(.tags =){},
                 DESINIT(.value =) tdl::Node::Children{{DESINIT(.name =) "input",
                                                        DESINIT(.description =) "input file",
                                                        DESINIT(.tags =){},
                                                        DESINIT(.value =) tdl::StringValueList{}}}},
                {DESINIT(.name =) "search",
                 DESINIT(.description =) "reusing index to search",
                 DESINIT(.tags =){},
                 DESINIT(.value =) tdl::Node::Children{{DESINIT(.name =) "queries",
                                                        DESINIT(.description =) "files with search queries",
                                                        DESINIT(.tags =){},
                                                        DESINIT(.value =) tdl::StringValueList{}},
                                                       {DESINIT(.name =) "index",
                                                        DESINIT(.description =) "path to the index file",
                                                        DESINIT(.tags =){},
                                                        DESINIT(.value =) tdl::StringValue{}}}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <NODE name="build" description="builds some index for search">
            <ITEMLIST name="input" type="string" description="input file" required="false" advanced="false" />
        </NODE>
        <NODE name="search" description="reusing index to search">
            <ITEMLIST name="queries" type="string" description="files with search queries" required="false" advanced="false" />
            <ITEM name="index" value="" type="string" description="path to the index file" required="false" advanced="false" />
        </NODE>
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }
}

void testNodeTags() {
    { // Single Int with special tag "required"
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){{DESINIT(.name =) "foo",
                                DESINIT(.description =) "testing a single int with special tag \"required\"",
                                DESINIT(.tags =){"required"},
                                DESINIT(.value =) tdl::IntValue{5}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="5" type="int" description="testing a single int with special tag &quot;required&quot;" required="true" advanced="false" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Single Int with special tag "advanced"
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){{DESINIT(.name =) "foo",
                                DESINIT(.description =) "testing a single int with special tag \"advanced\"",
                                DESINIT(.tags =){"advanced"},
                                DESINIT(.value =) tdl::IntValue{5}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="5" type="int" description="testing a single int with special tag &quot;advanced&quot;" required="false" advanced="true" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Single Int with some random tags: "fast" and "easy"
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){{DESINIT(.name =) "foo",
                                DESINIT(.description =) "testing a single int with normal tags \"fast\" and \"easy\"",
                                DESINIT(.tags =){"fast", "easy"},
                                DESINIT(.value =) tdl::IntValue{5}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="5" type="int" description="testing a single int with normal tags &quot;fast&quot; and &quot;easy&quot;" required="false" advanced="false" tags="easy,fast" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }

    { // Single String with special tag "input file"
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){{DESINIT(.name =) "foo",
                                DESINIT(.description =) "testing a single string with special tag \"input file\"",
                                DESINIT(.tags =){"input file"},
                                DESINIT(.value =) tdl::StringValue{"input"}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="input" type="input-file" description="testing a single string with special tag &quot;input file&quot;" required="false" advanced="false" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Single String with special tag "output file"
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){{DESINIT(.name =) "foo",
                                DESINIT(.description =) "testing a single string with special tag \"output file\"",
                                DESINIT(.tags =){"output file"},
                                DESINIT(.value =) tdl::StringValue{"output"}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="output" type="output-file" description="testing a single string with special tag &quot;output file&quot;" required="false" advanced="false" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }

    { // Single String with special tag "output prefix"
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){{DESINIT(.name =) "foo",
                                DESINIT(.description =) "testing a single string with special tag \"output prefix\"",
                                DESINIT(.tags =){"output prefix"},
                                DESINIT(.value =) tdl::StringValue{"output-path"}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEM name="foo" value="output-path" type="output-prefix" description="testing a single string with special tag &quot;output prefix&quot;" required="false" advanced="false" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }

    { // List of strings with special tag "input file"
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){{DESINIT(.name =) "foo",
                                DESINIT(.description =) "testing a list of strings with special tag \"input file\"",
                                DESINIT(.tags =){"input file"},
                                DESINIT(.value =) tdl::StringValueList{}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEMLIST name="foo" type="input-file" description="testing a list of strings with special tag &quot;input file&quot;" required="false" advanced="false" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }
    { // Single String with special tag "output file"
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){{DESINIT(.name =) "foo",
                                DESINIT(.description =) "testing a list of strings with special tag \"output file\"",
                                DESINIT(.tags =){"output file"},
                                DESINIT(.value =) tdl::StringValueList{}}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <ITEMLIST name="foo" type="output-file" description="testing a list of strings with special tag &quot;output file&quot;" required="false" advanced="false" />
    </PARAMETERS>
</tool>
)"};

        assert(output.size() == expected.size());
        assert(expected == output);
    }
}

void testCliMapping() {
    { // Single double, no tags, with limits
        auto output = convertToCTD(tdl::ToolInfo{
            DESINIT(.metaInfo =){},
            DESINIT(.params =){},
            DESINIT(.cliMapping =){{DESINIT(.optionIdentifier =) "--blub", DESINIT(.referenceName =) "blubRef"}}});

        auto expected = std::string{R"(<?xml version="1.0" encoding="UTF-8"?>
<tool ctdVersion="1.7">
    <citations />
    <PARAMETERS version="1.7.0" xsi:noNamespaceSchemaLocation="https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS/SCHEMAS/Param_1_7_0.xsd" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" />
    <cli>
        <clielement optionIdentifier="--blub">
            <mapping referenceName="blubRef" />
        </clielement>
    </cli>
</tool>
)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
}

void testCTD() {
    testToolInfo();
    testNodeSingleInt();
    testNodeSingleTypes();
    testNodeListTypes();
    testNodeNestedTypes();
    testNodeTags();
    testCliMapping();
}
