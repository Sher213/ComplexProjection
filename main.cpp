#include "ComplexProjection.h"
#include <iostream>
#include <vector>
#include <complex>
#include <random>
#include <cmath>
#include <numeric>

// ImGui Headers
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

class ProjectionUniformityEnvironment {
private:
    std::vector<double> domain;

public:
    ProjectionUniformityEnvironment(double minX, double maxX, double step) {
        for (double x = minX; x <= maxX; x += step) {
            domain.push_back(x);
        }
    }

    double evaluateUniformity(const ComplexProjection& projection) const {
        std::vector<std::complex<double>> points;
        points.reserve(domain.size());

        for (double x : domain) {
            points.push_back(projection.project(x, 1.0));
        }

        if (points.size() < 2) return 0.0;

        std::vector<double> distances;
        distances.reserve(points.size() - 1);

        double meanDistance = 0.0;
        for (size_t i = 0; i < points.size() - 1; ++i) {
            double dist = std::abs(points[i + 1] - points[i]);
            distances.push_back(dist);
            meanDistance += dist;
        }
        meanDistance /= distances.size();

        double variance = 0.0;
        for (double d : distances) {
            variance += (d - meanDistance) * (d - meanDistance);
        }
        variance /= distances.size();

        double spreadBonus = std::abs(points.back() - points.front());
        return (1.0 / (1.0 + variance)) * spreadBonus;
    }
};

class RLAgentFunctionOptimizer {
public:
    double wf, bf, wb, bb;
    double bestReward;
    double bestWf, bestBf, bestWb, bestBb;
    int episodeCount;

    std::default_random_engine generator;
    std::normal_distribution<double> perturbation;

    RLAgentFunctionOptimizer() 
        : wf(1.0), bf(0.0), wb(1.0), bb(0.0), 
          bestReward(-1.0), bestWf(1.0), bestBf(0.0), bestWb(1.0), bestBb(0.0),
          episodeCount(0), generator(std::random_device{}()), perturbation(0.0, 0.1) {}

    ComplexProjection getCurrentCandidate() const {
        return ComplexProjection(
            Function([this](double x) { return wf * x + bf; }, "Candidate_F"),
            Function([this](double x) { return wb * x + bb; }, "Candidate_B")
        );
    }

    void step(const ProjectionUniformityEnvironment& env, double learningRate) {
        episodeCount++;
        double oldWf = wf, oldBf = bf, oldWb = wb, oldBb = bb;

        wf += perturbation(generator) * learningRate;
        bf += perturbation(generator) * learningRate;
        wb += perturbation(generator) * learningRate;
        bb += perturbation(generator) * learningRate;

        double reward = env.evaluateUniformity(getCurrentCandidate());

        if (reward > bestReward) {
            bestReward = reward;
            bestWf = wf; bestBf = bf; bestWb = wb; bestBb = bb;
        } else {
            wf = oldWf; bf = oldBf; wb = oldWb; bb = oldBb;
        }
    }
};

int main() {
    // Initialize GLFW and OpenGL
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(900, 650, "Uniform Projection RL Dashboard", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync

    // Initialize ImGui Context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Setup RL Environment & Agent
    ProjectionUniformityEnvironment env(-5.0, 5.0, 0.2);
    RLAgentFunctionOptimizer agent;
    
    bool isRunning = true;
    float learningRate = 0.5f;
    int stepsPerFrame = 10;

    // Main GUI Application Loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Perform RL Training Steps if running
        if (isRunning) {
            for (int i = 0; i < stepsPerFrame; ++i) {
                agent.step(env, learningRate);
            }
        }

        // Create UI Dashboard Window
        ImGui::Begin("Reinforcement Learning: Uniform Complex Projection");
        
        ImGui::Text("Status: %s", isRunning ? "TRAINING ACTIVE" : "PAUSED");
        ImGui::Text("Total Episodes Evaluated: %d", agent.episodeCount);
        
        ImGui::Separator();
        ImGui::Text("Control Panel");
        if (ImGui::Button(isRunning ? "Pause Training" : "Resume Training")) {
            isRunning = !isRunning;
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset Agent")) {
            agent = RLAgentFunctionOptimizer();
        }

        ImGui::SliderFloat("Learning Rate", &learningRate, 0.01f, 2.0f);
        ImGui::SliderInt("Steps Per Frame", &stepsPerFrame, 1, 100);

        ImGui::Separator();
        ImGui::Text("Best Discovered Functions:");
        ImGui::BulletText("f(x) = %.4f * x + %.4f", agent.bestWf, agent.bestBf);
        ImGui::BulletText("b(x) = %.4f * x + %.4f", agent.bestWb, agent.bestBb);
        ImGui::Text("Max Uniformity Reward Achieved: %.6f", agent.bestReward);

        ImGui::Separator();
        ImGui::Text("Current Exploration State:");
        ImGui::BulletText("Candidate f(x) = %.4f * x + %.4f", agent.wf, agent.bf);
        ImGui::BulletText("Candidate b(x) = %.4f * x + %.4f", agent.wb, agent.bb);

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}