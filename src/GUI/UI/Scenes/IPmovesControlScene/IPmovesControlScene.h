#pragma once

#include <UI/Scenes/BasicScene/BasicScene.h>
#include <utility>

class IPmovesControlScene : public BasicScene {
public:
    IPmovesControlScene(const std::pair<float, float>& position, const std::pair<float, float>& size);

    /* Sets a progress value to the slider */
    void SetProgressValue(float progress);

    /* Returns a pair of <bool, float>. If the bool == true, then the float is new input from the user.
    Otherwise, the user hasn't inputed anything yet. */
    std::pair<bool, float> GetUserInput();

private:
    void RenderInner() const override;

    mutable float cur_value_{0};
    float prev_value_{0};
};