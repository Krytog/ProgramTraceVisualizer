#pragma once

#include <UI/Scenes/ViewScene/ViewScene.h>
#include <UI/Scenes/DetailsScene/DetailsScene.h>
#include <UI/Scenes/OptionsScene/OptionsScene.h>
#include <UI/Scenes/IPmovesControlScene/IPmovesControlScene.h>
#include <UI/Scenes/W2VControlScene/W2VControlScene.h>
#include <UI/Scenes/TrajectoryControlScene/TrajectoryControlScene.h>

#include <utility>

class UIManager {
public:
    enum class Mode { Waiting, IP, W2V, Trajectory };

    UIManager(struct GLFWwindow* glfw_window, const std::pair<int, int>& size);

    ~UIManager();

    /* Resizes all the GUI. Should be called when the window is resized */
    void Resize(const std::pair<int, int>& new_size);

    /* Draws all the UI into the render buffer */
    void DrawUI() const;

    /* Returns a reference to the ViewScene of the UI */
    [[nodiscard]] ViewScene& GetViewScene();

    /* Returns a reference to the DetailsScene of the UI */
    [[nodiscard]] DetailsScene& GetDetailsScene();

    /* Returns a reference to the OptionsScene of the UI */
    [[nodiscard]] OptionsScene& GetOptionsScene();

    /* Returns a reference to the IPmovesControlScene of the UI */
    [[nodiscard]] IPmovesControlScene& GetIPmovesControlScene();

    /* Returns a reference to the W2VControlScene of the UI */
    [[nodiscard]] W2VControlScene& GetW2VControlScene();

    /* Returns a reference to the TrajectoryControlScene of the UI */
    [[nodiscard]] TrajectoryControlScene& GetTrajectoryControlScene();

    /* Sets UI to the waiting for file mode */
    void GoToWaitingForFileMode();

    /* Sets UI to the ip moves mode */
    void GoToIPmovesMode();

    /* Sets UI to the w2m mode */
    void GoToW2VMode();

    /* Sets UI to the trajectory mode*/
    void GoToTrajectoryMode();

private:
    /* Inits underlying ImGui library that is used for UI handling. Should be called in the c-tor */
    void InitImGui(struct GLFWwindow* glfw_window) const;

    /* Renders all the scenes of the UI */
    void RenderAllScenes() const;

    Mode mode_;
    std::pair<int, int> current_size_;

    ViewScene view_scene_;
    DetailsScene details_scene_;
    OptionsScene options_scene_;
    IPmovesControlScene ipmovescontrol_scene_;
    W2VControlScene w2vcontrol_scene_;
    TrajectoryControlScene trajectorycontrol_scene_;
};