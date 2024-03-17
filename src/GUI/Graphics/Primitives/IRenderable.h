#pragma once

class IRenderable {
public:
	/* Renders the object into a frame buffer */
	virtual void Render() const = 0;

	virtual ~IRenderable() = default;
};