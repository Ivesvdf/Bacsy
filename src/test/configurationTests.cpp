#include <sstream>
#include <gtest/gtest.h>
#include "configurationFile.h"

TEST( ConfigurationTest, NoExceptionsWhenValid )
{
	std::stringstream ss;

	ss << "[someSection]\na=1\nb=10\n[anotherSection]\n[third]\na=5";
	ConfigurationFile file(ss);
}

TEST( ConfigurationTest, NoStartingSectionTest )
{
	std::stringstream ss;

	ss << "a=1\nb=10";
	ASSERT_THROW(ConfigurationFile file(ss), SyntaxError);
}

TEST( ConfigurationTest, TwoSameNameSections )
{
	std::stringstream ss;

	ss << "[someSection]\na=1\nb=10\n[someSection]";
	ASSERT_THROW(ConfigurationFile file(ss), SyntaxError);
}

TEST( ConfigurationTest, NoEquals )
{
	std::stringstream ss;

	ss << "[someSection]\na\nb=10\n";
	ASSERT_THROW(ConfigurationFile file(ss), SyntaxError);
}

TEST( ConfigurationTest, NoEmptyKeys )
{
	std::stringstream ss;

	ss << "[someSection]\n=a\nb=10\n";
	ASSERT_THROW(ConfigurationFile file(ss), SyntaxError);
}

TEST( ConfigurationTest, NoEmptyVals )
{
	std::stringstream ss;

	ss << "[someSection]\na=\nb=10\n";
	ASSERT_THROW(ConfigurationFile file(ss), SyntaxError);
}

TEST( ConfigurationTest, SimpleExtraction )
{
	std::stringstream ss;

	ss << "[someSection]\na=C:/bla.txt\nb=10\n";
	ConfigurationFile file(ss);

	ASSERT_EQ("C:/bla.txt", file.get<std::string>("someSection", "a"));
	ASSERT_EQ("10", file.get<std::string>("someSection", "b"));
	ASSERT_EQ(10, file.get<int>("someSection", "b"));
	ASSERT_THROW(file.get<int>("someSection", "c"), NoSuchKeyException);
	ASSERT_EQ(99, file.get<int>("someSection", "c", 99));
}

TEST( ConfigurationTest, TestArraySyntax )
{
	std::stringstream ss;

	ss << "[someSection]\na[]=C:/bla.txt\na[]=D:/koekoek.bat";
	ConfigurationFile file(ss);

	ASSERT_EQ("C:/bla.txt\nD:/koekoek.bat", file.get<std::string>("someSection", "a"));
}


TEST( ConfigurationTest, TestSections )
{
	std::stringstream ss;

	ss << "[someSection]\na=1\nb=10\n[anotherSection]\n[third]\na=5";
	ConfigurationFile file(ss);

	std::list<std::string> sections = file.sections();
	sections.sort();
	std::list<std::string>::iterator iterator = sections.begin();
	
	ASSERT_EQ("anotherSection", *iterator);
	iterator++;
	ASSERT_EQ("someSection", *iterator);
	iterator++;
	ASSERT_EQ("third", *iterator);
	iterator++;
	ASSERT_TRUE(sections.end() == iterator); 
}


TEST( ConfigurationTest, TestComments )
{
	std::stringstream ss;

	ss << "[someSection]\n; some comments\n# more comments \na=1\nb=10\n";
	ConfigurationFile file(ss);

	ASSERT_EQ("10", file.get<std::string>("someSection", "b"));
	ASSERT_EQ("1", file.get<std::string>("someSection", "a"));
}

TEST( ConfigurationTest, TestWhitespaceInsensitive )
{
	std::stringstream ss;

	ss << "[someSection]\n	a=1\nb	=10\nc= 11\nd = 12";
	ConfigurationFile file(ss);

	ASSERT_EQ("10", file.get<std::string>("someSection", "b"));
	ASSERT_EQ("1", file.get<std::string>("someSection", "a"));
	ASSERT_EQ("11", file.get<std::string>("someSection", "c"));
	ASSERT_EQ("12", file.get<std::string>("someSection", "d"));

	ss << "[ someSection ]\na=1\nb=10\nc=11\nd=12";
	std::list<std::string> sections = file.sections();
	std::list<std::string>::iterator iterator = sections.begin();
	ASSERT_EQ("someSection", *iterator);
	ASSERT_EQ(1u, sections.size());
}
