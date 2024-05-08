#pragma once

#include <Core/Plotting/PlotMesh/PlotMesh.h>
#include <glm/glm.hpp>

class Plot3DMesh : public PlotMesh {
public:
    Plot3DMesh(size_t grid_size);

    /* Sets the matrix that controls position and rotation of the plot's camera */
    void SetCameraTransform(const glm::mat4& transform_matrix);

    /* Returns the current transform of the plot's camera  */
    [[nodiscard]] glm::mat4 GetCameraTransform() const;

    /* Translates the camera */
    void TranslateCamera(const glm::vec3& vector);

    /* Rotates the camera */
    void RotateCamera(float strength, const glm::vec3& axis);

    /* Lets the Plot3DMesh to know about the size of the viewport so it can create a fitting projection
     * matrix*/
    void SetViewPortSize(float width, float height);

private:
    /* Applies the matrices by passing them into the shader */
    void ApplyMatrices();

    glm::mat4 transform_matrix_;
    glm::mat4 view_matrix_;
    glm::mat4 projection_matrix_;
};