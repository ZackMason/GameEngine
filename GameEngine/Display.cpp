#include "Display.h"

#include "include/GLEW/GL/glew.h"

#include <iostream>

Display::Display(const std::string& title, int width, int height)
{
	m_width  = width;
	m_height = height;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE    , 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE  , 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE   , 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE  , 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE , 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE  , 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_OPENGL);
	m_glContext = SDL_GL_CreateContext(m_window);

	GLenum status = glewInit();

	if (status != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize" << std::endl;
	}
	
	m_isClosed = false;

	// Setup OpenGL options
	glEnable    (GL_DEPTH_TEST);
	glEnable    (GL_DEPTH_CLAMP);
	glEnable    (GL_CULL_FACE);
	glEnable	(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace  (GL_BACK);
	glDepthFunc (GL_LESS);
	glEnable	(GL_MULTISAMPLE);
	std::cout << "Window Constructed" << std::endl;
}


Display::~Display()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow   (m_window);

	SDL_Quit();
	std::cout << "Destructor" << std::endl;
}

void Display::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Display::IsClosed() const
{
	return m_isClosed;
}

void Display::Update()
{
	SDL_GL_SwapWindow(m_window);

	SDL_Event e;

	while (SDL_PollEvent((&e)))
	{
		if(e.type == SDL_QUIT)
			m_isClosed = true;
	}
}
