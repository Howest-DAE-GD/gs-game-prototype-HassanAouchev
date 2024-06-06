#pragma once
#include "BaseGame.h"
#include "utils.h"
#include "vector"
#include "Hero.h"
#include "Enemy.h"
#include <Texture.h>
class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void ChangeCameraLocation(float elapsedSec,Point2f playerPosition);
	void GameReset(bool reset);
	std::vector<Point2f> m_Ground{
									Point2f {843.f,50.f},
									Point2f {0.f,50.f},

									};
	std::vector<Point2f> m_Position;
	
	Hero* m_pHero;
	std::vector<Enemy*> m_pEnemy; 
	Point2f m_CameraLocation{ Point2f{421.5f,250.f} };
	Texture* m_Font{};
};