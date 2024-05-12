#include "AppStateMachine.h"

#include <stdexcept>

#define ERROR_MESSAGE_UNREACHABLE_STATE "AppStateMachine: impossible state change"

AppStateMachine::AppStateMachine(States state) : state_(state) {
    states_graph_ = {
        {{States::NO_FILE_IP, States::FILE_IP}, []() {}},
        {{States::NO_FILE_IP, States::NO_FILE_W2V}, []() {}},
        {{States::NO_FILE_IP, States::NO_FILE_TRAJECTORY}, []() {}},
        {{States::NO_FILE_W2V, States::NO_FILE_IP}, []() {}},
        {{States::NO_FILE_W2V, States::FILE_W2V}, []() {}},
        {{States::NO_FILE_W2V, States::NO_FILE_TRAJECTORY}, []() {}},
        {{States::NO_FILE_TRAJECTORY, States::NO_FILE_IP}, []() {}},
        {{States::NO_FILE_TRAJECTORY, States::NO_FILE_W2V}, []() {}},
        {{States::NO_FILE_TRAJECTORY, States::FILE_TRAJECTORY}, []() {}},
        {{States::FILE_IP, States::FILE_W2V}, []() {}},
        {{States::FILE_IP, States::FILE_TRAJECTORY}, []() {}},
        {{States::FILE_W2V, States::FILE_IP}, []() {}},
        {{States::FILE_W2V, States::FILE_TRAJECTORY}, []() {}},
        {{States::FILE_TRAJECTORY, States::FILE_IP}, []() {}},
        {{States::FILE_TRAJECTORY, States::FILE_W2V}, []() {}},
    };
}

AppStateMachine::AppStateMachine() : AppStateMachine(States::NO_FILE_IP) {
}

bool AppStateMachine::HasFile() const {
    return static_cast<uint32_t>(state_) & static_cast<uint32_t>(States::FILE);
}

AppStateMachine::States AppStateMachine::GetState() const {
    return state_;
}

bool AppStateMachine::CanGoToState(States desired_state) const {
    return states_graph_.contains({state_, desired_state});
}

void AppStateMachine::GoToState(States desired_state) {
    if (!CanGoToState(desired_state)) {
        throw std::runtime_error(ERROR_MESSAGE_UNREACHABLE_STATE);
    }
    const auto old_state = state_;
    state_ = desired_state;
    states_graph_.at({old_state, state_})();
}

void AppStateMachine::AddCallback(const std::pair<States, States>& edge, std::function<void()> callback) {
    states_graph_[{edge.first, edge.second}] = std::move(callback);
}

void AppStateMachine::RemoveCallback(const std::pair<States, States>& edge) {
    if (states_graph_.contains({edge.first, edge.second})) {
        states_graph_[{edge.first, edge.second}] = []() {};
    }
}

void AppStateMachine::GoToStateWithFile() {
    const uint32_t desired_state_uint = static_cast<uint32_t>(state_) | static_cast<uint32_t>(States::FILE);
    const States desired_state = static_cast<States>(desired_state_uint);
    if (state_ == desired_state) {
        return;
    }
    GoToState(desired_state);
}

void AppStateMachine::GoToStateIgnoringFile(States desired_state) {
    const uint32_t file_bit = static_cast<uint32_t>(state_) & static_cast<uint32_t>(States::FILE);
    const uint32_t mask_without_file_bit = ~static_cast<uint32_t>(States::FILE);
    const uint32_t desired_state_uint =
        (static_cast<uint32_t>(desired_state) & mask_without_file_bit) | file_bit;
    desired_state = static_cast<States>(desired_state_uint);
    GoToState(desired_state);
}
