/*
 * Copyright (c) 2014 - Adam Streck 
 * MuSyCoS - Multivalued Synchronous Networks Constraint Solver 
 * MuSyCoS is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3. 
 * MuSyCoS is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>. 
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> 
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

	EXPECT_NO_THROW(ModelParsers::control_syntax({ "A'=B", "B'=A{2,0}" }));
	EXPECT_THROW(ModelParsers::control_syntax({ "", "A'=B" }), runtime_error);
}

TEST(ParsingTest, SpecieReconstruction) {
	Specie spec1 = ModelParsers::obtainSpecie("A2b'=0*A2b{0}+2*dx");
	EXPECT_STREQ("A2b", spec1.name.c_str());
	EXPECT_EQ(2, spec1.max_val);
	EXPECT_STREQ("0*A2b{0}+2*dx", spec1.rule.c_str());
}

TEST(ParsingTest, ModelSemantics) {
	vector<Specie> species;
	species.emplace_back(ModelParsers::obtainSpecie("A'=2*B{1}+A{0,2}"));
	species.emplace_back(ModelParsers::obtainSpecie("B'=A{1}*B"));

	EXPECT_NO_THROW(ModelParsers::control_semantics(species));

	species.emplace_back(ModelParsers::obtainSpecie("C'=B{2}"));
	EXPECT_THROW(ModelParsers::control_semantics(species), runtime_error) << "Incorrect value (B can't attain 2)";

	species[2] = ModelParsers::obtainSpecie("C'=a{1}");
	EXPECT_THROW(ModelParsers::control_semantics(species), runtime_error) << "'a' is not a known specie";
}