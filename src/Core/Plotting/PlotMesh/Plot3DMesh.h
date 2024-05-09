#pragma once

#include <Core/Plotting/PlotMesh/PlotMesh.h>
#include <glm/glm.hpp>
#include "glm/fwd.hpp"
#include <GUI/Graphics/Primitives/Cube/Cube.h>

class Plot3DMesh : public PlotMesh {
public:
    Plot3DMesh(size_t grid_size);

    /* Lets the Plot3DMesh to know about the size of the viewport so it can create a fitting projection
     * matrix*/
    void SetViewPortSize(float width, float height);

    /* Rotates the plot according to mouse move delta */
    void HandleMouseMove(float x, float y);

    /* Changes the distances to the object accoring to the mouse scroll */
    void HandleMouseScroll(float scroll);

    /* Uses the current camera data to set a proper transform uniform in the Plot3D shader */
    void LoadTransformFromCamera();

    /* Resets the camera to the defaults */
    void ResetCamera();

    void Render() const override;

private:
    class Camera3D {
    public:
        Camera3D();

        void HandleMouseMove(float x, float y, float strength = 0.01f);

        void HandleMouseScroll(float mouse_scroll, float strength = 0.5f);

        void SetScreenRatio(float widht, float height);

        void Reset();

        glm::mat4 GetViewTransform() const;

    private:
        glm::mat4 GetViewMatrix() const;

        glm::mat4 projection_matrix_;

        float pitch_;
        float yaw_;
        float distance_;
        glm::vec3 target_;
    };

    Camera3D camera_;
    Cube cube_;
};