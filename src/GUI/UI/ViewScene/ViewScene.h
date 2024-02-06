#pragma once

#include <unordered_set>
#include <memory>

class IRenderable;	// Forward declaration

class ViewScene {
public:
	ViewScene();

	/* Callback triggered when mouse moves */
	void OnMouseMove(double x, double y);

	/* Callback triggered when mouse scrolls */
	void OnMouseScroll(double delta);

	/* Renders the scene*/
	void Render() const;

	/* Adds an object to this scene. Returns true if the object is added for the first time and false otherwise */
	bool AddObject(const std::shared_ptr<IRenderable>& object);

	/* Removes an object from this scene. Returns false if the object isn't present and true otherwise */
	bool RemoveObject(const std::shared_ptr<IRenderable>& object);

private:
	/* Renders all object */
	void RenderAll() const;

	/* Renders UI elements of the scene */
	void MarkUpUI() const;

	std::unordered_set<std::shared_ptr<IRenderable>> renderable_objects_;
};