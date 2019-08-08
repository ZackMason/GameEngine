#pragma once

#include "Layer.h"
#include <vector>

class LayerStack
{
public:
	LayerStack();
	~LayerStack() {};
	
	void PushLayer(std::shared_ptr<Layer> layer);
	void PushOverlay(std::shared_ptr<Layer> overlay);
	void PopLayer(std::shared_ptr<Layer> layer);
	void PopOverlay(std::shared_ptr<Layer> overlay);

	std::vector<std::shared_ptr<Layer>>::iterator begin() { return m_layers.begin(); }
	std::vector<std::shared_ptr<Layer>>::iterator end() { return m_layers.end(); }

private:
	std::vector<std::shared_ptr<Layer>> m_layers;
	std::vector<std::shared_ptr<Layer>>::iterator m_layerItr;
};

