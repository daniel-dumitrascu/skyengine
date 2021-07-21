#include "GameRectangle.h"
#include "video/Painter.h"
#include "shaders/ShaderDefines.h"
#include "keys/ResourceKeyCollection.h"
#include "shaders/ShaderResources.h"
#include "shaders/ShaderLoader.h"
#include "global/GlobalPaths.h"
#include "../global/GlobalData.h"
#include "math/Geometry.h"
#include "utils/UniqueGenerator.h"


GameRectangle::GameRectangle(Rectangle& rect, float posX, float posY, GameObjectLabel& gameLabel) :
	GameObject(nullptr, nullptr, -1, posX, posX, 1, gameLabel, INPUT_HANDLE_PROFILE_NONE)
{
	m_rectangle = new Rectangle(rect);		 
	m_wireframe = Geometry::ConstrWireframeFromRect(*m_rectangle);

	// Once we have a rect we can now calculate the size of that rect (height and lenght)
	vec_2x rectDimensions = Geometry::CalculateRectDimensions(*m_rectangle);
	m_rectHeight = rectDimensions.elem[0];
	m_rectLenght = rectDimensions.elem[1];

	Init();
}

GameRectangle::GameRectangle(Rectangle& rect, float posX, float posY, const vec_4x& color, GameObjectLabel& gameLabel) :
	GameObject(nullptr, nullptr, -1, posX, posY, 1, gameLabel, INPUT_HANDLE_PROFILE_NONE)
{
	m_rectangle = new Rectangle(rect);
	m_wireframe = Geometry::ConstrWireframeFromRect(*m_rectangle);

	// Once we have a rect we can now calculate the size of that rect (height and lenght)
	vec_2x rectDimensions = Geometry::CalculateRectDimensions(*m_rectangle);
	m_rectHeight = rectDimensions.elem[0];
	m_rectLenght = rectDimensions.elem[1];

	Init();
	SetColor(color);
}

GameRectangle::~GameRectangle()
{
	Painter::RemoveGeometryFromGPU(m_wireframe);
	delete m_wireframe;
}

std::unique_ptr<GameObject> GameRectangle::Clone()
{
	GameObjectLabel newlabel(m_gameLabel.GetType(), UniqueGenerator::Instance().GenerateUniqueID());
	return std::make_unique<GameRectangle>(*m_rectangle, GetPosition().elem[0], GetPosition().elem[1], newlabel);
}

void GameRectangle::Init()
{
	// Construct a world-projection matrix 
	matrix::game_matrix::WorldProjMatrix(m_wp_matrix, m_world_matrix, proj_matrix);

	// Select the key for the right shader
	ResourceKeyCollection* resourceKeyCollection = ResourceKeyCollection::GetInstance();

	// Do we have this resource loaded?
	// If not, we need to load it
	m_shader = ShaderRes::GetInstance()->RetriveProgramID(SHADER_ID_PRIMITIVE);
	if (!m_shader)
	{
		ShaderRes::GetInstance()->AddShaderPair((ShaderPair*)ShaderLoader::GetInstance()->Load(shaders_path + resourceKeyCollection->GetNameByKey(KEY_SHADER, SHADER_ID_PRIMITIVE)),
			SHADER_ID_PRIMITIVE);

		m_shader = ShaderRes::GetInstance()->RetriveProgramID(SHADER_ID_PRIMITIVE);
	}

	Painter::SendGeometryToGPU(m_wireframe);
	Painter::EnableMeshLocation(Painter::GetInstance()->GetMeshLocation());
}

void GameRectangle::PreDraw()
{
	Painter::GetInstance()->SelectShader(m_shader);
}

void GameRectangle::Draw()
{
	// Select data buffers
	Painter::SelectArrayBuffer(m_wireframe->GetVBOVert());
	Painter::SelectIndxBuffer(m_wireframe->GetVBOIndx());

	// Set from where to start the drawing
	Painter::SetDrawColorData(Painter::GetInstance()->GetColorLocation(), (const float*)&(m_color.elem[0]));

	// Set mesh drawing
	Painter::SetStartDrawData(Painter::GetInstance()->GetMeshLocation(), sizeof(Vertex), 0);

	// Set matrix drawing data
	Painter::SetDrawMatrix4Data(Painter::GetInstance()->GetMemoryLocation(), (const float*)&(m_wp_matrix.elem[0]));

	Painter::DrawData(m_wireframe->GetGeometryCount());
}

void GameRectangle::Update()
{
	/* Reset m_wp matrix */
	matrix::matrix_4x::SetIdentity(m_wp_matrix);

	/* Construct a world-projection matrix */
	matrix::game_matrix::WorldProjMatrix(m_wp_matrix, m_world_matrix, proj_matrix);
}

void GameRectangle::SetColor(const vec_4x& color)
{
	vector::vector_4x::SetVector(m_color, color.elem[0], color.elem[1], color.elem[2], color.elem[3]);
}