#pragma once

#include <GUI/Graphics/Primitives/ShaderWidget/ShaderWidget.h>

class ProgressWidget : public ShaderWidget {
public:
    ProgressWidget();

    /* Loads the given progress into the widget, progress should be within [0, 1] */
    void SetProgress(float progress);
};
