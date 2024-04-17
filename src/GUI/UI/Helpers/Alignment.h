#pragma once

#include "imgui.h"

namespace alignment {

const constexpr float kBorderOffset = 8.0f;

class Wrapper {
public:
    explicit Wrapper(bool result);

    operator bool() const;

private:
    bool result_;
};

class AlignmentBase {
protected:
    AlignmentBase(const ImVec2& window_size);

    ImVec2 window_size_;
};

class Centered : public AlignmentBase {
public:
    Centered(const ImVec2& window_size);

    template <typename Func>
    Wrapper operator()(Func control) const {
        const ImVec2 original_pos = ImGui::GetCursorPos();

        const constexpr float kOffsetToVoid = -10000.0f;
        ImGui::SetCursorPos(ImVec2(kOffsetToVoid, kOffsetToVoid));
        control();
        const ImVec2 calculated_size = ImGui::GetItemRectSize();

        ImGui::SetCursorPos(ImVec2((window_size_.x - calculated_size.x) * 0.5f, original_pos.y));
        control();

        return Wrapper(ImGui::IsItemClicked());
    }
};

class Lefted : public AlignmentBase {
public:
    Lefted(const ImVec2& window_size);

    template <typename Func>
    Wrapper operator()(Func control) const {
        const ImVec2 original_pos = ImGui::GetCursorPos();

        ImGui::SetCursorPos(ImVec2(kBorderOffset, original_pos.y));
        control();

        return Wrapper(ImGui::IsItemClicked());
    }
};

class Righted : public AlignmentBase {
public:
    Righted(const ImVec2& window_size);

    template <typename Func>
    Wrapper operator()(Func control) const {
        const ImVec2 original_pos = ImGui::GetCursorPos();

        const constexpr float kOffsetToVoid = -10000.0f;
        ImGui::SetCursorPos(ImVec2(kOffsetToVoid, kOffsetToVoid));
        control();
        const ImVec2 calculated_size = ImGui::GetItemRectSize();

        ImGui::SetCursorPos(ImVec2((window_size_.x - calculated_size.x - kBorderOffset), original_pos.y));
        control();

        return Wrapper(ImGui::IsItemClicked());
    }
};

}  // namespace alignment

#define ALIGNMENT_AT_CENTER(control) alignment::Centered{ImGui::GetWindowSize()}([&]() { control; })

#define ALIGNMENT_AT_BEGIN(control) alignment::Lefted{ImGui::GetWindowSize()}([&]() { control; })

#define ALIGNMENT_AT_END(control) alignment::Righted{ImGui::GetWindowSize()}([&]() { control; })