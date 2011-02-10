/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2011 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "../path.h"

TEST(PathNormalise, Date){
    EXPECT_EQ(Path::Normalise("@date@"), wxDateTime::Now().FormatISODate());
}

TEST(PathNormalise, Time){
    EXPECT_EQ(Path::Normalise("@time@"), wxDateTime::Now().Format("%H") + "-" +  wxDateTime::Now().Format("%M"));
}

TEST(PathNormalise, Year){
    EXPECT_EQ(Path::Normalise("@year@"), wxDateTime::Now().Format("%Y"));
    EXPECT_EQ(Path::Normalise("@YYYY@"), wxDateTime::Now().Format("%Y"));
}

TEST(PathNormalise, Month){
    EXPECT_EQ(Path::Normalise("@month@"), wxDateTime::Now().Format("%m"));
    EXPECT_EQ(Path::Normalise("@MM@"), wxDateTime::Now().Format("%m"));
}

TEST(PathNormalise, Day){
    EXPECT_EQ(Path::Normalise("@day@"), wxDateTime::Now().Format("%d"));
    EXPECT_EQ(Path::Normalise("@DD@"), wxDateTime::Now().Format("%d"));
}

TEST(PathNormalise, Hour){
    EXPECT_EQ(Path::Normalise("@hour@"), wxDateTime::Now().Format("%H"));
    EXPECT_EQ(Path::Normalise("@hh@"), wxDateTime::Now().Format("%H"));
}

TEST(PathNormalise, Minute){
    EXPECT_EQ(Path::Normalise("@minute@"), wxDateTime::Now().Format("%M"));
    EXPECT_EQ(Path::Normalise("@mm@"), wxDateTime::Now().Format("%M"));
}

TEST(PathNormalise, MismatchedAt){
    EXPECT_EQ(Path::Normalise("C:\\thisisa@test\\path.doc"), "C:\\thisisa@test\\path.doc");
    EXPECT_EQ(Path::Normalise("C:\\testdir\\@@date@.zip"), "C:\\testdir\\@" + wxDateTime::Now().FormatISODate() + ".zip");
    EXPECT_EQ(Path::Normalise("C:\\testdir\\@@date@@.zip"), "C:\\testdir\\@" + wxDateTime::Now().FormatISODate() + "@.zip");
}

TEST(PathNormalise, DateEmbedded){
    EXPECT_EQ(Path::Normalise("C:\\testdir\\@date@.zip"),"C:\\testdir\\" + wxDateTime::Now().FormatISODate() + ".zip");
}

TEST(PathNormalise, BunchedEmbeddded){
    EXPECT_EQ(Path::Normalise("C:\\@YYYY@@MM@\\@date@.zip"), "C:\\" + wxDateTime::Now().Format("%Y") + wxDateTime::Now().Format("%m") + "\\" + wxDateTime::Now().FormatISODate() + ".zip");
}

TEST(PathNormalise, Plain){
    EXPECT_EQ(Path::Normalise("docs"), "docs");
}

TEST(PathNormalise, DoesntExist){
    EXPECT_EQ(Path::Normalise("C:\\thisisa@test@\\path.doc"), "C:\\thisisa@test@\\path.doc");
}
