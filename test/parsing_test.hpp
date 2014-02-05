/*
* Copyright (C) 2012-2014 - Adam Streck
*/

#pragma once

#include <gtest/gtest.h>

#include "../model/model_parsers.hpp"

TEST(ParstingTest, RegexSyntax) {
	EXPECT_TRUE(regex_match("A2_a", regex(ModelParsers::spec_name)));
	EXPECT_FALSE(regex_match("2a", regex(ModelParsers::spec_name)));

	EXPECT_TRUE(regex_match("A2_a", regex(ModelParsers::ext_literal)));
	EXPECT_TRUE(regex_match("A{21}", regex(ModelParsers::ext_literal)));
	EXPECT_TRUE(regex_match("A{0,2}", regex(ModelParsers::ext_literal)));
	EXPECT_FALSE(regex_match("A{}", regex(ModelParsers::ext_literal)));

	EXPECT_TRUE(regex_match("A*B", regex(ModelParsers::clause)));
	EXPECT_TRUE(regex_match("2*A*B", regex(ModelParsers::clause)));
	EXPECT_TRUE(regex_match("A{0,2}*AB0{1}", regex(ModelParsers::clause)));
	EXPECT_FALSE(regex_match("A*2", regex(ModelParsers::clause)));
}