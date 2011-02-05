/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2011 Steven Lamerton
// License:     GNU GPL 2 http://www.gnu.org/licenses/gpl-2.0.html
/////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "../path.h"

TEST(PathNormalise, Date){
    EXPECT_EQ(Path::Normalise(wxT("@date@")), wxDateTime::Now().FormatISODate());
}

TEST(PathNormalise, Time){
    EXPECT_EQ(Path::Normalise(wxT("@time@")), wxDateTime::Now().Format(wxT("%H")) + wxT("-") +  wxDateTime::Now().Format(wxT("%M")));
}

TEST(PathNormalise, Year){
    EXPECT_EQ(Path::Normalise(wxT("@YYYY@")), wxDateTime::Now().Format(wxT("%Y")));
}

TEST(PathNormalise, Month){
    EXPECT_EQ(Path::Normalise(wxT("@MM@")), wxDateTime::Now().Format(wxT("%m")));
}

TEST(PathNormalise, Day){
    EXPECT_EQ(Path::Normalise(wxT("@DD@")), wxDateTime::Now().Format(wxT("%d")));
}

TEST(PathNormalise, Hour){
    EXPECT_EQ(Path::Normalise(wxT("@hh@")), wxDateTime::Now().Format(wxT("%H")));
}

TEST(PathNormalise, Minute){
    EXPECT_EQ(Path::Normalise(wxT("@mm@")), wxDateTime::Now().Format(wxT("%M")));
}

TEST(PathNormalise, EmbeddedAt){
    EXPECT_EQ(Path::Normalise(wxT("C:\\thisisa@test\\path.doc")), wxT("C:\\thisisa@test\\path.doc"));
}

TEST(PathNormalise, DateEmbedded){
    EXPECT_EQ(Path::Normalise(wxT("C:\\testdir\\@date@.zip")), wxT("C:\\testdir\\") + wxDateTime::Now().FormatISODate() + wxT(".zip"));
}

TEST(PathNormalise, BunchedEmbeddded){
    EXPECT_EQ(Path::Normalise(wxT("C:\\@YYYY@@MM@\\@date@.zip")), wxT("C:\\") + wxDateTime::Now().Format(wxT("%Y")) + wxDateTime::Now().Format(wxT("%m")) + wxT("\\") + wxDateTime::Now().FormatISODate()+ wxT(".zip"));
}

TEST(PathNormalise, ExtraAt){
    EXPECT_EQ(Path::Normalise(wxT("C:\\testdir\\@@date@.zip")), wxT("C:\\testdir\\@") + wxDateTime::Now().FormatISODate() + wxT(".zip"));
    EXPECT_EQ(Path::Normalise(wxT("C:\\testdir\\@@date@@.zip")), wxT("C:\\testdir\\@") + wxDateTime::Now().FormatISODate() + wxT("@.zip"));
}

TEST(PathNormalise, Plain){
    EXPECT_EQ(Path::Normalise(wxT("docs")), wxT("docs"));
}
