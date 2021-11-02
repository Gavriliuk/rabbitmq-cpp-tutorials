#pragma once

#include <string>
#include <sstream>
#include <iterator>

#include "kashmir/uuid.h"
#include "kashmir/devrand.h"

template<typename Iterator>
std::string join(Iterator first, const Iterator last, const std::string& separator)
{
    std::stringstream result;
    std::copy(first, last - 1, std::ostream_iterator<std::string>(result, separator.c_str()));
    std::copy(last - 1, last, std::ostream_iterator<std::string>(result));
    return result.str();
}

std::string uuid()
{
    kashmir::system::DevRand devrand;
    kashmir::uuid_t uuid;
    devrand >> uuid;

    std::stringstream result;
    result << uuid;
    return result.str();
}
