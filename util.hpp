#pragma once

#include <cctype>
#include <fstream>
#include <iterator>
#include <string>
#include <string_view>
#include <utility>
#include "filesystem.hpp"



namespace elona
{


namespace strutil
{


inline bool contains(
    std::string_view str,
    std::string_view pattern,
    std::string_view::size_type pos = 0)
{
    return str.find(pattern, pos) != std::string::npos;
}



inline std::string to_lower(std::string_view source)
{
    std::string ret;
    std::transform(
        std::begin(source),
        std::end(source),
        std::back_inserter(ret),
        [](char c) { return std::tolower(c); });
    return ret;
}



} // namespace strutil



namespace fileutil
{


// Note: the line number is 1-based.
// Note: the line does not contains a line break.
struct read_by_line
{
    struct iterator
    {
        using value_type = std::pair<size_t, std::string>;
        using difference_type = size_t;
        using pointer = const value_type*;
        using reference = const value_type&;
        using iterator_category = std::input_iterator_tag;


        iterator(std::ifstream& in, bool is_begin)
            : in(in)
        {
            if (is_begin)
            {
                ++(*this);
            }
        }


        void operator++()
        {
            if (std::getline(in, value.second))
            {
                ++value.first;
            }
            else // EOF
            {
                value.first = 0; // Becomes equal to end().
            }
        }


        reference operator*() const
        {
            return value;
        }


        pointer operator->() const
        {
            return &value;
        }


        bool operator!=(const iterator& other) const
        {
            return value.first != other.value.first;
        }


    private:
        std::ifstream& in;
        value_type value;
    };


    read_by_line(const fs::path& filepath)
        : in(filepath)
    {
    }


    iterator begin()
    {
        return {in, true};
    }


    iterator end()
    {
        return {in, false};
    }


private:
    std::ifstream in;
};


} // namespace fileutil


} // namespace elona