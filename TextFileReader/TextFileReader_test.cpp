#include <gtest/gtest.h>

#include "TextFileReader.h"

using namespace tasty;


TEST(TextFileReader, empty)
{
    TextFileReader txt;

    ASSERT_EQ(txt.open("test/empty.txt"), "");

    {
        Error err;
        ASSERT_EQ(txt.read(err), false);
        ASSERT_EQ(err, "");
        ASSERT_EQ(txt.buf().size(), 0);
    }

    ASSERT_EQ(txt.close(), "");
}


TEST(TextFileReader, read)
{
    TextFileReader txt;

    ASSERT_EQ(txt.open("test/prices_20230304.txt", 256), "");

    {
        Error err;
        ASSERT_EQ(txt.read(err), true);
        ASSERT_EQ(err, "");
        ASSERT_EQ(txt.buf().size(), 256);

        ASSERT_EQ(txt.read(err), true);
        ASSERT_EQ(err, "");
        ASSERT_EQ(txt.buf().size(), 124);

        ASSERT_EQ(txt.read(err), false);
        ASSERT_EQ(err, "");
    }

    ASSERT_EQ(txt.close(), "");
}


TEST(TextFileReader, readline)
{
    TextFileReader txt;

    ASSERT_EQ(txt.open("test/prices_20230304.txt", 256), "");

    Error err;
    ASSERT_EQ(txt.readline(err), true); ASSERT_EQ(err, "");
    ASSERT_EQ(txt.line(), "timestamp,ticker,price");

    ASSERT_EQ(txt.readline(err), true); ASSERT_EQ(err, "");
    ASSERT_EQ(txt.line(), "202303040931,A,12.34");

    ASSERT_EQ(txt.close(), "");


    ASSERT_EQ(txt.open("test/prices_20230304.txt", 256), "");

    u32 i = 0;
    while (txt.readline(err)) {
        ASSERT_EQ(err, "");
        i++;
    }
    ASSERT_EQ(i, 18);

    ASSERT_EQ(txt.readline(err), false);
    ASSERT_EQ(err, "");
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
