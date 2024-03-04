#pragma once

#include <fstream>

#include "tasty.h"


namespace tasty
{

class TextFileReader
{
private:
    FILE*
        m_src;

    c8* m_buf;
    u64 m_capacity;
    u64 m_size;

    u64 m_cursor;

    string_view
        m_line;

public:
    TextFileReader() :
        m_buf{ nullptr }, m_capacity{ 0 }, m_cursor{ 0 }, m_size{ 0 }, m_src{ nullptr }
    {};

    ~TextFileReader()
    {
        if (m_buf)
            std::free(m_buf);

        if (auto err = close(); !err.empty())
            std::cerr << err << std::endl;
    };


    string_view 
        buf() const { return string_view(m_buf, m_size); }

    string_view
        line() const { return m_line; }


    Error
        open(const fs::path& srcPath, u64 capacity = 64 * 1024)
    {
        m_cursor = m_size = 0;

        if (m_capacity != capacity) {
            m_capacity = capacity;

            if (auto err = close(); !err.empty())
                return "TextFileReader::open : " + err;

            if (m_buf)
                m_buf = (c8*)std::realloc(m_buf, m_capacity);
            else
                m_buf = (c8*)std::malloc(m_capacity);
            if (!m_buf)
                return "TextFileReader::open : Fail to malloc " + std::to_string(m_capacity) + " bytes";
        }

        m_src = std::fopen(srcPath.string().c_str(), "rb");
        if (!m_src)
            return "TextFileReader::open : Fail to open " + fs::absolute(srcPath).string();

        return "";
    };


    Error
        close()
    {
        if (m_src) {
            if (auto res = std::fclose(m_src); res == EOF)
                return "TextFileReader::close : Fail to close : errno = " + std::to_string(res);
            m_src = nullptr;
        }

        return "";
    };


    bool
        eof() const
    {
        return std::feof(m_src);
    };


    Error
        read(u64 pos = 0)
    {
        if (m_size = std::fread(m_buf + pos, 1, m_capacity - pos, m_src); !m_size) {
            if (std::ferror(m_src))
                return "TextFileReader::read : Fail to read : errno = " + std::to_string(m_size);
        }

        m_size += pos;
        m_cursor = 0;

        return "";
    };


    bool
        readline(Error& err)
    {
        do {
            if (const auto p = (c8*)memchr(m_buf + m_cursor, '\n', m_size - m_cursor); p) {
                m_line = string_view(m_buf + m_cursor, p - (m_buf + m_cursor));
                m_cursor = p - m_buf + 1;
                return true;
            }

            /// No \n found
            ///
            if (eof()) {
                m_line = string_view(m_buf + m_cursor, m_size - m_cursor);
                return m_cursor < m_size;
            }

            /// Copy tail to the beginning
            ///
            if (m_cursor < m_size) {
                std::memcpy(m_buf, m_buf + m_cursor, m_size - m_cursor);
            }

            /// Consume more data (do not update m_cursor)
            ///
            if (err = read(m_size - m_cursor); !err.empty()) {
                err = "TextFileReader::readline : " + err;
                return false;
            }

            m_cursor = 0;
        } while (m_size);

        err = "TextFileReader::readline : EOF";
        return false;
    };
};

}
