# Functional Space

**Functional Space** is an open-source tool for visualizing mathematical functions and exploring their relationships through a complex-plane projection.

## Overview

The project lets you define a pair of functions:

- **Forward function**, `f(x)`
- **Backward function**, `b(x)`

For example, the default configuration uses:

```
f(x) = sin(x)
b(x) = cos(x)
```

These two functions are first plotted together on a **Function Plane**, showing `f(x)` and `b(x)` overlaid on the same x-axis so their relationship (phase offset, symmetry, periodicity, etc.) can be visually compared.

## Complex Projection

The core feature of Functional Space is the **Complex Projection Plane**, which combines the forward and backward functions into a single complex-valued curve:

```
P(x) = f(x) + b(x) * x * i
```

Here, `f(x)` supplies the real component and `b(x) * x` supplies the imaginary component of the projected point at each `x`. As `x` sweeps across its domain, `P(x)` traces a path through the complex plane (plotted with **Real** on the horizontal axis and **Imaginary** on the vertical axis), revealing structure — loops, symmetry, and winding behavior — that isn't obvious from the individual `f(x)` and `b(x)` plots alone.

This makes Functional Space useful for:

- Visually exploring the interplay between a function and its derivative, transform, or paired counterpart
- Building intuition for how real-valued function pairs behave when interpreted as a complex-valued curve
- Quickly experimenting with different `f(x)` / `b(x)` definitions and seeing the resulting projection update

## Requirements

- A C++17 (or later) compiler
- [GLFW](https://www.glfw.org/) (windowing/OpenGL context)
- [Dear ImGui](https://github.com/ocornut/imgui) (with the GLFW + OpenGL3 backends)
- [ImPlot](https://github.com/epezent/implot)
- OpenGL

## Building

The project is a standard GLFW + Dear ImGui + ImPlot desktop application. A typical build simply compiles `main.cpp` alongside `Function`, `ComplexProjection`, ImGui/ImPlot, and their GLFW/OpenGL3 backend sources, then links against GLFW and OpenGL. Consult the build files in this repository for the exact commands for your platform.

## Usage

1. **Launch the application.** A window titled *Functional Space* opens, showing the current `f(x)` and `b(x)` definitions plotted on the **Function Plane**, and their combination plotted on the **Complex Projection Plane**.

2. **Edit the functions.** Under **Function Input**, type a math expression into the `f(x)` field and/or the `b(x)` field. Expressions support:
   - Standard operators: `+`, `-`, `*`, `/`, `^` (power), and parentheses
   - The variable `x`
   - Constants `pi` and `e`
   - Functions: `sin`, `cos`, `tan`, `asin`, `acos`, `atan`, `exp`, `log`, `sqrt`, `abs`

   Example expressions: `x^2`, `sin(x)`, `cos(x)`, `exp(-x^2)`.

3. **Apply the functions.** Click **Apply Functions** to parse and validate both expressions. If an expression is invalid, an error message is displayed and the previous functions remain in effect; otherwise, both plots update immediately.

4. **Read the plots.**
   - The **Function Plane** shows `f(x)` and `b(x)` overlaid over the sampled `x` range.
   - The **Complex Projection Plane** shows the resulting curve `P(x) = f(x) + b(x)·x·i` traced across the same range, with the current `f(x)` and `b(x)` definitions displayed under **Current Functions**.

5. **Iterate.** Change either expression and click **Apply Functions** again to explore how different function pairs shape the projected curve.

## Limitations

> **Note:** Functional Space only handles projections into a 2D space. It does not support projections into higher-dimensional (3D+) spaces.

## Open Source

Functional Space is **open source**. Contributions, issues, and pull requests are welcome.

## License

See the `LICENSE` file for details.
