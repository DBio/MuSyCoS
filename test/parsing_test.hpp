/*
* Copyright (C) 2012-2014 - Adam Streck
*/

#pragma once

#include <gtest/gtest.h>

#include "../model/model_parsers.hpp"

TEST(ParstingTest, RegexSyntax) {
	EXPECT_TRUE(regex_match("A2_a", regex(ModelParsers::spec_name_form)));
	EXPECT_FALSE(regex_match("2a", regex(ModelParsers::spec_name_form)));

	EXPECT_TRUE(regex_match("A2_a", regex(ModelParsers::ext_literal_form)));
	EXPECT_TRUE(regex_match("A{21}", regex(ModelParsers::ext_literal_form)));
	EXPECT_TRUE(regex_match("A{0,2}", regex(ModelParsers::ext_literal_form)));
	EXPECT_FALSE(regex_match("A{}", regex(ModelParsers::ext_literal_form)));

	EXPECT_TRUE(regex_match("A*B", regex(ModelParsers::clause_from)));
	EXPECT_TRUE(regex_match("2*A*B", regex(ModelParsers::clause_from)));
	EXPECT_TRUE(regex_match("A{0,2}*AB0{1}", regex(ModelParsers::clause_from)));
	EXPECT_FALSE(regex_match("A*2", regex(ModelParsers::clause_from)));
	EXPECT_FALSE(regex_match("", regex(ModelParsers::clause_from)));

	EXPECT_TRUE(regex_match("A'=A", regex(ModelParsers::line_form)));
	EXPECT_TRUE(regex_match("B_d5'=2*Ca2{1,5}+3*aP", regex(ModelParsers::line_form)));
	EXPECT_FALSE(regex_match("B=B", regex(ModelParsers::line_form)));
}