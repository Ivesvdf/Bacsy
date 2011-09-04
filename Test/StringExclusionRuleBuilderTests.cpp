/*
 * Copyright (C) 2011  Ives van der Flaas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>
#include "Poco/Types.h"
#include "Bacsy/Rules/StringExclusionRuleBuilder.h"
#include "Bacsy/Common/DummyFile.h"
#include "Bacsy/Rules/SizeExclusionSubRule.h"

using namespace Bacsy::Rules;
using namespace Bacsy::Common;

TEST( StringExclusionRuleBuilderTests, TestUnaryAndPath)
{
	ExclusionRule rule = StringExclusionRuleBuilder::build("/home/ives/.vimrc");

	ASSERT_TRUE(rule.match(DummyFile("/home/ives/.vimrc")));
	ASSERT_FALSE(rule.match(DummyFile("/home/ives/.imrc")));
}
TEST( StringExclusionRuleBuilderTests, TestNAryAndNegation)
{
	ExclusionRule rule = StringExclusionRuleBuilder::build("/home/ives/.* & !/home/ives/.vimrc");

	ASSERT_FALSE(rule.match(DummyFile("/home/ives/.vimrc")));
	ASSERT_FALSE(rule.match(DummyFile("/home/ives/vimrc")));
	ASSERT_TRUE(rule.match(DummyFile("/home/ives/.imrc")));
}
	
TEST( StringExclusionRuleBuilderTests, TestSizes)
{
	ExclusionRule rule = StringExclusionRuleBuilder::build("<500B");
	DummyFile f("bla");
	f.setSize(499);
	ASSERT_TRUE(rule.match(f));
	f.setSize(500);
	ASSERT_FALSE(rule.match(f));
	f.setSize(501);
	ASSERT_FALSE(rule.match(f));

	rule = StringExclusionRuleBuilder::build(">500B");
	f.setSize(499);
	ASSERT_FALSE(rule.match(f));
	f.setSize(500);
	ASSERT_FALSE(rule.match(f));
	f.setSize(501);
	ASSERT_TRUE(rule.match(f));

	rule = StringExclusionRuleBuilder::build(">1kB");
	f.setSize(1023);
	ASSERT_FALSE(rule.match(f));
	f.setSize(1024);
	ASSERT_FALSE(rule.match(f));
	f.setSize(1025);
	ASSERT_TRUE(rule.match(f));

	rule = StringExclusionRuleBuilder::build(">14MB");
	f.setSize(14*1024*1024-1);
	ASSERT_FALSE(rule.match(f));
	f.setSize(14*1024*1024);
	ASSERT_FALSE(rule.match(f));
	f.setSize(14*1024*1024+1);
	ASSERT_TRUE(rule.match(f));

	rule = StringExclusionRuleBuilder::build(">4GB");
	Poco::UInt64 fourGig = Poco::UInt64(4*1024) * Poco::UInt64(1024*1024);
	f.setSize(fourGig-1);
	ASSERT_FALSE(rule.match(f));
	f.setSize(fourGig);
	ASSERT_FALSE(rule.match(f));
	f.setSize(fourGig+1);
	ASSERT_TRUE(rule.match(f));
}

TEST( StringExclusionRuleBuilderTests, TestDoubleNegation)
{
	ExclusionRule rule1 = StringExclusionRuleBuilder::build("!!<500B");
	ExclusionRule rule2 = StringExclusionRuleBuilder::build("<500B");
	DummyFile f("bla");
	f.setSize(499);
	ASSERT_EQ(rule1.match(f), rule2.match(f));
	f.setSize(500);
	ASSERT_EQ(rule1.match(f), rule2.match(f));
	f.setSize(501);
	ASSERT_EQ(rule1.match(f), rule2.match(f));
}

TEST( StringExclusionRuleBuilderTests, TestSelfAssigment)
{
	// Yes this doesn't really belong here... A test is still a test. I should
	// probably rename this file ExclusionTests or something...
	
	ExclusionRule rule = StringExclusionRuleBuilder::build(">14MB");

	DummyFile f("bla");
	f.setSize(14*1024*1024-1);
	ASSERT_FALSE(rule.match(f));
	f.setSize(14*1024*1024);
	ASSERT_FALSE(rule.match(f));
	f.setSize(14*1024*1024+1);

	rule = rule;

	f.setSize(14*1024*1024-1);
	ASSERT_FALSE(rule.match(f));
	f.setSize(14*1024*1024);
	ASSERT_FALSE(rule.match(f));
	f.setSize(14*1024*1024+1);
}
