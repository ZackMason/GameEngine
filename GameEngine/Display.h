#pragma once

#include "include/SDL2/SDL.h"
#include <string>

#define W_SIZE 2

#if W_SIZE == 1

#define WIDTH 1280
#define HEIGHT 720

#endif // W_SIZE 1

#if W_SIZE == 2

#define WIDTH 1920
#define HEIGHT 1080

#endif

#if W_SIZE == 3

#define WIDTH 640
#define HEIGHT 400

#endif

//1920x1080
//1280x720
//640x400

class Display
{
public:
	Display(const std::string& title, int width = WIDTH, int height = HEIGHT);
	virtual ~Display();

	void Clear   (float r, float g, float b, float a);
	void Update  ();
	bool IsClosed() const;

	void SetWindowSize(int width, int height)
	{
		m_width  = width;
		m_height = height;
		SDL_SetWindowSize(m_window, m_width, m_height);
	}

	SDL_GLContext GetContext() { return m_glContext; }

	void SetIsClosed(bool val) { m_isClosed = val; }

	inline SDL_Window *GetWindow() const { return m_window; }
	inline float       GetAspect() const { return (float)m_width / (float)m_height; }

private:
	Display(const Display& other) {}
	Display& operator=(const Display& other) {}
	
	int           m_width;
	int           m_height;
	bool          m_isClosed;
	SDL_Window   *m_window;
	SDL_GLContext m_glContext;
};

