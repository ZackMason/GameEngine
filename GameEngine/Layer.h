#pragma once

#include <string>

class Layer
{
public:
	Layer(const std::string& name = "Layer");
	virtual ~Layer();

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	virtual void OnEvent() {}

	inline const std::string& GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

protected:
	std::string m_name;
};

