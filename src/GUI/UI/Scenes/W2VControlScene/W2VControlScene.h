#pragma once

#include <UI/Scenes/BasicScene/BasicScene.h>
#include <UI/Helpers/ValueWithPrev.h>

class W2VControlScene : public BasicScene {
    using Color = float[4];

public:
    W2VControlScene(const std::pair<float, float>& position, const std::pair<float, float>& size);

    [[nodiscard]] int GetInputDimension();
    [[nodiscard]] const float* GetInputColor();
    [[nodiscard]] int GetInputNeighbours();
    [[nodiscard]] int GetInputEpochs();
    [[nodiscard]] int GetInputCells();
    [[nodiscard]] float GetInputMinDist();

    struct W2VControlState {
        int dimension;
        Color color;
        int neighbours;
        int epochs;
        int cells;
        float min_dist;
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

    /* Sets the dimension status that will be rendered in the UI. If real_dimension = 0, then recalc is in
     * progress  */
    void SetRealDimension(int real_dimension);

private:
    void RenderInner() const override;

    /* The following three functions mark up the widget and perform layout logic */
    void DimensionInfo() const;
    void CalculateButton() const;
    void SettingsButton() const;

    /* A lot of mutable variables here as Render of IRenderable is const, but DearImGui rendering and state
     * change always come together */
    mutable int dimension_;
    mutable float color_[4];
    mutable int neighbours_;
    mutable int epochs_;
    mutable int cells_;
    mutable float min_dist_;

    mutable ValueWithPrev<bool> is_pending_recalculation_;

    int real_dimension_{0};
};