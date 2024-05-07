#pragma once

#include <UI/Scenes/BasicScene/BasicScene.h>
#include <UI/Helpers/ValueWithPrev.h>

class W2VControlScene : public BasicScene {
    using Color = float[4];

public:
    W2VControlScene(const std::pair<float, float>& position, const std::pair<float, float>& size);

    /* The following fuctions work as follows: they return a pair of <bool, T>. If the bool == true, then the
    T is new input from the user. Otherwise, the user hasn't inputed anything new (from the last time this
    function was called) yet. */

    [[nodiscard]] std::pair<bool, int> GetInputDimension();
    [[nodiscard]] std::pair<bool, Color> GetInputColor();
    [[nodiscard]] std::pair<bool, int> GetInputNeighbours();
    [[nodiscard]] std::pair<bool, int> GetInputEpochs();
    [[nodiscard]] std::pair<bool, int> GetInputCells();
    [[nodiscard]] std::pair<bool, float> GetInputMinDist();

    struct W2VControlState {
        int dimension;
        Color color;
        int neighbours;
        int epochs;
        int cells;
        float min_dist;
        bool is_pending_recalculation;
    };

    /* Returns the current state. It should be used only to initialize a new W2VHandler. For updates, use
     * GetInput functions, as they help to avoid redundant state updates */
    [[nodiscard]] W2VControlState GetState() const;

    /* Resets the state. It should be used when a new W2VHandler is created */
    void ResetState();

    /* Returns true if the W2VHandler is pending recalculation */
    [[nodiscard]] std::pair<bool, bool> IsPendingRecalculation() const;

    /* When the recalculation is done, this method should be invoked to reset the apply button */
    void OnRecalculationFinished();

private:
    void RenderInner() const override;

    /* The following three functions mark up the widget and perform layout logic */
    void DimensionInfo() const;
    void CalculateButton() const;
    void SettingsButton() const;

    /* A lot of mutable variables here as Render of IRenderable is const, but DearImGui rendering and state
     * change always come together */
    mutable ValueWithPrev<int> dimension_;
    mutable ValueWithPrev<Color> color_;
    mutable ValueWithPrev<int> neighbours_;
    mutable ValueWithPrev<int> epochs_;
    mutable ValueWithPrev<int> cells_;
    mutable ValueWithPrev<float> min_dist_;
    mutable ValueWithPrev<bool> is_pending_recalculation_;
};