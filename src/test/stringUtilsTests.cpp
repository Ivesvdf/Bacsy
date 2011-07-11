#include <gtest/gtest.h>
#include "stringUtils.h"

TEST( StringUtilsTest, SimpleTest )
{
	ASSERT_EQ("x", StringUtils::rstrip("xyyy", "y"));
	ASSERT_EQ("xyyy", StringUtils::lstrip("xyyy", "y"));
	ASSERT_EQ("x", StringUtils::strip("xyyy", "y"));
	ASSERT_EQ("bcde", StringUtils::strip("abcdea", "a"));
}

TEST( StringUtilsTest, MultipleDropTest ) 
{
	ASSERT_EQ("cdax", StringUtils::strip("abcdaxba", "ab"));
	ASSERT_EQ("cdax", StringUtils::strip("abcdaxba", "ba"));
}

TEST( StringUtilsTest, DefaultTest )
{
	ASSERT_EQ("text ", StringUtils::lstrip("  text "));
	ASSERT_EQ("  text", StringUtils::rstrip("  text "));
	ASSERT_EQ("text", StringUtils::strip("text"));
}

