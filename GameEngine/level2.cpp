#include <iostream>
#include <string>
#include <filesystem>
#include "Application.h"
#include "Level2.h"
#include "Config.h"

static Vertex screen_quad[] = { Vertex(glm::vec3(-1.0,1.0,0.0),glm::vec2(0.0,1.0)),
						   Vertex(glm::vec3(-1.0,-1.0,0.0),glm::vec2(0.0,0.0)),
						   Vertex(glm::vec3(1.0,-1.0,0.0),glm::vec2(1.0,0.0)),
						   Vertex(glm::vec3(-1.0,1.0,0.0),glm::vec2(0.0,1.0)),
						   Vertex(glm::vec3(1.0,-1.0,0.0),glm::vec2(1.0,0.0)),
						   Vertex(glm::vec3(1.0,1.0,0.0),glm::vec2(1.0,1.0))
};

Level2::Level2(const Display &d) : m_camera(glm::vec3(0.1, 4, 0), 70.0f, d.GetAspect(), 1.f, 12000.0f),
m_ofbo(),
m_screen_sdr("./res/SHADERS/deferredPBRlightShader"),
m_screen(screen_quad, 6),
m_player("CAR_01", "deferredPBR", "wood"),
m_sky("skydome2", "skydome", "skydome")
{
	m_sky.m_transform->GetScale() *= 80;
	m_player.m_transform->GetPos().z -= 42;
	m_player.m_transform->GetPos().x = 44;
	m_player.m_transform->GetPos().y -= 3;
	m_player.m_transform->GetScale() *= 15;
	auto a = Actor("CAR_window_01", "deferredPBR", "wood");
	a.m_transform->GetPos().z -= 42;
	a.m_transform->GetPos().x = 44;
	a.m_transform->GetPos().y -= 3;
	a.m_transform->GetScale() *= 15;
	m_terrain.push_back(a);
	auto b = Actor("CAR_wheel_01", "deferredPBR", "wood");
	b.m_transform->GetPos().z -= 42;
	b.m_transform->GetPos().x = 44;
	b.m_transform->GetPos().y -= 3;
	b.m_transform->GetScale() *= 15;
	m_terrain.push_back(b);
}

Level2::~Level2()
{
}

void Level2::Add_Actor(std::shared_ptr<Actor> actor)
{

}

void Level2::Display_Levels()
{
	namespace fs = std::experimental::filesystem;
	std::string path = "./res/LEVELS";
	for (const auto & entry : fs::directory_iterator(path))
		std::cout << entry.path() << "\n";

}

bool Level2::Load_Levels()
{

	return true;
}

void Level2::Render_Level()
{

}

