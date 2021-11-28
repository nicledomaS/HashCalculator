#pragma once

#include <boost/noncopyable.hpp>

#include <string>

namespace hash_calculator
{

class Segment : private boost::noncopyable
{
public:
    virtual ~Segment() = default;

    virtual void calcHash() = 0;
    virtual bool isReady() const = 0;
    virtual std::string getHash() const = 0;
};

} // hash_calculator