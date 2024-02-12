#pragma once

#include <UI/Scenes/BasicScene/BasicScene.h>

#include <string>
#include <deque>

class DetailsScene : public BasicScene {
public: 
	DetailsScene(const std::pair<float, float>& position, const std::pair<float, float>& size);

	/* Pushes a line into this scene */
	void PushLine(const std::string& line);

	/* Pops the front line */
	void PopFrontLine();

	/* Pops the back line */
	void PopBackLine();

	/* Gives the direct access to lines buffer */
	std::deque<std::string>& GetInnerBuffer();

private:
	/* Renders the scene */
	void RenderInner() const override;

	/* Renders the text, should be used only inside of RenderInner */
	void RenderText() const;

	/* Renders the static UI of the scene, should be used only inside of RenderInner and be the first render function called there */
	void RenderStaticUI() const;

	std::deque<std::string> lines_;
};