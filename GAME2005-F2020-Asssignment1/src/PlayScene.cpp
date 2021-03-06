#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("background", 400, 300, 0, 255, true);
	drawDisplayList();

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

}

void PlayScene::update()
{
	if (m_pPlaneSprite->getAnimation("explosion").current_frame == 6){
		m_pPlaneSprite->getAnimation("explosion").current_frame = 0;
		m_pPlaneSprite->setAnimationState(PLANE_IDLE);
		m_pPlaneSprite->getRigidBody()->isColliding = false;		
	}

	std::string labelText = "";
	if (m_pPlaneSprite->isColliding(m_pBall)) {
		labelText = "HIT";
		m_pBall->getRigidBody()->isColliding = true;
		m_pPlaneSprite->setAnimationState(PLANE_EXPLOSION);
	}
	else {
		labelText = "Distance = " + std::to_string(m_pPlaneSprite->getDistance(m_pBall));
	}

	updateDisplayList();

	//m_pDistanceLabel->setText(labelText);
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();
	
	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/startBackground.png", "background");
	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	// Plane Sprite
	m_pPlaneSprite = new Plane();
	addChild(m_pPlaneSprite);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	////Label for Hit
	//const SDL_Color blue = { 0, 0, 255, 255 };
	//m_pDistanceLabel = new Label("Distance", "Consolas", 40, blue, glm::vec2(400.0f, 40.0f));
	//m_pDistanceLabel->setParent(this);
	//addChild(m_pDistanceLabel);

	//Ball
	m_pBall = new Target();
	addChild(m_pBall);

	// Back Button
	//m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	//m_pBackButton->getTransform()->position = glm::vec2(300.0f, 400.0f);
	//m_pBackButton->addEventListener(CLICK, [&]()-> void
	//{
	//	m_pBackButton->setActive(false);
	//	TheGame::Instance()->changeSceneState(START_SCENE);
	//});

	//m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	//{
	//	m_pBackButton->setAlpha(128);
	//});

	//m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	//{
	//	m_pBackButton->setAlpha(255);
	//});
	//addChild(m_pBackButton);

	//// Next Button
	//m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	//m_pNextButton->getTransform()->position = glm::vec2(500.0f, 400.0f);
	//m_pNextButton->addEventListener(CLICK, [&]()-> void
	//{
	//	m_pNextButton->setActive(false);
	//	TheGame::Instance()->changeSceneState(END_SCENE);
	//});

	//m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	//{
	//	m_pNextButton->setAlpha(128);
	//});

	//m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	//{
	//	m_pNextButton->setAlpha(255);
	//});

	//addChild(m_pNextButton);

	/* Instructions Label */
	const SDL_Color blue = { 0, 0, 255, 255 };
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas", 20, blue);
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	addChild(m_pInstructionsLabel);
}

void PlayScene::GUI_Function() const
{
	double velx = m_pBall->getRigidBody()->velocity.x,
		vely = m_pBall->getRigidBody()->velocity.y,
		vel = Util::magnitude(m_pBall->getRigidBody()->velocity),
		disx = m_pBall->getTransform()->position.x,
		disy = m_pBall->getTransform()->position.y,
		dis = m_pPlaneSprite->getDistance(m_pBall),
		maxdis = m_pBall->m_maxdis,
		accx = m_pBall->getRigidBody()->acceleration.x,
		accy = m_pBall->getRigidBody()->acceleration.y,
		acc = Util::magnitude(m_pBall->getRigidBody()->acceleration),
		force = m_pBall->m_force;
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Change Variables", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);
	if (!m_pBall->m_bThrow)
	{
		if (ImGui::Button("Throw"))
		{
			m_pBall->throwPos = m_pPlayer->getTransform()->position;
			m_pBall->doThrow();
			m_pBall->m_bThrow = true;
			m_pBall->m_maxdis = 0;
			maxdis = 0;
		}
		
	}
	else
		ImGui::Button("Wait");


	static float xPos = 485.0f;
	if (ImGui::SliderFloat("EnemyDistance", &xPos, 200, Config::SCREEN_WIDTH) - m_pPlaneSprite->getWidth()/ 2){
		m_pPlaneSprite->getTransform()->position = glm::vec2(xPos, 600.0f - 65 / 2);
		
	}
	ImGui::Separator();
	
	static float xThrowSpeed = 95.0f;
	if (ImGui::SliderFloat("Velocity", &xThrowSpeed, 0, 800)) {
		m_pBall->throwSpeed = xThrowSpeed;
	}

	static float angle = 15.0f;
	if (ImGui::SliderFloat("Angle", &angle, 0, 90)) {
		m_pBall->angle = angle;
	}

	static float mass = 2.2f;
	if (ImGui::SliderFloat("Mass", &mass, 0, 100)) {
		m_pBall->mass = mass;
	}
	static float gravity = m_pBall->gravity;
	if (ImGui::SliderFloat("Gravity", &gravity, 5.0f, 20)) {
		m_pBall->gravity = gravity;
	}
	
		
	ImGui::Text("Velocity on x-axis = %.2f m/s", velx);
	ImGui::Text("Velocity on y-axis = %.2f m/s", -vely);
	ImGui::Text("Velocity = %.2f m/s", vel);
	ImGui::Text("Distance between TD and ST %.2f m", dis);
	ImGui::Text("Maximum height of projectile %.2f m", maxdis);
	ImGui::Text("Acceleration on x-axis %.2f m/s�", accx);
	ImGui::Text("Acceleration on y-axis %.2f m/s�", accy);
	ImGui::Text("Acceleration %.2f m/s?", accy);
	ImGui::Text("Force %.2f N", force);

	/*static float xThrowSpeed = 0.0f;
	if (ImGui::SliderFloat("Radian", &xThrowSpeed, 0, 400)) {
		m_pBall->throwSpeed.x = xThrowSpeed;
	}*/

	//static float float3[3] = { 0.0f, 1.0f, 1.5f };
	//if(ImGui::SliderFloat3("My Slider", float3, 0.0f, 2.0f))
	//{
	//	std::cout << float3[0] << std::endl;
	//	std::cout << float3[1] << std::endl;
	//	std::cout << float3[2] << std::endl;
	//	std::cout << "---------------------------\n";
	//}
	
	ImGui::End();
	ImGui::EndFrame();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
