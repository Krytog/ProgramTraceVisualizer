#pragma once

#include <GUI/Graphics/Primitives/IRenderable.h>
#include <GUI/Graphics/VertexObjects/VertexObjectIndexed.h>
#include <GUI/Graphics/Shader/Shader.h>

class ShaderWidget : public IRenderable {
public:
    ShaderWidget(const char* shader_code);

    /* Renders the widget, inherited from IRenderable */
    void Render() const override;

    /* Passes the time into shader, so it can be accessed in the shader code */
    void SetTime(float time);

    /* Sets the resolution of the screen, so it can be accessed in the shader code */
    void SetResolution(float width, float height);

protected:
    VertexObjectIndexed mesh_;
    Shader shader_;
};
