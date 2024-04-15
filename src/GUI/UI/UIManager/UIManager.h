#pragma once

#include <UI/Scenes/ViewScene/ViewScene.h>
#include <UI/Scenes/DetailsScene/DetailsScene.h>
#include <UI/Scenes/OptionsScene/OptionsScene.h>

#include <utility>

class UIManager {
public:
    UIManager(struct GLFWwindow* glfw_window, const std::pair<int, int>& size);

    ~UIManager();

    /* Draws all the UI into the render buffer */
    void DrawUI() const;

    /* Returns the reference to the ViewScene of the UI */
    ViewScene& GetViewScene();

    /* Returns the reference to the DetailsScene of the UI */
    DetailsScene& GetDetailsScene();

    /* Returns the reference to the OptionsScene of the UI */
    OptionsScene& GetOptionsScene();

    /* Returns a pointer to a UIManager that is marked as main */
    static UIManager* GetMainUIManager();

    /* Marks a UIManager as main */
    void MarkAsMain();

private:
    /* Inits underlying ImGui library that is used for UI handling. Should be called in the c-tor */
    void InitImGui(struct GLFWwindow* glfw_window) const;

    /* Renders all the scenes of the UI */
    void RenderAllScenes() const;

    ViewScene view_scene_;
    DetailsScene details_scene_;
    OptionsScene options_scene_;

    static UIManager* k_main_ui_manager;
};