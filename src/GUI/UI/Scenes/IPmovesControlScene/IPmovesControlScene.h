#pragma once

#include <UI/Scenes/BasicScene/BasicScene.h>
#include <UI/Helpers/ValueWithPrev.h>

class IPmovesControlScene : public BasicScene {
    using Color = float[4];

public:
    IPmovesControlScene(const std::pair<float, float>& position, const std::pair<float, float>& size);

    /* Sets a progress value to the slider */
    void SetProgressValue(float progress);

    /* The following fuctions work as follows: they return a pair of <bool, T>. If the bool == true, then the
    T is new input from the user. Otherwise, the user hasn't inputed anything new (from the last time this
    function was called) yet. */

    [[nodiscard]] std::pair<bool, float> GetInputProgress();
    [[nodiscard]] std::pair<bool, bool> GetInputPause();
    [[nodiscard]] std::pair<bool, Color> GetInputNewColor();
    [[nodiscard]] std::pair<bool, Color> GetInputOldColor();
    [[nodiscard]] std::pair<bool, int> GetInputSlidingWindowSize();
    [[nodiscard]] std::pair<bool, int> GetInputAdvance();
    [[nodiscard]] std::pair<bool, int> GetInputHilbertDegree();
    [[nodiscard]] std::pair<bool, size_t> GetInputMaxMemory();

    struct IPmovesControlState {
        float progress;
        bool paused;
        Color new_color;
        Color old_color;
        int sliding_window_size;
        int advance;
        int hilbert_degree;
        size_t max_memory;
    };

    /* Returns the current state. It should be used only to initialize a new IPmovesHandler. For updates, use
     * GetInput functions, as they help to avoid redundant state updates */
    [[nodiscard]] IPmovesControlState GetState() const;

    /* Resets the state. It should be used when a new IPmovesHandler is created */
    void ResetState();

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
    mutable ValueWithPrev<Color> new_color_;
    mutable ValueWithPrev<Color> old_color_;
    mutable ValueWithPrev<int> sliding_window_size_;
    mutable ValueWithPrev<int> advance_;
    mutable ValueWithPrev<int> hilbert_degree_;
    mutable ValueWithPrev<int> max_memory_;
};