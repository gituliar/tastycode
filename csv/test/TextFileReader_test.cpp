#include <gtest/gtest.h>

#include "TextFileReader.h"

using namespace tasty;


TEST(TextFileReader, empty)
{
    TextFileReader txt;

    ASSERT_EQ(txt.open("data/empty.txt"), "");

    ASSERT_EQ(txt.read(), "");
    ASSERT_EQ(txt.buf().size(), 0);
    ASSERT_EQ(txt.eof(), true);

    ASSERT_EQ(txt.close(), "");
}


TEST(TextFileReader, read)
{
    TextFileReader txt;

    ASSERT_EQ(txt.open("data/stocks_20230304.txt", 256), "");

    ASSERT_EQ(txt.read(), "");
    ASSERT_EQ(txt.buf().size(), 256);
    ASSERT_EQ(txt.eof(), false);

    ASSERT_EQ(txt.read(), "");
    ASSERT_EQ(txt.buf().size(), 124);
    ASSERT_EQ(txt.eof(), true);

    ASSERT_EQ(txt.close(), "");
}


TEST(TextFileReader, readline)
{
    TextFileReader txt;

    ASSERT_EQ(txt.open("data/stocks_20230304.txt", 256), "");

    Error err;
    ASSERT_EQ(txt.readline(err), true); ASSERT_EQ(err, "");
    ASSERT_EQ(txt.line(), "timestamp,ticker,price");

    ASSERT_EQ(txt.readline(err), true); ASSERT_EQ(err, "");
    ASSERT_EQ(txt.line(), "202303040931,A,12.34");

    ASSERT_EQ(txt.close(), "");


    ASSERT_EQ(txt.open("data/stocks_20230304.txt", 256), "");

    u32 i = 0;
    for (; txt.readline(err); i++) {
        ASSERT_EQ(err, "");
    }
    ASSERT_EQ(i, 18);

}
