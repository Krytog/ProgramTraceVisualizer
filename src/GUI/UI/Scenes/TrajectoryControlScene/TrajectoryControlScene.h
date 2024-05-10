#pragma once

#include <UI/Scenes/BasicScene/BasicScene.h>
#include <UI/Helpers/ValueWithPrev.h>

class TrajectoryControlScene : public BasicScene {
    using Color = float[4];

public:
    TrajectoryControlScene(const std::pair<float, float>& position, const std::pair<float, float>& size);

    /* The following fuctions work as follows: they return a pair of <bool, T>. If the bool == true, then the
    T is new input from the user. Otherwise, the user hasn't inputed anything new (from the last time this
    function was called) yet. */

    [[nodiscard]] std::pair<bool, int> GetInputDimension();
    [[nodiscard]] std::pair<bool, Color> GetInputBeginColor();
    [[nodiscard]] std::pair<bool, Color> GetInputEndColor();
    [[nodiscard]] std::pair<bool, int> GetInputHilbertDegree();

    struct TrajectoryControlState {
        int dimension;
        Color begin_color;
        Color end_color;
        int hilbert_degree;
    };

    /* Returns the current state. It should be used only to initialize a new TrajectoryHandler. For updates, use
     * GetInput functions, as they help to avoid redundant state updates */
    [[nodiscard]] TrajectoryControlState GetState() const;

    /* Resets the state. It should be used when a new TrajectoryHandler is created */
    void ResetState();

    /* Sets the dimension status that will be rendered in the UI */
    void SetRealDimension(int real_dimension);

private:
    void RenderInner() const override;

    /* The following three functions mark up the widget and perform layout logic */
    void DimensionInfo() const;
    void DimensionSlider() const;
    void SettingsButton() const;

    /* A lot of mutable variables here as Render of IRenderable is const, but DearImGui rendering and state
     * change always come together */
    mutable ValueWithPrev<int> dimension_;
    mutable ValueWithPrev<Color> begin_color_;
    mutable ValueWithPrev<Color> end_color_;
    mutable ValueWithPrev<int> hilbert_degree_;

    int real_dimension_{0};
};