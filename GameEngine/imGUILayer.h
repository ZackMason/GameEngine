#pragma once
#include "Layer.h"
class imGUILayer :
	public Layer
{
public:
	imGUILayer();
	~imGUILayer();

	void OnAttach() override;
	void OnUpdate() override;
	void OnEvent() override;
	void OnDetach() override;
};

