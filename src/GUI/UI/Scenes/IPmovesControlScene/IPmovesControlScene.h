#pragma once

#include <UI/Scenes/BasicScene/BasicScene.h>

class IPmovesControlScene : public BasicScene {
public:
	IPmovesControlScene(const std::pair<float, float>& position, const std::pair<float, float>& size);

private:
	void RenderInner() const override;
};