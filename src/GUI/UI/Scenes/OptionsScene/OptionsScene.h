#pragma once

#include <UI/Scenes/BasicScene/BasicScene.h>

class OptionsScene : public BasicScene {
public:
	OptionsScene(const std::pair<float, float>& position, const std::pair<float, float>& size);

private:
	void RenderInner() const override;
};