void Level2::OnUpdate()
{
	auto  display = Application::Get().GetDisplay();
	auto  state = Application::Get().GetState();
	m_clock.Update();

	//INPUT BEGIN HERE, NEEDS ABSTRACTION

//SDL_PumpEvents();

	//mouse recording

	(SDL_GetMouseState(&mousex, &mousey));
	if (SDL_GetRelativeMouseMode())
	{
		lastx = thisx;
		lasty = thisy;
		thisx = mousex;
		thisy = mousey;
		m_camera.RotY(-(lastx - thisx) / 6.0f * m_clock.Get_DT());
		m_camera.RotX(-(lasty - thisy) / 6.0f * m_clock.Get_DT());
	}

#if 0
	//capture mouse when you click on the window
	if (SDL_GetMouseState(&mousex, &mousey))
	{
		SDL_CaptureMouse(SDL_TRUE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
#endif

	// release the mouse
	if (state[SDL_SCANCODE_B])
		display->SetIsClosed(true);
	if (state[SDL_SCANCODE_E])
		auto_move = !auto_move;
	if (state[SDL_SCANCODE_ESCAPE])
		SDL_SetRelativeMouseMode(SDL_FALSE);
	if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
		m_camera.MoveForward(Config::Get().playerSpeed);
	if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
		m_camera.MoveForward(-14.f);
	if (state[SDL_SCANCODE_I])
		m_camera.RotX(-0.46f * m_clock.Get_DT());
	if (state[SDL_SCANCODE_K])
		m_camera.RotX(
			0.46f * m_clock.Get_DT());
	if (state[SDL_SCANCODE_SPACE])
		m_camera.MoveUp(14.f);
	if (state[SDL_SCANCODE_A])
		m_camera.MoveRight(14.f);
	if (state[SDL_SCANCODE_D])
		m_camera.MoveRight(-14.f);
	if (state[SDL_SCANCODE_C])
		m_camera.MoveUp(-14.f);
	if (state[SDL_SCANCODE_F])
	{
		SDL_SetWindowFullscreen(display->GetWindow(), SDL_WINDOW_FULLSCREEN);
		//display->SetWindowSize(1920, 1080);
		//SDL_SetVideoMode(SDL_FULLSCREEN);
		SDL_Surface* screenSurface = SDL_GetWindowSurface(display->GetWindow());
		SDL_UpdateWindowSurface(display->GetWindow());
		m_camera.SetAspect(display->GetAspect());
	}

	display->Clear(0.5f, 0.5f, 1.f, 1.0f);

	//camera.LookAt(PlaneActor.m_transform->GetPos());

	//std::cout << "-h = " << -h(glm::vec2(height_v.x, height_v.z)) << "\n";
	m_camera.Update(m_clock.Get_DT());

	m_sky.m_transform->SetPos(m_camera.GetPos());


	if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_L])
	{
		m_camera.RotY(0.816f * m_clock.Get_DT());
	}
	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_J])
	{
		m_camera.RotY(-0.816f * m_clock.Get_DT());
	}


	//Drawing begins here, needs abstraction

#if 1
	//m_ofbo.Bind();
	m_gbo.Bind();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, .0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif


	//glDepthMask(GL_FALSE);
	//m_sky.Draw(m_camera, m_clock.Get_Time_Passed());
	//glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (auto& a : m_actors)
	{
		glm::vec3 ray = a.m_transform->GetPos() - m_camera.GetPos();
		ray = glm::normalize(ray);
		ray *= -1.0;
		//if (glm::dot(ray, camera.GetForward()) > -0.2)
		a.Draw(m_camera, m_clock.Get_Time_Passed(),m_lights);
		a.m_transform->GetRot() += m_clock.Get_DT();
		a.m_transform->QUpdate();
	}

	m_player.Draw(m_camera, m_clock.Get_Time_Passed());

#if 0
	for (unsigned int i = 0; i < Water.size(); i++)
	{
		float distance = glm::length(camera.GetPos() - Water[i].m_transform->GetPos());
		sorted[distance] = &Water[i];
	}

	for (std::map<float, Actor*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		it->second->Draw(camera, time_passed);
	}
	sorted.clear();
#else
	for (auto &a : m_terrain)
	{
		a.Draw(m_camera, m_clock.Get_Time_Passed(), m_lights);

	}

	for (auto &l :m_lights)
	{
		if (l->m_position.w > 0.0f)
			continue;
		l->m_position.x += (rand() % 3 - 1);
		l->m_position.z += (rand() % 3 - 1);
	}

#endif
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	//calculate light
	m_gbo.Unbind(); // Binds gbuffers attachments!!!!!!


	if (Config::wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	m_ofbo.Bind();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_screen_sdr.Bind();
	
	glBindVertexArray(m_screen.GetVAO());
	m_screen_sdr.setMat4("u_projection", m_camera.GetProjection());
	m_screen_sdr.setMat4("u_inv_projection", glm::inverse(m_camera.GetProjection()));
	m_screen_sdr.setMat4("u_view", m_camera.GetView());
	m_screen_sdr.setMat4("u_inv_view", glm::inverse(m_camera.GetView()));
	m_screen_sdr.setInt("u_pos", 0);
	m_screen_sdr.setInt("u_color", 1);
	m_screen_sdr.setInt("u_norm", 2);
	m_screen_sdr.setInt("u_last_frame", 3);
	//m_screen_sdr.setFloat("metallic", Config::bias);
	//m_screen_sdr.setFloat("roughness", Config::lp);
	m_screen_sdr.setFloat("lp", Config::lp);
	m_screen_sdr.SetLights(m_lights);
	m_screen_sdr.setVec3("u_viewpos", m_camera.GetPos());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glClear(GL_DEPTH_BUFFER_BIT);

	//glBindFramebuffer(GL_READ_BUFFER, m_ofbo);
	//glCopyTexImage2D(m_gbo.m_last_frame, 0, GL_RGBA, 0, 0, WIDTH, HEIGHT, 0);
	m_ofbo.Unbind();

#if 1
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gbo.m_buffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ofbo); // write to default framebuffer
	glBlitFramebuffer(
		0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ofbo);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	m_sky.Draw(m_camera, m_clock.Get_Time_Passed());
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

