/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2011 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "../rules.h"

TEST(Rules, Constructor){
    RuleSet rules("test");
    EXPECT_TRUE(rules.IsValid());

    RuleSet blankname("");
    EXPECT_TRUE(blankname.IsValid());
}

TEST(Rules, GetName){
    RuleSet rules("test");
    EXPECT_EQ(rules.GetName(), "test");

    RuleSet blankname("");
    EXPECT_EQ(blankname.GetName(), "");
}
