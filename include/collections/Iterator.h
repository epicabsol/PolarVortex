#pragma once

template <typename T>
class Iterator {
public:
    virtual ~Iterator() { }
    
    virtual bool HasNext() const = 0;
    virtual T& Next() = 0;
    virtual const T& Next() const = 0;
};