#pragma once

#include <string>

class Layer
{
public:
	Layer();
	Layer(const std::string& name = "Layer");
	virtual ~Layer();

	virtual void OnAttach() {}
	virtual void OnDettach() {}
	virtual void OnUpdate() {}
	virtual void OnEvent() {}

	inline const std::string& GetName() const { return m_name; }

protected:
	std::string m_name;
};