#endif

	m_ofbo.Unbind();


	//post process - tonemapping / HDR gamma correction
#if 1

	//save last frame to gbuffer to use for ssr without gamma corrections


	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	scr_sdr2->Bind();
	glBindVertexArray(m_screen.GetVAO());
	m_ofbo.Bind_TA(0);
	glGenerateMipmap(GL_TEXTURE_2D);

#if 1
	glCopyImageSubData(m_ofbo.GetColor() , GL_TEXTURE_2D, 0, 0, 0, 0,
		m_gbo.m_last_frame, GL_TEXTURE_2D, 0, 0, 0, 0,
		WIDTH, HEIGHT, 1);
#endif	
	m_ofbo.Bind_DTA(1);
	glGenerateMipmap(GL_TEXTURE_2D);

	//glCopyTexImage2D(m_gbo.m_last_frame, 0, GL_RGBA, 0, 0, WIDTH, HEIGHT, 0);
	//m_screen_sdr.setFloat("Time", m_clock.Get_Time_Passed());
	scr_sdr2->setFloat("gamma", Config::Get().gamma);
	scr_sdr2->setFloat("expos", Config::Get().expos);
	scr_sdr2->setInt("diffuse", 0);

	scr_sdr2->setInt("depth", 1);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);
#endif
	if (Config::wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}

