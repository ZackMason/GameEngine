#include "LayerStack.h"



LayerStack::LayerStack()
{
	m_layerItr = m_layers.begin();
}


void LayerStack::PushLayer(std::shared_ptr<Layer>  layer)
{
	m_layerItr = m_layers.emplace(m_layerItr, layer);
	layer->OnAttach();
}

void LayerStack::PushOverlay(std::shared_ptr<Layer> overlay)
{
	m_layers.emplace_back(overlay);
	overlay->OnAttach();
}

void LayerStack::PopLayer(std::shared_ptr<Layer>  layer)
{
	layer->OnDetach();
	auto it = std::find(m_layers.begin(), m_layers.end(), layer);
	if (it != m_layers.end())
	{
		m_layers.erase(it);
		m_layerItr--;
	}
}

void LayerStack::PopOverlay(std::shared_ptr<Layer> overlay)
{
	overlay->OnDetach();
	auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
	if (it != m_layers.end())
		m_layers.erase(it);
}
