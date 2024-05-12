#pragma once

#include <UI/Scenes/BasicScene/BasicScene.h>
#include <functional>

class OptionsScene : public BasicScene {
public:
    using OpenFileCallback = std::function<void(const std::string&)>;
    using ModeCallback = std::function<void()>;

    OptionsScene(const std::pair<float, float>& position, const std::pair<float, float>& size);

    /* Sets a callback that will be executed when a new filed is opened. The file name will be passed as
     * argument */
    void SetOpenFileCallback(OpenFileCallback callback);

    /* Sets a callback that will be executed when ip moves mode is selected */
    void SetModeIPCallback(ModeCallback callback);

    /* Sets a callback that will be executed when w2v mode is selected */
    void SetModeW2VCallback(ModeCallback callback);

    /* Sets a callback that will be executed when trajectory mode is selected */
    void SetModeTrajectoryCallback(ModeCallback callback);

private:
    void RenderInner() const override;

    /* Renders File section of menu */
    void FileMenu() const;

    /* Renders Mode section of menu */
    void ModeMenu() const;

    void OpenFileButton() const;

    OpenFileCallback openfile_callback_;
    ModeCallback mode_ip_callback_;
    ModeCallback mode_w2v_callback_;
    ModeCallback mode_trajectory_callback_;
};