void Level2::OnAttach()
{
	int water_size = 40;
	int water_scale = 6;
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("./res/MESHS/plane_02.obj");
	std::shared_ptr<Texture> box_tex = std::make_shared<Texture>("./res/TEXTURES/Generated_Checker_Tiles/Generated_Checker_Tiles_DIFF.png");
	std::shared_ptr<Texture> water_tex_r = std::make_shared<Texture>("./res/TEXTURES/Generated_Checker_Tiles/Generated_Checker_Tiles_RGH.png");
	std::shared_ptr<Texture> water_tex_n = std::make_shared<Texture>("./res/TEXTURES/Generated_Checker_Tiles/Generated_Checker_Tiles_NRM.png");
	std::shared_ptr<Texture> water_tex = std::make_shared<Texture>("./res/TEXTURES/Water_001_SD/Water_001_COLOR.jpg");
	//std::shared_ptr<Texture> water_tex_n = std::make_shared<Texture>("./res/TEXTURES/Water_001_SD/Water_001_NORM.jpg");
	//std::shared_ptr<Texture> water_tex_r = std::make_shared<Texture>("./res/TEXTURES/Water_001_SD/Water_001_COLOR.jpg");
	//std::shared_ptr<Texture> water_spec = std::make_shared<Texture>("./res/TEXTURES/water_dudv.jpg");
	std::shared_ptr<Shader> water_sdr = std::make_shared<Shader>("./res/SHADERS/deferredPBRTexShader");
	std::shared_ptr<Material> water_mat = std::make_shared<Material>(water_sdr, box_tex);
	water_mat->Add(water_tex_n);
	for (int x = -water_size; x <= water_size; x++)
	{
		for (int z = -water_size; z <= water_size; z++)
		{
			Actor world(mesh, water_mat);
			world.m_transform->GetPos().y -= 3.0f;
			world.m_transform->GetPos().z = z * 20. * water_scale;
			world.m_transform->GetPos().x = x * 20. * water_scale;
			world.m_transform->GetScale() *= water_scale;
			world.m_transform->GetScale().y = 1;

			m_terrain.push_back(world);
		}
	}

	std::shared_ptr<Mesh> bmesh = std::make_shared<Mesh>("./res/MESHS/monkey.obj");
	std::shared_ptr<Texture> btex = std::make_shared<Texture>("./res/TEXTURES/box_diffuse.png");
	std::shared_ptr<Texture> bspec = std::make_shared<Texture>("./res/TEXTURES/box_specular.png");
	std::shared_ptr<Shader> b_sdr = std::make_shared<Shader>("./res/SHADERS/deferredPBRShader");
	std::shared_ptr<Material> bmat = std::make_shared<Material>(b_sdr, btex);
	bmat->Add(bspec);
	

	for (int x = 0; x < 20; x++)
	{
		for (int z = 0; z < 20; z++)
		{
			Actor box(bmesh, bmat);
			box.m_transform->GetPos().z = z * 15;
			box.m_transform->GetPos().x = x * 15;
			box.m_transform->GetScale() *= 5;
			box.m_transform->GetPos().y += box.m_transform->GetScale().y - 1.f  * .5f;
			m_actors.push_back(box);
		}
	}
	water_sdr = std::make_shared<Shader>("./res/SHADERS/deferredPBRTexShader");
	std::shared_ptr<Material> water_mat2 = std::make_shared<Material>(water_sdr, box_tex);
	bmesh = std::make_shared<Mesh>("./res/MESHS/cube.obj");
	water_mat2->Add(water_tex_n);
	water_mat2->Add(water_tex_r);
	for (int i = 0; i < 200; i++)
	{
		Actor box(bmesh, water_mat2);
		box.m_transform->GetPos().z = rand() % 500+300;
		box.m_transform->GetPos().x = rand() % 500+300;
		box.m_transform->GetScale().y *= 20;
		box.m_transform->GetScale().z *= 20;
		box.m_transform->GetRot().y = rand() % 20 * PI/2.f;
		box.m_transform->QUpdate();
		box.m_transform->GetPos().y +=7;
		m_terrain.push_back(box);
	}
	//bmesh = std::make_shared<Mesh>("./res/MESHS/test_home_02.obj");
	//Actor home(bmesh, water_mat);
	//m_terrain.push_back(home);

	for (int i = 0; i < -87; i++)
	{
		Light *l = new PointLight();
		l->m_position.z =  300.f * (rand() % 100 / 100.)-30.f;
		l->m_position.x = 330.f * (rand() % 100 / 100.);
		l->m_position.y = 12.f;
		l->m_ads = glm::vec3(.4, 0.9, 1.0);
		l->m_color = glm::vec4((rand()%100)/100., (rand() % 100) / 100., (rand() % 100) / 100.,1.0);
		l->m_color += glm::vec4(0.5, 0.5, 0.5, 0.0);
		l->m_clq = glm::vec3(.9, 0.009, 0.0000032);
		m_lights.push_back(l);
	}
#if 1
	Light *l = new DirectionalLight();
	l->m_ads = glm::vec3(0.15, 0.1,.0);
	l->m_position.w = 1.0f;
	l->m_position.y = 41.0f;
	l->m_position.z = 1.0f;
	l->m_color = glm::vec4(0.15, 0.15, 0.15, 0.0);
	m_lights.push_back(l);
	l = new DirectionalLight();
	l->m_ads = glm::vec3(0.15, 0.1, .0);
	l->m_position.w = 1.0f;
	l->m_position.y = 1.0f;
	l->m_position.z = -1.0f;
	l->m_color = glm::vec4(0.15, 0.15, 0.15, 0.0);
	m_lights.push_back(l);
	l = new DirectionalLight();
	l->m_ads = glm::vec3(0.15, 0.1, .0);
	l->m_position.w = 1.0f;
	l->m_position.y = 1.0f;
	l->m_position.x= -1.0f;
	l->m_color = glm::vec4(0.15, 0.15, 0.15, 0.0);
	m_lights.push_back(l);
#endif 
	//delete l;


}
