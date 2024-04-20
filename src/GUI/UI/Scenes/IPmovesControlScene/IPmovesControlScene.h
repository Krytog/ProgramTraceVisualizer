#pragma once

#include <UI/Scenes/BasicScene/BasicScene.h>
#include <UI/Helpers/ValueWithPrev.h>

class IPmovesControlScene : public BasicScene {
public:
    IPmovesControlScene(const std::pair<float, float>& position, const std::pair<float, float>& size);

    /* Sets a progress value to the slider */
    void SetProgressValue(float progress);

    /* The following fuctions work as follows: they return a pair of <bool, T>. If the bool == true, then the
    T is new input from the user. Otherwise, the user hasn't inputed anything new (from the last time this
    function was called) yet. */

    [[nodiscard]] std::pair<bool, float> GetInputProgress();
    [[nodiscard]] std::pair<bool, bool> GetInputPause();
    [[nodiscard]] std::pair<bool, float[4]> GetInputNewColor();
    [[nodiscard]] std::pair<bool, float[4]> GetInputOldColor();
    [[nodiscard]] std::pair<bool, int> GetInputSlidingWindowSize();
    [[nodiscard]] std::pair<bool, int> GetInputAdvance();
    [[nodiscard]] std::pair<bool, int> GetInputHilbertDegree();
    [[nodiscard]] std::pair<bool, size_t> GetInputMaxMemory();

private:
    void RenderInner() const override;

    /* The following three functions mark up the widget and perform layout logic */
    void PauseButton() const;
    void ProgressSlider() const;
    void SettingsButton() const;

    /* A lot of mutable variables here as Render of IRenderable is const, but DearImGui rendering and state
     * change always come together */
    mutable ValueWithPrev<float> progress_;
    mutable ValueWithPrev<bool> pause_;
    mutable ValueWithPrev<float[4]> new_color_;
    mutable ValueWithPrev<float[4]> old_color_;
    mutable ValueWithPrev<int> sliding_window_size_;
    mutable ValueWithPrev<int> advance_;
    mutable ValueWithPrev<int> hilbert_degree_;
    mutable ValueWithPrev<int> max_memory_;
};