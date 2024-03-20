#pragma once

#include <cstring>
#include <fstream>


// ---- tasty.h ------------------------------------------------------------------------------------
// 
#include <filesystem>
#include <string>
#include <vector>

using c8 = char;
using f32 = float;
using f64 = double;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using std::string;
using std::string_view;
using std::vector;

namespace fs = std::filesystem;

using Error = std::string;
// 
// ---- tasty.h ------------------------------------------------------------------------------------


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

    //  Declare as non-copyable
    //
    TextFileReader(const TextFileReader&) = delete;
    TextFileReader& operator=(const TextFileReader& other) = delete;


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
        read(Error& err)
    {
        if (m_size = std::fread(m_buf + m_cursor, 1, m_capacity - m_cursor, m_src); !m_size) {
            if (std::ferror(m_src)) {
                err = "TextFileReader::read : Fail to read : errno = " + std::to_string(m_size);
                return false;
            }
            else if (std::feof(m_src)) {
                return false;
            }

            err = "TextFileReader::read : unknown error";
            return false;
        }

        m_size += m_cursor;
        m_cursor = 0;

        return true;
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

            //  No \n found
            //
            if (std::feof(m_src)) {
                m_line = string_view(m_buf + m_cursor, m_size - m_cursor);
                return m_cursor < m_size;
            }

            //  Copy tail to the beginning
            //
            if (m_cursor < m_size) {
                std::memcpy(m_buf, m_buf + m_cursor, m_size - m_cursor);
            }
            m_cursor = m_size - m_cursor;

            //  Read more data from file
            //
            if (!read(err)) {
                err = "TextFileReader::readline : " + err;
                return false;
            }
        } while (m_size);

        err = "TextFileReader::readline : unreachable";
        return false;
    };
};

}
