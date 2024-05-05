#pragma once

#include <GUI/Graphics/Primitives/ShaderWidget/ShaderWidget.h>
#include <Utils/LightTimer/LightTimer.h>

class ProgressWidget : public ShaderWidget {
public:
    ProgressWidget();

    /* Loads the given progress into the widget, progress should be within [0, 1] */
    void SetProgress(float progress);

    /* Goes into a mode with no progress indication. Should be used when waiting for events with no progress information */
    void GoToWaitingMode();

    /* Goes into the usual mode with progress indication */
    void GoToProgressMode();

private:
    bool is_in_progress_mode_;
    LightTimer timer_;
};
