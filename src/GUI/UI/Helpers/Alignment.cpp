#include "Alignment.h"
#include "imgui.h"

namespace alignment {

Wrapper::Wrapper(bool result) : result_(result) {
}

Wrapper::operator bool() const {
    return result_;
}

AlignmentBase::AlignmentBase(const ImVec2& window_size) : window_size_(window_size) {
}

Centered::Centered(const ImVec2& window_size) : AlignmentBase(window_size) {
}

Lefted::Lefted(const ImVec2& window_size) : AlignmentBase(window_size) {
}

Righted::Righted(const ImVec2& window_size) : AlignmentBase(window_size) {
}

}  // namespace alignment