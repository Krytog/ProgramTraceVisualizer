#pragma once

#include <Graphics/RenderBuffer/RenderBuffer.h>
#include <UI/Scenes/BasicScene/BasicScene.h>

#include <unordered_set>

class IRenderable;  // Forward declaration

class ViewScene : public BasicScene {
public:
    ViewScene(const std::pair<float, float>& position, const std::pair<float, float>& size);

    /* Returns the delta of mouse position since the last frame */
    [[nodiscard]] std::pair<float, float> GetMouseDelta() const;

    /* Returns the value of mouse scroll (0 is no scroll, a positive value if scrolls upwards and a negative
     * value if scrolls downwards)*/
    [[nodiscard]] float GetMouseScroll() const;

    /* Returns whether the left mouse button is pressed within the current frame */
    [[nodiscard]] bool IsMousePressed() const;

    /* Override UpdateOnResize from BasicScene, as we need to resize the rendering buffer */
    void UpdateOnResize(const std::pair<float, float>& posiiton, const std::pair<float, float>& size);

    /* Adds an object to this scene. Returns true if the object is added for the first time and false
     * otherwise */
    bool AddObject(const IRenderable* object);

    /* Removes an object from this scene. Returns false if the object isn't present and true otherwise */
    bool RemoveObject(const IRenderable* object);

    /* Sets frametime that will be rendered and used to calculate FPS */
    void SetFrametime(double frametime);

    /* Returns the size of the view port. Notice that this size is slightly different from the scene size */
    [[nodiscard]] std::pair<float, float> GetViewPortSize() const;

private:
    /* Renders the scene*/
    void RenderInner() const override;

    /* Renders (only into the inner buffer) all renderable objects, added to this scene */
    void RenderObjects() const;

    /* Renders UI elements of the scene, draws the content of the inner buffer */
    void RenderUI() const;

    /* Takes raw input from ImGui and stores it so it can be accessed by controllers. Should be marked const
     * as it's called from render inner */
    void UpdateMouseInput() const;

    std::unordered_set<const IRenderable*> renderable_objects_;
    RenderBuffer render_buffer_;
    double frametime_;

    mutable std::pair<float, float> mouse_delta_{0, 0};
    mutable float mouse_scroll_{0};
    mutable bool is_mouse_pressed_{false};
};