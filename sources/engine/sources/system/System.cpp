#include "System.h"
#include "global/GlobalPaths.h"
#include "utils/Utils.h"
#include "global/GlobalData.h"


Application*  System::m_app = NULL;


System* System::GetSystem()
{
	static System m_instance;
	return &m_instance;
}

void System::SetApplication(Application* const app, const std::string title)
{ 
	m_app = app;
	if (m_app == nullptr)
	{
		log.message("Application could not be initiated.", Logging::MSG_ERROR);
		exit(1);
	}

	m_title = utils::str::Strfmt("%s - built on %s %s, version 0.001", title.c_str(), __TIME__, __DATE__);
}

System::System() : m_title(""), m_driver(NULL), log("System")
{
	ConstrSystemGlobals();
}

void System::Init() 
{ 
	m_driver = new Driver;
	if (m_driver == nullptr)
	{
		log.message("System driver could not be initiated.", Logging::MSG_ERROR);
		exit(1);
	}

	m_driver->InitDriver();
	m_app->Init();
}

void System::Run() 
{ 
	global_timer.Start();

	int updates = 0, frames = 0;
	signed long long timer = CoreTime::GetTime();

	while (m_driver->IsRunning() && m_app->IsRunning())
	{
		if (global_timer.IsExceeded())
		{
			m_driver->Update();    /* update the platform interactions and graphics settings */
			m_app->Update();	   /* update a game frame */
			m_app->Draw();	       /* draw a game frame */
			m_driver->Render();    /* render the frame */

			updates++;
		}

		frames++;

		//TODO debug
		if ((CoreTime::GetTime() - timer) > SECOND_IN_MILISEC)
		{
			timer += SECOND_IN_MILISEC;
			//log.message(std::string("Rendered frames per second: ") + std::to_string(updates) + std::string("    Total frames per second: ") + std::to_string(frames), Logging::MSG_DEBUG);
			updates = 0;
			frames = 0;
		}
	} 
}

void System::Close()
{ 
	m_app->Close(); 
	CleanUp();
}

bool System::CleanUp()
{
	delete m_driver;
	m_driver = NULL;

	return true;
}

void System::ConstrSystemGlobals()
{
	utils::path::GetWorkingDir(working_dir_path);

	levels_path		= working_dir_path + "/resources/levels/";
	models_path		= working_dir_path + "/resources/models/";
	meshes_path		= working_dir_path + "/resources/meshes/";
	textures_path	= working_dir_path + "/resources/textures/PNG_files/";
	animation_path	= working_dir_path + "/resources/animations/";
	shaders_path	= working_dir_path + "/resources/shaders/";
}