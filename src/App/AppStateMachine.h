#pragma once

#include <cstdint>

class AppStateMachine {
public:
    enum class States : uint32_t {
        FILE = (1u << 31),
        NO_FILE_IP = 1,
        FILE_IP = FILE + NO_FILE_IP,
        NO_FILE_W2V = (1U << 1),
        FILE_W2V = FILE + NO_FILE_W2V
    };

    AppStateMachine();
    AppStateMachine(States state);

    /* Returns true if the app is in some state that has file, false otherwise */
    bool HasFile() const;

    /* Returns current state */
    States GetState() const;

    /* Goes to the desired_state, if possible. If it's not, then throws an error */
    void GoToState(States desired_state);

    /* Returns true if it's possible to go to the desired_state, false otherwise */
    bool CanGoToState(States desired_state) const;

private:
    States state_;
};