#include "pch.h"
#include "Game.h"
#include <iostream>

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_Font = new Texture{ "eVade the Minions while you make your wqy out of the Villain's tower","Impostograph-Regular.ttf",20,Color4f{0.f,0.f,0.f,0.0f} };
	float randomEnemyHeight{50.f};
	float MapLength{50.f};
	m_pHero = new Hero(Point2f{ 20.f, 75.f });
	for (int index{}; randomEnemyHeight<6000.f; index++)
	{
		int randomEnemyHeightAdd{ rand() % 150 + 150 };
		randomEnemyHeight += randomEnemyHeightAdd;
		m_pEnemy.push_back(new Enemy(Point2f{ float(rand() % 500 + 150),float(randomEnemyHeight) }, float(rand() % 20 + 40)));
	}
	for (int index{}; MapLength<6000.f; index++)
	{
		int randomWidth{ (rand() % 50) + 100 };
		int randomLength{ rand() % 100 + 100 };
		int randomHeight{ rand() % 150 + 150 };
		for (int index1{}; index1 < 4; index1++)
		 {
			if (index1 < 2)
			{
				if (index1 == 0)
				{
					m_Ground.push_back(Point2f{ 0.f,MapLength + randomHeight });
					
				}
				else
				{
					m_Ground.push_back(Point2f{ 0.f + randomLength,MapLength + randomHeight });
					
				}

			}
			else
			{
				if(index1==2)
				{
					m_Ground.push_back(Point2f{ 0.f + randomLength,MapLength + randomHeight+randomWidth });
				}
				else
				{
					m_Ground.push_back(Point2f{ 0.f,MapLength + randomHeight + randomWidth });
					MapLength += (randomLength+randomWidth);
				}
			}

		}
	}
	m_Ground.push_back(Point2f{ 843.f,MapLength });
	for (int index{}; MapLength >50.f ; index++)
	{
		int randomWidth{(rand()%50)+100};
		int randomLength{ rand()%100+100 };
		int randomHeight{rand()%150+150};
		for (int index1{}; index1 < 4; index1++)
		 {
			if (index1 < 2)
			{
				if (index1 == 0)
				{
					m_Ground.push_back(Point2f{ 843.f,MapLength - randomHeight });
					
				}
				else
				{
					m_Ground.push_back(Point2f{ 843.f - randomLength,MapLength-  randomHeight });
					
				}

			}
			else
			{
				if(index1==2)
				{
					m_Ground.push_back(Point2f{ 843.f - randomLength,MapLength - randomHeight-randomWidth });
				}
				else
				{
					m_Ground.push_back(Point2f{ 843.f,MapLength - randomHeight - randomWidth });
					MapLength -= (randomLength+randomWidth);
				}
			}

		}
	}

	m_pHero->SetCollision(m_Ground);
	for	(Enemy* enemy:m_pEnemy)
	{
		enemy->SetCollision(m_Ground);
	}

}

void Game::Cleanup( )
{
	delete m_pHero;
	m_pHero = nullptr;
	for (Enemy* enemy : m_pEnemy)
	{
		delete enemy;
	}
	m_pEnemy.clear();
}

void Game::Update( float elapsedSec )
{

	GameReset(utils::IsOverlapping(Rectf{ 0,0.f + 1000.f *5 ,846.f,500.f }, m_pHero->GetHitBoxCharacter()));
	GameReset(m_pHero->GetReset());
	// Check keyboard state
	m_pHero->update(elapsedSec);
	//std::cout << m_CameraLocation.y<< std::endl;
	ChangeCameraLocation(elapsedSec, m_pHero->GetCenterPoint());
	//std::cout << m_pHero->GetCenterPoint().x << " " << m_CameraLocation.x<< std::endl;
	for (Enemy* enemy : m_pEnemy)
	{
		enemy->Update(elapsedSec);
		m_pHero->SetIsHit(enemy->GetHitBoxAttack());
		enemy->SetIsHit(m_pHero->GetHitBoxAttack());
		enemy->SetCenterPointHero(m_pHero->GetCenterPoint());
	}
}

