#pragma once

#include <functional>
#include <string>
#include <cmath>

class Function {
private:
    std::function<double(double)> function;
    std::string name;

public:
    Function() = default;

    Function(
        std::function<double(double)> f,
        const std::string& functionName = "f"
    )
        : function(f), name(functionName)
    {
    }

    double operator()(double x) const {
        return function(x);
    }

    const std::string& getName() const {
        return name;
    }

    bool valid() const {
        return static_cast<bool>(function);
    }
};