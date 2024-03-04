#include <gtest/gtest.h>

#include "CsvFileReader.h"

using namespace tasty;


TEST(csv, CsvFileReader)
{
    CsvReader<"xxx", "yy", "1"> csv;

    ASSERT_EQ(csv.get<"xxx">(), 0);
    ASSERT_EQ(csv.get<"yy">(), 1);
    ASSERT_EQ(csv.get<"1">(), 2);
}
