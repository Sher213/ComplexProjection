#include "Function.h"
#include "ComplexProjection.h"

#include "imgui.h"
#include "implot.h"

#include <GLFW/glfw3.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <cmath>
#include <cctype>
#include <cstdio>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>


// ============================================================
// Mathematical Expression Parser
// ============================================================

class ExpressionParser
{
private:

    std::string expression;
    std::size_t position = 0;


    void skipWhitespace()
    {
        while (
            position < expression.size() &&
            std::isspace(
                static_cast<unsigned char>(
                    expression[position]
                )
            )
        )
        {
            ++position;
        }
    }


    bool match(char c)
    {
        skipWhitespace();

        if (
            position < expression.size() &&
            expression[position] == c
        )
        {
            ++position;
            return true;
        }

        return false;
    }


    std::string parseIdentifier()
    {
        skipWhitespace();

        std::size_t start = position;

        while (
            position < expression.size() &&
            (
                std::isalpha(
                    static_cast<unsigned char>(
                        expression[position]
                    )
                )
                ||
                std::isdigit(
                    static_cast<unsigned char>(
                        expression[position]
                    )
                )
                ||
                expression[position] == '_'
            )
        )
        {
            ++position;
        }

        return expression.substr(
            start,
            position - start
        );
    }


    double parseNumber()
    {
        skipWhitespace();

        std::size_t start = position;

        while (
            position < expression.size() &&
            (
                std::isdigit(
                    static_cast<unsigned char>(
                        expression[position]
                    )
                )
                ||
                expression[position] == '.'
                ||
                expression[position] == 'e'
                ||
                expression[position] == 'E'
            )
        )
        {
            ++position;
        }

        if (start == position)
        {
            throw std::runtime_error(
                "Expected number."
            );
        }

        return std::stod(
            expression.substr(
                start,
                position - start
            )
        );
    }


    double parsePrimary(double x)
    {
        skipWhitespace();


        // Parentheses
        if (match('('))
        {
            double result =
                parseExpression(x);

            if (!match(')'))
            {
                throw std::runtime_error(
                    "Missing closing ')'."
                );
            }

            return result;
        }


        // Number
        if (
            position < expression.size() &&
            (
                std::isdigit(
                    static_cast<unsigned char>(
                        expression[position]
                    )
                )
                ||
                expression[position] == '.'
            )
        )
        {
            return parseNumber();
        }


        // Identifier
        if (
            position < expression.size() &&
            std::isalpha(
                static_cast<unsigned char>(
                    expression[position]
                )
            )
        )
        {
            std::string name =
                parseIdentifier();


            // Variable
            if (name == "x")
                return x;


            // Constants
            if (name == "pi")
                return 3.14159265358979323846;


            if (name == "e")
                return std::exp(1.0);


            // Function
            if (!match('('))
            {
                throw std::runtime_error(
                    "Expected '(' after " + name
                );
            }


            double argument =
                parseExpression(x);


            if (!match(')'))
            {
                throw std::runtime_error(
                    "Missing ')' after " + name
                );
            }


            if (name == "sin")
                return std::sin(argument);

            if (name == "cos")
                return std::cos(argument);

            if (name == "tan")
                return std::tan(argument);

            if (name == "asin")
                return std::asin(argument);

            if (name == "acos")
                return std::acos(argument);

            if (name == "atan")
                return std::atan(argument);

            if (name == "exp")
                return std::exp(argument);

            if (name == "log")
                return std::log(argument);

            if (name == "sqrt")
                return std::sqrt(argument);

            if (name == "abs")
                return std::abs(argument);


            throw std::runtime_error(
                "Unknown function: " + name
            );
        }


        throw std::runtime_error(
            "Unexpected character."
        );
    }


    double parseUnary(double x)
    {
        skipWhitespace();


        if (match('+'))
            return parseUnary(x);


        if (match('-'))
            return -parseUnary(x);


        return parsePower(x);
    }


    double parsePower(double x)
    {
        double value =
            parsePrimary(x);


        if (match('^'))
        {
            double exponent =
                parseUnary(x);

            return std::pow(
                value,
                exponent
            );
        }


        return value;
    }


    double parseTerm(double x)
    {
        double value =
            parseUnary(x);


        while (true)
        {
            if (match('*'))
            {
                value *=
                    parseUnary(x);
            }
            else if (match('/'))
            {
                value /=
                    parseUnary(x);
            }
            else
            {
                break;
            }
        }


        return value;
    }


