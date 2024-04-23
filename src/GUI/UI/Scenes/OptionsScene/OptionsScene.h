#pragma once

#include <UI/Scenes/BasicScene/BasicScene.h>
#include <functional>

class OptionsScene : public BasicScene {
public:
	using NewTraceCallback = std::function<void(const std::string&)>;

	OptionsScene(const std::pair<float, float>& position, const std::pair<float, float>& size);

	void SetNewTraceCallback(NewTraceCallback callback);	

private:
	void RenderInner() const override;

	void OpenFileButton() const;

	NewTraceCallback newtrace_callback_;
};