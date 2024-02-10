#pragma once

#include <Graphics/RenderBuffer/RenderBuffer.h>

#include <unordered_set>
#include <memory>

class IRenderable;	// Forward declaration

class ViewScene {
public:
	ViewScene(GLint width, GLint height);

	/* Callback triggered when mouse moves */
	void OnMouseMove(double x, double y);

	/* Callback triggered when mouse scrolls */
	void OnMouseScroll(double delta);

	/* Renders the scene*/
	void Render();

	/* Adds an object to this scene. Returns true if the object is added for the first time and false otherwise */
	bool AddObject(const std::shared_ptr<IRenderable>& object);

	/* Removes an object from this scene. Returns false if the object isn't present and true otherwise */
	bool RemoveObject(const std::shared_ptr<IRenderable>& object);

private:
	/* Renders (only into the inner buffer) all renderable objects, added to this scene */
	void RenderObjects() const;

	/* Renders UI elements of the scene, draws the content of the inner buffer */
	void RenderUI();

	/* Resizes the underlying rendering buffer if the widget's size doesn't fit. Returns the current size */
	std::pair<GLint, GLint> TryResize();

	std::unordered_set<std::shared_ptr<IRenderable>> renderable_objects_;
	RenderBuffer render_buffer_;
};