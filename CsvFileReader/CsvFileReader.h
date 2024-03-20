#pragma once

#include <cstdlib>
#include <iostream>
#include <type_traits>

#include "tasty.h"


namespace tasty
{

namespace
{
    template<u32 N>
    struct ColName {
        char buf[N + 1]{};
        constexpr ColName(char const* s) {
            for (unsigned i = 0; i != N; ++i) buf[i] = s[i];
        }
        constexpr operator char const* () const { return buf; }
    };
    template<u32 N> ColName(char const (&)[N]) -> ColName<N - 1>;


    template<class T, u32 N, class ... Ts>
    constexpr i32
        index(u32 i, const ColName<N>& column, const T& iColumn, const Ts& ... header)
    {
        return (column == iColumn) ? i : index(i + 1, column, header ...);
    }

    template<class T, u32 N>
    constexpr i32
        index(u32 i, const ColName<N>& column, const T& iColumn)
    {
        return (column == iColumn) ? i : -1;
    }
}

template<ColName ... header>
class CsvReader
{
private:
    /// read() state
    ///
    FILE*
        m_src;

    c8* m_buf;
    /// data size, when full m_size == m_buf.size()
    u64 m_size;

    /// init() state
    ///
    vector<u8>
        m_map;

    /// readline() state
    ///
    string_view
        m_line;
    vector<i32>
        m_tokens;
    u32 m_cursor;

public:
    CsvReader() :
        m_cursor{ 0 }, m_size{ 0 }, m_src{ nullptr }
    { }

    ~CsvReader()
    {
        if (auto err = close(); !err.empty())
            std::cerr << err;
    };

    Error
        open(const fs::path& srcPath)
    {
        m_cursor = 0;
        m_size = 0;

        m_src = std::fopen(srcPath.string().c_str(), "rb");
        if (!m_src)
            return "CsvReader::open : Fail to open " + srcPath.string();

        /// TODO: init m_map

        return "";
    }

    Error
        close()
    {
        if (!m_src)
            return "";

        if (auto res = std::fclose(m_src); res == EOF)
            return "CsvReader::close : Fail to close : errno = " + std::to_string(res);

        m_src = nullptr;

        return "";
    };

    bool
        readline()
    {
        do {
            /// Find next \n char
            ///
            if (const auto p = (c8*)memchr(m_buf + m_cursor, '\n', m_size - m_cursor); p) {
                /// ignore \n in m_line
                m_line = std::string_view(m_buf + m_cursor, p - (m_buf + m_cursor));
                m_cursor = p - m_buf + 1;
                return "";
            }

            /// Read last line (has no \n)
            ///
            if (std::feof(m_src)) {
                m_line = std::string_view(m_buf + m_cursor, m_size - m_cursor);
                m_cursor = m_size;
                return "";
            }

            /// Copy tail to the beginning
            ///
            if (m_cursor < m_size) {
                auto n = m_size - m_cursor;
                std::memcpy(m_buf, m_buf + m_cursor, n);
            }

            /// Consume more data
            ///
            if (auto err = read(); err != "")
                return "CsvReader::readline : " + err;
        } while (m_size);

        return "CsvReader::readline : EOF";
    }

    template<ColName column>
    i32
        get()
    {
        constexpr auto i = index(0, column, header ...);
        static_assert(i >= 0, "Unknown column");

        //return m_tokens[m_map[i]];
        return i;
    }

private:
    Error
        read()
    {
        if (m_size = std::fread(m_buf + m_cursor, 1, m_size - m_cursor, m_src); !m_size) {
            if (std::ferror(m_src))
                return "CsvReader::read : Fail to read : errno = " + std::to_string(m_size);
        }

        m_cursor = 0;

        return "";
    }
};

}