void Game::Draw( ) const
{
	ClearBackground( );
	glPushMatrix();
	glTranslatef(-m_CameraLocation.x+423.f,-m_CameraLocation.y+250.f,0);
	
	for (int index{}; index <5 ; ++index)
	{
		utils::SetColor(Color4f(0.7f, 0.7f, 0.7f, 1.f));
		utils::FillRect(Rectf{ 0,500.f+1000.f*index,846.f,500.f});
		utils::SetColor(Color4f{ 1.f,1.f,1.f,1.f });
		utils::FillRect(Rectf{ 0,0 + 1000.f * index,846.f,500.f });
	}
	utils::SetColor(Color4f(0.f, 1.f, 0.f, 1.f));
	utils::FillRect(Rectf{ 0,0.f + 1000.f*5,846.f,500.f });
	
	utils::SetColor(Color4f{ 1.f,0.f,0.f,1.f });
	utils::DrawPolygon(m_Ground);
	m_pHero->Draw();
	for (Enemy* enemy : m_pEnemy)
	{
		enemy->Draw();
	}
	m_Font->Draw(Rectf{ 150.f,200.f,500.f,20.f });
	glPopMatrix();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	m_pHero->SetMousePos(float(e.x), float(e.y));
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{

	//std::cout << "MOUSEBUTTONDOWN event: ";
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		m_pHero->SetIsAttacking();
		break;
	case SDL_BUTTON_RIGHT:
		m_pHero->SetIsSpecialAtt();
		break;
	case SDL_BUTTON_MIDDLE:
		
		break;
	}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::ChangeCameraLocation(float elapsedSec, Point2f playerPosition)
{
	//if (playerPosition.x > 423.f && playerPosition.x < 6000.f - 430.f)
	//{
	//	if (m_CameraLocation.x < playerPosition.x - 25.f)
	//		m_CameraLocation.x = playerPosition.x - 25.f;
	//	else if (m_CameraLocation.x < playerPosition.x - 10.f)
	//		m_CameraLocation.x += 200.f * 1 / 6 * elapsedSec;
	//	else if (m_CameraLocation.x > playerPosition.x + 25.f)
	//		m_CameraLocation.x = playerPosition.x + 25.f;
	//	else if (m_CameraLocation.x > playerPosition.x + 10.f)
	//		m_CameraLocation.x -= 200.f * 1 / 6 * elapsedSec;
	//}
	if(playerPosition.y>250.f)
	{
		if (m_CameraLocation.y < playerPosition.y - 25.f)
			m_CameraLocation.y = playerPosition.y - 25.f;
		else if (m_CameraLocation.y < playerPosition.y - 5.f)
			m_CameraLocation.y += 200.f * 3 / 4 * elapsedSec;
		else if (m_CameraLocation.y > playerPosition.y + 25.f)
			m_CameraLocation.y = playerPosition.y + 25.f;
		else if (m_CameraLocation.y > playerPosition.y + 5.f)
			m_CameraLocation.y -= 200.f * 3 / 4 * elapsedSec;
	}
}

void Game::GameReset(bool reset)
{
	if(reset)
	{
		float randomEnemyHeight{ 50.f };
		float MapLength{ 50.f };
		delete m_pHero;
		m_pHero = nullptr;
		for (Enemy* enemy : m_pEnemy)
		{
			delete enemy;
		}
		m_pEnemy.clear();
		m_Ground.clear();
		m_Ground.push_back(Point2f{ 843.f,50.f });
		m_Ground.push_back(Point2f{ 0.f,50.f });

		m_pHero = new Hero(Point2f{ 20.f, 75.f });
		for (int index{}; randomEnemyHeight < 6000.f; index++)
		{
			int randomEnemyHeightAdd{ rand() % 150 + 150 };
			randomEnemyHeight += randomEnemyHeightAdd;
			m_pEnemy.push_back(new Enemy(Point2f{ float(rand() % 500 + 150),float(randomEnemyHeight) }, float(rand() % 20 + 40)));
		}
		for (int index{}; MapLength < 6000.f; index++)
		{
			int randomWidth{ (rand() % 50) + 100 };
			int randomLength{ rand() % 100 + 100 };
			int randomHeight{ rand() % 150 + 150 };
			for (int index1{}; index1 < 4; index1++)
			{
				if (index1 < 2)
				{
					if (index1 == 0)
					{
						m_Ground.push_back(Point2f{ 0.f,MapLength + randomHeight });

					}
					else
					{
						m_Ground.push_back(Point2f{ 0.f + randomLength,MapLength + randomHeight });

					}

				}
				else
				{
					if (index1 == 2)
					{
						m_Ground.push_back(Point2f{ 0.f + randomLength,MapLength + randomHeight + randomWidth });
					}
					else
					{
						m_Ground.push_back(Point2f{ 0.f,MapLength + randomHeight + randomWidth });
						MapLength += (randomLength + randomWidth);
					}
				}

			}
		}
		m_Ground.push_back(Point2f{ 843.f,MapLength });
		for (int index{}; MapLength > 50.f; index++)
		{
			int randomWidth{ (rand() % 50) + 100 };
			int randomLength{ rand() % 100 + 100 };
			int randomHeight{ rand() % 150 + 150 };
			for (int index1{}; index1 < 4; index1++)
			{
				if (index1 < 2)
				{
					if (index1 == 0)
					{
						m_Ground.push_back(Point2f{ 843.f,MapLength - randomHeight });

					}
					else
					{
						m_Ground.push_back(Point2f{ 843.f - randomLength,MapLength - randomHeight });

					}

				}
				else
				{
					if (index1 == 2)
					{
						m_Ground.push_back(Point2f{ 843.f - randomLength,MapLength - randomHeight - randomWidth });
					}
					else
					{
						m_Ground.push_back(Point2f{ 843.f,MapLength - randomHeight - randomWidth });
						MapLength -= (randomLength + randomWidth);
					}
				}

			}
		}

		m_pHero->SetCollision(m_Ground);
		for (Enemy* enemy : m_pEnemy)
		{
			enemy->SetCollision(m_Ground);
		}
	}
	m_CameraLocation= Point2f{421.5f,250.f} ;

}
