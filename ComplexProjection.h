#pragma once

#include "Function.h"

#include <vector>
#include <complex>
#include <stdexcept>

class ComplexProjection {
private:

    Function f;
    Function b;

    // Values along the imaginary axis.
    // Each value represents a multiple of i.
    //
    // Example:
    // -2, -1, 0, 1, 2
    //
    // represents:
    // -2i, -i, 0, i, 2i
    std::vector<double> complexAxis;


public:

    // --------------------------------------------------
    // Constructors
    // --------------------------------------------------

    ComplexProjection() = default;

    ComplexProjection(
        const Function& forward,
        const Function& backward
    )
        : f(forward),
          b(backward)
    {
    }


    // --------------------------------------------------
    // Function setters
    // --------------------------------------------------

    void setF(const Function& forward) {
        f = forward;
    }

    void setB(const Function& backward) {
        b = backward;
    }


    // --------------------------------------------------
    // Function getters
    // --------------------------------------------------

    const Function& getF() const {
        return f;
    }

    const Function& getB() const {
        return b;
    }

    // --------------------------------------------------
    // Complex projection
    // --------------------------------------------------

    /*
        Produces:

            P(x) = f(x) + b(x)i

        where imaginaryValue determines
        how far along the imaginary axis
        the projection is.

        Example:

            f(x) = 2x
            b(x) = x

            P(2, 3) = 4 + 6i
    */
    std::complex<double> project(
        double x,
        double imaginaryValue = 1.0
    ) const {

        double realPart = f(x);

        double imaginaryPart =
            b(x) * imaginaryValue;

        return std::complex<double>(
            realPart,
            imaginaryPart
        );
    }


    // --------------------------------------------------
    // Projection function
    // --------------------------------------------------

    /*
        This represents the projection itself:

            P(x) = f(x) + b(x)i

        where i = 1.

        This is useful when treating the
        projection as a normal complex-valued
        function.
    */
    std::complex<double> project(
        double x
    ) const {

        return std::complex<double>(
            f(x),
            b(x)
        );
    }


    // --------------------------------------------------
    // Projection across the entire imaginary axis
    // --------------------------------------------------

    /*
        For a single x, generate:

            f(x) + b(x)(-Ni)
            ...
            f(x) + b(x)(-i)
            f(x)
            f(x) + b(x)i
            ...
            f(x) + b(x)(Ni)

        This produces the entire vertical
        projection associated with x.
    */
    std::vector<std::complex<double>>
    projectAcrossAxis(double x) const {

        std::vector<std::complex<double>> result;

        result.reserve(complexAxis.size());

        double realPart = f(x);
        double imaginaryBase = b(x);

        for (double i : complexAxis) {

            result.emplace_back(
                realPart,
                imaginaryBase * i
            );
        }

        return result;
    }


    // --------------------------------------------------
    // Project the entire real domain
    // across the entire imaginary axis
    // --------------------------------------------------

    /*
        Produces a 2D collection of complex
        points.

        For every x:

            P(x, i) = f(x) + b(x)i

        Therefore:

            x ∈ X
            i ∈ I

        gives:

            P : X × I → C
    */
    std::vector<std::vector<std::complex<double>>>
    projectPlane(
        double minX,
        double maxX,
        double stepX
    ) const {

        if (stepX <= 0.0) {
            throw std::invalid_argument(
                "X step must be greater than zero."
            );
        }

        if (minX > maxX) {
            throw std::invalid_argument(
                "X minimum cannot exceed maximum."
            );
        }

        std::vector<std::vector<std::complex<double>>>
            plane;

        for (
            double x = minX;
            x <= maxX;
            x += stepX
        ) {

            plane.push_back(
                projectAcrossAxis(x)
            );
        }

        return plane;
    }


    // --------------------------------------------------
    // Complex axis construction
    // --------------------------------------------------

    void populateComplexAxis(
        double min,
        double max,
        double step = 0.1
    ) {

        if (step <= 0.0) {
            throw std::invalid_argument(
                "Axis step must be greater than zero."
            );
        }

        if (min > max) {
            throw std::invalid_argument(
                "Axis minimum cannot exceed maximum."
            );
        }

        complexAxis.clear();

        for (
            double x = min;
            x <= max;
            x += step
        ) {

            complexAxis.push_back(x);
        }
    }


    // --------------------------------------------------
    // Axis access
    // --------------------------------------------------

    const std::vector<double>&
    getComplexAxis() const {
        return complexAxis;
    }

    std::size_t
    getComplexAxisSize() const {
        return complexAxis.size();
    }

    void clearComplexAxis() {
        complexAxis.clear();
    }

    void addComplexAxisPoint(
        double value
    ) {
        complexAxis.push_back(value);
    }


    // --------------------------------------------------
    // Convert axis coordinate to complex number
    // --------------------------------------------------

    std::complex<double>
    getComplexValue(
        double value
    ) const {

        return {
            0.0,
            value
        };
    }


    // --------------------------------------------------
    // Individual function evaluation
    // --------------------------------------------------

    double projectForward(
        double x
    ) const {
        return f(x);
    }

    double projectBackward(
        double x
    ) const {
        return b(x);
    }
};