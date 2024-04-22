#pragma once

#include <Graphics/RenderBuffer/RenderBuffer.h>
#include <UI/Scenes/BasicScene/BasicScene.h>

#include <unordered_set>

class IRenderable;  // Forward declaration

class ViewScene : public BasicScene {
public:
    ViewScene(const std::pair<float, float>& position, const std::pair<float, float>& size);

    /* Callback triggered when mouse moves */
    void OnMouseMove(double x, double y);

    /* Callback triggered when mouse scrolls */
    void OnMouseScroll(double delta);

    /* Resizes the scene and the underlaying render buffer */
    void Resize(const std::pair<float, float>& new_size);

    /* Adds an object to this scene. Returns true if the object is added for the first time and false
     * otherwise */
    bool AddObject(const IRenderable* object);

    /* Removes an object from this scene. Returns false if the object isn't present and true otherwise */
    bool RemoveObject(const IRenderable* object);

    /* Sets frametime that will be rendered and used to calculate FPS */
    void SetFrametime(double frametime);

private:
    /* Renders the scene*/
    void RenderInner() const override;

    /* Renders (only into the inner buffer) all renderable objects, added to this scene */
    void RenderObjects() const;

    /* Renders UI elements of the scene, draws the content of the inner buffer */
    void RenderUI() const;

    std::unordered_set<const IRenderable*> renderable_objects_;
    RenderBuffer render_buffer_;
    double frametime_;;
};