    double parseExpression(double x)
    {
        double value =
            parseTerm(x);


        while (true)
        {
            if (match('+'))
            {
                value +=
                    parseTerm(x);
            }
            else if (match('-'))
            {
                value -=
                    parseTerm(x);
            }
            else
            {
                break;
            }
        }


        return value;
    }


public:

    explicit ExpressionParser(
        const std::string& expression
    )
        : expression(expression)
    {
    }


    double evaluate(double x)
    {
        position = 0;

        double result =
            parseExpression(x);

        skipWhitespace();


        if (position != expression.size())
        {
            throw std::runtime_error(
                "Unexpected characters in expression."
            );
        }


        return result;
    }
};


// ============================================================
// Convert text expression into Function
// ============================================================

Function makeFunction(
    const std::string& expression,
    const std::string& name
)
{
    // Validate expression first.
    ExpressionParser parser(
        expression
    );

    parser.evaluate(0.0);


    // Capture expression.
    auto callable =
        [expression](double x)
        {
            ExpressionParser parser(
                expression
            );

            return parser.evaluate(x);
        };


    return Function(
        callable,
        name
    );
}


// ============================================================
// Main
// ============================================================

int main()
{
    // ========================================================
    // Initial functions
    // ========================================================

    Function f(
        [](double x)
        {
            return std::sin(x);
        },
        "a(x) = sin(x)"
    );


    Function b(
        [](double x)
        {
            return std::cos(x);
        },
        "b(x) = cos(x)"
    );


    // ========================================================
    // Complex projection
    // ========================================================

    ComplexProjection projection(
        f,
        b
    );


    projection.populateComplexAxis(
        -10.0,
        10.0,
        0.01
    );


    // ========================================================
    // Graph data
    // ========================================================

    std::vector<double> fValues;
    std::vector<double> bValues;

    std::vector<double> projectionReal;
    std::vector<double> projectionImaginary;


    auto rebuildData =
        [&]()
        {
            const auto& axis =
                projection.getComplexAxis();


            fValues.clear();
            bValues.clear();

            projectionReal.clear();
            projectionImaginary.clear();


            fValues.reserve(
                axis.size()
            );

            bValues.reserve(
                axis.size()
            );

            projectionReal.reserve(
                axis.size()
            );

            projectionImaginary.reserve(
                axis.size()
            );


            for (double x : axis)
            {
                double forward =
                    projection.projectForward(x);

                double backward =
                    projection.projectBackward(x);


                fValues.push_back(
                    forward
                );

                bValues.push_back(
                    backward
                );


                // P(x) = a(x) + b(x)i

                projectionReal.push_back(
                    forward
                );

                projectionImaginary.push_back(
                    backward * x
                );
            }
        };


    rebuildData();


    // ========================================================
    // GUI input buffers
    // ========================================================

    char fBuffer[512] =
        "sin(x)";

    char bBuffer[512] =
        "cos(x)";


    char errorBuffer[512] =
        "";


    // ========================================================
    // GLFW
    // ========================================================

    if (!glfwInit())
        return -1;


    const char* glslVersion =
        "#version 130";


    GLFWwindow* window =
        glfwCreateWindow(
            1400,
            900,
            "Functional Space",
            nullptr,
            nullptr
        );


    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(
        window
    );

    glfwSwapInterval(1);


    // ========================================================
    // ImGui
    // ========================================================

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImPlot::CreateContext();


    ImGuiIO& io =
        ImGui::GetIO();


    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;


    ImGui::StyleColorsDark();


    ImGui_ImplGlfw_InitForOpenGL(
        window,
        true
    );


    ImGui_ImplOpenGL3_Init(
        glslVersion
    );


    // ========================================================
    // Main loop
    // ========================================================

    while (
        !glfwWindowShouldClose(window)
    )
    {
        glfwPollEvents();


        ImGui_ImplOpenGL3_NewFrame();

        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();


        // ====================================================
        // Main window
        // ====================================================

        ImGui::Begin(
            "Functional Space"
        );


        ImGui::Text(
            "Complex Projection System"
        );


        ImGui::Separator();


        // ====================================================
        // Function editor
        // ====================================================

        ImGui::Text(
            "Function Input"
        );


        ImGui::Text(
            "Enter mathematical expressions:"
        );


        ImGui::Text(
            "Examples: x^2, sin(x), cos(x), exp(-x^2)"
        );


        ImGui::Spacing();


        // ----------------------------------------------------
        // a(x)
        // ----------------------------------------------------

        ImGui::Text(
            "a(x)"
        );


        ImGui::SetNextItemWidth(
            -1
        );


        ImGui::InputText(
            "##fInput",
            fBuffer,
            sizeof(fBuffer)
        );


        // ----------------------------------------------------
        // b(x)
        // ----------------------------------------------------

        ImGui::Text(
            "b(x)"
        );


        ImGui::SetNextItemWidth(
            -1
        );


        ImGui::InputText(
            "##bInput",
            bBuffer,
            sizeof(bBuffer)
        );


        ImGui::Spacing();


        // ====================================================
        // Apply functions
        // ====================================================

        if (
            ImGui::Button(
                "Apply Functions",
                ImVec2(
                    200,
                    35
                )
            )
        )
        {
            try
            {
                std::string fExpression =
                    fBuffer;

                std::string bExpression =
                    bBuffer;


                Function newF =
                    makeFunction(
                        fExpression,
                        "a(x) = " +
                            fExpression
                    );


                Function newB =
                    makeFunction(
                        bExpression,
                        "b(x) = " +
                            bExpression
                    );


                // ------------------------------------------------
                // Use your existing setters.
                // ------------------------------------------------

                projection.setF(
                    newF
                );

                projection.setB(
                    newB
                );


                rebuildData();


                errorBuffer[0] =
                    '\0';
            }
            catch (
                const std::exception& e
            )
            {
                std::snprintf(
                    errorBuffer,
                    sizeof(errorBuffer),
                    "%s",
                    e.what()
                );
            }
        }


        if (errorBuffer[0] != '\0')
        {
            ImGui::Text(
                "Error: %s",
                errorBuffer
            );
        }


        ImGui::Separator();


        // ====================================================
        // Current functions
        // ====================================================

        ImGui::Text(
            "Current Functions"
        );


        ImGui::Text(
            "Forward: %s",
            projection
                .getF()
                .getName()
                .c_str()
        );


        ImGui::Text(
            "Backward: %s",
            projection
                .getB()
                .getName()
                .c_str()
        );


        ImGui::Separator();


        // ====================================================
        // Function Plane
        // ====================================================

        ImGui::Text(
            "Function Plane"
        );


        if (
            ImPlot::BeginPlot(
                "Function Plane",
                ImVec2(
                    -1,
                    350
                )
            )
        )
        {
            ImPlot::SetupAxes(
                "x",
                "y"
            );


            const auto& axis =
                projection.getComplexAxis();


            ImPlot::PlotLine(
                projection
                    .getF()
                    .getName()
                    .c_str(),

                axis.data(),

                fValues.data(),

                static_cast<int>(
                    axis.size()
                )
            );


            ImPlot::PlotLine(
                projection
                    .getB()
                    .getName()
                    .c_str(),

                axis.data(),

                bValues.data(),

                static_cast<int>(
                    axis.size()
                )
            );


            ImPlot::EndPlot();
        }


        ImGui::Separator();


        // ====================================================
        // Complex Projection
        // ====================================================

        ImGui::Text(
            "Complex Projection Plane"
        );


        ImGui::Text(
            "P(x) = a(x) + b(x)i"
        );


        if (
            ImPlot::BeginPlot(
                "Complex Plane",
                ImVec2(
                    -1,
                    450
                )
            )
        )
        {
            ImPlot::SetupAxes(
                "Real",
                "Projected"
            );


            ImPlot::PlotLine(
                "Complex Projection",

                projectionReal.data(),

                projectionImaginary.data(),

                static_cast<int>(
                    projectionReal.size()
                )
            );


            ImPlot::EndPlot();
        }


        ImGui::End();


        // ====================================================
        // Render
        // ====================================================

        ImGui::Render();


        int displayWidth;
        int displayHeight;


        glfwGetFramebufferSize(
            window,
            &displayWidth,
            &displayHeight
        );


        glViewport(
            0,
            0,
            displayWidth,
            displayHeight
        );


        glClearColor(
            0.05f,
            0.05f,
            0.05f,
            1.0f
        );


        glClear(
            GL_COLOR_BUFFER_BIT
        );


        ImGui_ImplOpenGL3_RenderDrawData(
            ImGui::GetDrawData()
        );


        glfwSwapBuffers(
            window
        );
    }


    // ========================================================
    // Cleanup
    // ========================================================

    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();

    ImPlot::DestroyContext();

    ImGui::DestroyContext();


    glfwDestroyWindow(
        window
    );

    glfwTerminate();


    return 0;
}