#include "GameStateKickstart.h"
#include "geometry/mesh/MeshLoader.h"
#include "geometry/mesh/MeshResources.h"
#include "texture/TextureLoader.h"
#include "texture/TextureResources.h"
#include "shaders/ShaderLoader.h"
#include "shaders/ShaderResources.h"
#include "global/GlobalPaths.h"
#include "global/GlobalData.h"
#include "GameStateMainMenu.h"
#include "video/Painter.h"
#include "GameStateManager.h"
#include "settings/SettingsLoader.h"


GameStateKickstart::GameStateKickstart() : is_done_loading(false),
							   m_splash_timer(0500)
{
	stateID = GAME_STATE_KICKSTART;
}

GameStateKickstart::~GameStateKickstart()
{

}

void GameStateKickstart::ResumeState()
{
	// Load the default resources
	std::string splashScreenMesh = SettingsLoader::GetInstance()->GetValue("defaultResources.splashScreen.mesh");
	std::string splashScreenTexture = SettingsLoader::GetInstance()->GetValue("defaultResources.splashScreen.texture");
	std::string splashScreenShader = SettingsLoader::GetInstance()->GetValue("defaultResources.splashScreen.shader");

	MeshRes::GetInstance()->Add((WireFrame*)MeshLoader::GetInstance()->Load(working_dir_path + "\\" + splashScreenMesh), splashScreenMesh);
	TexRes::GetInstance()->Add((Texture*)TexLoader::GetInstance()->Load(working_dir_path + "\\" + splashScreenTexture), splashScreenTexture);

	//TODO - this should be done directly into AddShaderPair
	std::unique_ptr<ShaderPair> shaderPair((ShaderPair*)ShaderLoader::GetInstance()->Load(working_dir_path + "\\" + splashScreenShader));
	ShaderRes::GetInstance()->AddShaderPair(shaderPair.get(), splashScreenShader);

	/***********************************************************************************************************************/
	unsigned int program_id = ShaderRes::GetInstance()->RetriveProgramID(splashScreenShader);

	// move them in DrawScheme
	// for every shader pair :)
	// I call them once in init and then send the info as mesh_id and tex_id to every object in the scene

	//TODO call them each time I need to draw objects 
	//the functions will be called to every object group from the scenne that is assigned to a shader pair
	Painter::GetInstance()->SelectShader(program_id);
	Painter::SetActiveTexture(program_id, "s_texture");

	Painter::EnableMeshLocation(Painter::GetInstance()->GetMeshLocation());
	Painter::EnableTextureLocation(Painter::GetInstance()->GetTexLocation());
	/***********************************************************************************************************************/

	m_splash_timer.Start();
}

void GameStateKickstart::SuspendState()
{

}

void GameStateKickstart::UpdateState()
{
	if (!m_splash_timer.IsExceeded() || !is_done_loading)
	{
		if (m_splash_screen)
		    m_splash_screen->Update();
		else
		{
			MeshLoader* mesh_loader = MeshLoader::GetInstance();
			MeshRes* mesh_res = MeshRes::GetInstance();
			TexLoader* tex_loader = TexLoader::GetInstance();
			TexRes* tex_res = TexRes::GetInstance();
			ShaderRes* shader_res = ShaderRes::GetInstance();

			/* Load the splash screen and menu resources */	
			std::string menuTexture = SettingsLoader::GetInstance()->GetValue("defaultResources.menu.texture");
			Texture* texture = (Texture*)tex_loader->Load(working_dir_path + "\\" + menuTexture);
			tex_res->Add(texture, menuTexture);
					
			std::string splashScreenShader = SettingsLoader::GetInstance()->GetValue("defaultResources.splashScreen.shader");
			unsigned int program_id = shader_res->RetriveProgramID(SettingsLoader::GetInstance()->GetValue("defaultResources.splashScreen.shader"));

			/* Get resources for the splashscreen */
			std::string menuMeshPath = SettingsLoader::GetInstance()->GetValue("defaultResources.menu.mesh");
			WireFrame* mesh = (WireFrame*)mesh_loader->Load(working_dir_path + "\\" + menuMeshPath);
			mesh_res->Add(mesh, menuMeshPath);

			m_splash_screen = std::make_unique<SplashScreen>(mesh, texture, program_id);
			m_splash_screen->Init();

			return;
		}

		/*TODO - here you can also load sounds, tex and other gameplay resources */

		is_done_loading = true;
	}
	else
	{
		GameStateManager::PushState(new GameStateMainMenu);
	}
}

void GameStateKickstart::RenderState()
{
	m_splash_screen->PreDraw();
	m_splash_screen->Draw();
}