#include "client/llm_client.h"
#include "../src/agent/agent_loop.h"
#include "../src/agent/loop_detector.h"

#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>

using namespace edu_schedule;

static void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

class MockLLM final : public LLMClient {
public:
    std::vector<std::string> replies;
    std::vector<std::vector<ChatMessage>> requests;
    std::size_t next{0};
    LLMConfig config;

    LLMResponse chat(const std::vector<ChatMessage>& messages) override {
        requests.push_back(messages);
        LLMResponse response;
        if (next < replies.size()) {
            response.success = true;
            response.content = replies[next++];
        }
        return response;
    }
    void set_config(const LLMConfig& value) override { config = value; }
    const LLMConfig& get_config() const noexcept override { return config; }
};

// Test-only Tool substitute; production ToolRegistry integration belongs to later tasks.
class TestAgent final : public AgentLoop {
public:
    using AgentLoop::AgentLoop;
    int calls{0};
protected:
    std::string act(const AgentDecision&) override {
        ++calls;
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        return "mock tool result";
    }
};

static void final_answer() {
    auto llm = std::make_shared<MockLLM>();
    llm->replies = {"done"};
    TestAgent agent(llm);
    std::vector<StepEvent> events;
    agent.setStepHook([&](const StepEvent& event) { events.push_back(event); });
    require(agent.run("goal") == "done", "final answer mismatch");
    require(agent.calls == 0 && events.size() == 1, "unexpected action/event count");
    require(events[0].result == "done", "final result missing from hook");
}

static void tool_then_answer() {
    auto llm = std::make_shared<MockLLM>();
    llm->replies = {"Action:mock", "done"};
    TestAgent agent(llm);
    std::vector<StepEvent> events;
    agent.setStepHook([&](const StepEvent& event) {
        require(agent.calls == 1, "hook ran before action completed");
        events.push_back(event);
    });
    require(agent.run("goal") == "done", "tool flow failed");
    require(events.size() == 2, "missing step event");
    require(events[0].result == "mock tool result", "tool result missing");
    require(events[0].duration_ms >= 10, "duration excludes action");
    require(events[1].observation == "mock tool result", "result not passed to next step");
    require(llm->requests[1].back().content == "Observation: mock tool result",
            "LLM did not receive tool result");
}

static void step_limit() {
    auto llm = std::make_shared<MockLLM>();
    llm->replies = {"Action:first", "Action:second", "must not be read"};
    TestAgent agent(llm, 2);
    int events = 0;
    agent.setStepHook([&](const StepEvent&) { ++events; });
    require(agent.run("goal").find("gioi han 2") != std::string::npos,
            "step exhaustion reported as success");
    require(agent.calls == 2 && llm->next == 2 && events == 2,
            "last action skipped or step limit exceeded");
    TestAgent zero(llm, 0);
    require(zero.run("goal").find("gioi han 0") != std::string::npos,
            "zero budget not handled");
    require(llm->next == 2, "zero budget called LLM");
}

static void repeat_and_reset() {
    auto llm = std::make_shared<MockLLM>();
    llm->replies = {"Action:same", "Action:same", "Action:same", "Action:same"};
    TestAgent agent(llm, 6);
    std::vector<StepEvent> events;
    agent.setStepHook([&](const StepEvent& event) { events.push_back(event); });
    require(agent.run("first goal").find("Dung khan cap") == 0, "loop not stopped");
    require(agent.calls == 3 && events.size() == 4, "blocked action executed or not logged");
    require(events[1].is_loop_warning, "repeat warning missing");
    require(events.back().status == StepStatus::LoopBlocked, "blocked status missing");
    llm->replies = {"Action:same", "done"};
    llm->next = 0;
    events.clear();
    require(agent.run("second goal") == "done", "loop detector not reset");
    require(!events[0].is_loop_warning, "stale repeat warning");
    const auto& fresh = llm->requests[4];
    require(fresh.size() == 3 && fresh[1].content == "second goal", "history not reset");
}

static void failed_thinking() {
    auto llm = std::make_shared<MockLLM>();
    TestAgent agent(llm);
    std::vector<StepEvent> events;
    agent.setStepHook([&](const StepEvent& event) { events.push_back(event); });
    require(agent.run("goal").find("Loi:") == 0, "LLM failure not reported");
    require(agent.calls == 0 && events.size() == 1, "failed step not logged");
    require(events[0].status == StepStatus::ThinkingFailed, "failure status missing");
}

static void ping_pong() {
    LoopDetector detector;
    detector.recordAction("a");
    detector.recordAction("b");
    detector.recordAction("a");
    auto warning = detector.recordAndCheck("b");
    require(warning.isWarning() && warning.type == LoopType::PingPong, "ping-pong warning missing");
    detector.recordAction("a");
    require(detector.recordAndCheck("b").isCritical(), "ping-pong stop missing");
}

int main() {
    try {
        final_answer();
        tool_then_answer();
        step_limit();
        repeat_and_reset();
        failed_thinking();
        ping_pong();
        std::cout << "AgentLoop: all 6 offline scenarios passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "AgentLoop test failure: " << error.what() << '\n';
        return 1;
    }
}
