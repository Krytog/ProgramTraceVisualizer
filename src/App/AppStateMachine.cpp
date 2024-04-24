#include "AppStateMachine.h"

#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

#define ERROR_MESSAGE_UNREACHABLE_STATE "AppStateMachine: impossible state change"

namespace {
using States = AppStateMachine::States;

const static std::unordered_map<States, std::unordered_set<States>> kStateGraph = {
    {States::FILE_IP, {States::FILE_W2V}},
    {States::NO_FILE_IP, {States::FILE_IP, States::NO_FILE_W2V}},
    {States::FILE_W2V, {States::FILE_IP}},
    {States::NO_FILE_W2V, {States::FILE_W2V, States::NO_FILE_IP}},
};
}  // namespace

AppStateMachine::AppStateMachine() : state_(States::NO_FILE_IP) {
}

AppStateMachine::AppStateMachine(States state) : state_(state) {
}

bool AppStateMachine::HasFile() const {
    return static_cast<uint32_t>(state_) & static_cast<uint32_t>(States::FILE);
}

AppStateMachine::States AppStateMachine::GetState() const {
    return state_;
}

bool AppStateMachine::CanGoToState(States desired_state) const {
    if (!kStateGraph.contains(state_)) {
        return false;
    }
    return kStateGraph.at(state_).contains(desired_state);
}

void AppStateMachine::GoToState(States desired_state) {
    if (!CanGoToState(desired_state)) {
        throw std::runtime_error(ERROR_MESSAGE_UNREACHABLE_STATE);
    }
    state_ = desired_state;
}