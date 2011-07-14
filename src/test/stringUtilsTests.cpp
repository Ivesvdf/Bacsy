#include <gtest/gtest.h>
#include "stringUtils.h"

TEST( StringUtilsTest, SimpleStripTest )
{
	ASSERT_EQ("x", StringUtils::rstrip("xyyy", "y"));
	ASSERT_EQ("xyyy", StringUtils::lstrip("xyyy", "y"));
	ASSERT_EQ("x", StringUtils::strip("xyyy", "y"));
	ASSERT_EQ("bcde", StringUtils::strip("abcdea", "a"));
}

TEST( StringUtilsTest, MultipleStripDropTest ) 
{
	ASSERT_EQ("cdax", StringUtils::strip("abcdaxba", "ab"));
	ASSERT_EQ("cdax", StringUtils::strip("abcdaxba", "ba"));
}

TEST( StringUtilsTest, DefaultStripTest )
{
	ASSERT_EQ("text ", StringUtils::lstrip("  text "));
	ASSERT_EQ("  text", StringUtils::rstrip("  text "));
	ASSERT_EQ("text", StringUtils::strip("text"));
}

TEST( StringUtilsTest, SplitTest )
{
	typedef std::vector<std::string> VS;

	VS a = StringUtils::split("a", ',');
	ASSERT_EQ("a", a[0]); 
	ASSERT_EQ(1u, a.size());

	VS b = StringUtils::split("a,b", ',');
	ASSERT_EQ("a", b[0]); 
	ASSERT_EQ("b", b[1]); 
	ASSERT_EQ(2u, b.size());

	VS c = StringUtils::split("a,b", ' ');
	ASSERT_EQ("a,b", c[0]); 
	ASSERT_EQ(1u, c.size());

	VS d = StringUtils::split("Is this real life?", ' ');
	ASSERT_EQ("Is", d[0]); 
	ASSERT_EQ("this", d[1]); 
	ASSERT_EQ("real", d[2]); 
	ASSERT_EQ("life?", d[3]); 
	ASSERT_EQ(4u, d.size());
}
