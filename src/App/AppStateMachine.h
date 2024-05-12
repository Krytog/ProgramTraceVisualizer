#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>

class AppStateMachine {
public:
    enum class States : uint32_t {
        FILE = (1u << 31),
        NO_FILE_IP = 1,
        FILE_IP = FILE | NO_FILE_IP,
        NO_FILE_W2V = (1u << 1),
        FILE_W2V = FILE | NO_FILE_W2V,
        NO_FILE_TRAJECTORY = (1u << 2),
        FILE_TRAJECTORY = FILE | NO_FILE_TRAJECTORY
    };

    using Callback = std::function<void()>;

    AppStateMachine();
    AppStateMachine(States state);

    /* Returns true if the app is in some state that has file, false otherwise */
    bool HasFile() const;

    /* Returns current state */
    States GetState() const;

    /* If currently in NO_FILE_SOME_STATE, then goes to FILE_SOME_STATE. Otherwise, does nothing */
    void GoToStateWithFile();

    /* Changes the file-bit fo desired_state to the file-bit of the current state, then goes to that changed
     * desired_state */
    void GoToStateIgnoringFile(States desired_state);

    /* Goes to the desired_state, if possible. If it's not, then throws an error */
    void GoToState(States desired_state);

    /* Returns true if it's possible to go to the desired_state, false otherwise */
    bool CanGoToState(States desired_state) const;

    /* Binds a callback that will be executed on change edge.first -> edge.second */
    void AddCallback(const std::pair<States, States>& edge, Callback callback);

    /* Removes a callback associated with the given edge */
    void RemoveCallback(const std::pair<States, States>& edge);

private:
    struct Hasher {
        std::size_t operator()(const std::pair<States, States>& edge) const {
            const auto first = std::hash<uint32_t>{}(static_cast<uint32_t>(edge.first));
            const auto second = std::hash<uint32_t>{}(static_cast<uint32_t>(edge.second));
            return (first * 0x1f1f1f1f) ^ second;
        }
    };

    States state_;
    std::unordered_map<std::pair<States, States>, Callback, Hasher> states_graph_;
};