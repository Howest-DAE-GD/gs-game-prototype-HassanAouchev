#include "pch.h"
#include "Hero.h"
#include <iostream>
#include <math.h>

Hero::Hero(Point2f pos) :
	m_Character{ Rectf{pos.x,pos.y,40.f,80.f} },
	m_Position{ pos },
	m_isHeroAttacking{},
	m_IsJumping{},
	m_IsLeft{},
	m_IsRight{},
	m_IsUp{},
	m_IsDown{},
	m_CanJump{ true },
	m_IsAttacking{},
	m_IsHit{},
	m_Direction{ Direction::neutral },
	m_Sword{},
	m_AccummelatedTimeAttack{},
	m_IsFacingRight{ true },
	m_AttackDelay{},
	m_MouseX{},
	m_MouseY{},
	m_MouseFollow{},
	m_CenterPoint{},
	m_LineLength{},
	m_IsSpecialAtt{},
	m_LineWidth{15.f},
	m_CameraLocation{ Point2f{0.f,250.f} },
	m_StopLoop{},
	m_CanDoubleJump{},
	m_DelayDoubleJump{0},
	m_IsSpace{},
	m_DashCharges{5},
	m_rechargeDashes{},
	m_DirectionPoint{},
	m_Reset{},
	m_deathDelay{}
{
}

void Hero::update(float elapsedSec)
{
	if (m_IsHit )
	{
		m_deathDelay += elapsedSec;
		if (m_deathDelay > 1.f && m_Velocity.y == 0)
		{
			m_Reset = true;
			m_deathDelay = 0;
		}
	}
	if (m_CanJump == false&&!m_CanDoubleJump&&m_DashCharges>0)
	{
		m_DelayDoubleJump += elapsedSec;
		if (m_DelayDoubleJump >= 0.5f)
		{
			m_CanDoubleJump = true;
			m_DelayDoubleJump = 0;
		}
	}
	if (m_DashCharges < 5)
	{
		m_rechargeDashes += elapsedSec;
			if(m_rechargeDashes>1.5f)
			{
				++m_DashCharges;
				m_rechargeDashes = 0;
			}
	}

	if (m_Position.x > 843.f&& !(m_Position.x > 6000.f-843.f))
		m_CameraLocation.x = m_Position.x;
	if (m_Position.y > 500.f && !(m_Position.y > 3000.f - 500.f))
		m_CameraLocation.y = m_Position.y;
	utils::HitInfo hitinfoWall{};
	utils::Raycast(m_Collision, Point2f{m_Position.x,m_Position.y+10.f}, Point2f{m_Position.x + m_Character.width,m_Position.y+10.f}, hitinfoWall);
	if (m_IsAttacking || m_AttackDelay)m_AccummelatedTimeAttack += elapsedSec;
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	m_IsLeft = pStates[SDL_SCANCODE_LEFT];
	m_IsRight = pStates[SDL_SCANCODE_RIGHT];
	m_IsUp = pStates[SDL_SCANCODE_UP];
	m_IsDown = pStates[SDL_SCANCODE_DOWN];
	m_IsSpace = pStates[SDL_SCANCODE_SPACE];

	//std::cout << m_CenterPoint.y << std::endl;
	if (m_IsUp)
	{
		if (m_IsRight)
		{
			m_DirectionPoint = Point2f{ m_CenterPoint.x + 50,m_CenterPoint.y + 50 };
		}
		else if (m_IsLeft)
		{
			m_DirectionPoint = Point2f{ m_CenterPoint.x - 50,m_CenterPoint.y + 50 };
		}
		else
			m_DirectionPoint = Point2f{ m_CenterPoint.x,m_CenterPoint.y + float(sqrt(5000)) };
	}
	else if (m_IsDown)
	{
		if (m_IsRight)
		{
			m_DirectionPoint = Point2f{ m_CenterPoint.x + 50,m_CenterPoint.y - 50 };
		}
		else if (m_IsLeft)
		{
			m_DirectionPoint = Point2f{ m_CenterPoint.x - 50,m_CenterPoint.y - 50 };
		}
		else
			m_DirectionPoint = Point2f{ m_CenterPoint.x,m_CenterPoint.y - float(sqrt(5000)) };
	}
	else if (m_IsRight)
	{
		m_DirectionPoint = Point2f{ m_CenterPoint.x + float(sqrt(5000)) ,m_CenterPoint.y};
	}
	else if (m_IsLeft)
	{
		m_DirectionPoint = Point2f{ m_CenterPoint.x - float(sqrt(5000)),m_CenterPoint.y };
	}
	else m_DirectionPoint = m_CenterPoint;
	if (m_IsSpace&&m_IsUp&&m_CanJump&&!m_CanDoubleJump) m_Direction = Direction::up;
	else if (m_IsRight&&!m_CanDoubleJump) m_Direction = Direction::right;
	else if (m_IsLeft&&!m_CanDoubleJump) m_Direction = Direction::left;
	else if (m_IsDown&&!m_CanDoubleJump) m_Direction = Direction::down;
	else if (m_IsSpace&&m_IsUp && !m_StopLoop&&m_CanDoubleJump)
	{
		--m_DashCharges;
		m_CanDoubleJump = false;
		m_DelayDoubleJump = 0;
		m_Velocity.y = 400.f;
		if (m_IsRight) 
		{
			m_Velocity.x = 200.f;
		}
		else if(m_IsLeft)  m_Velocity.x = -200.f;
	}
	else if (m_IsSpace &&m_IsDown && !m_StopLoop && m_CanDoubleJump)
	{
		--m_DashCharges;
		m_CanDoubleJump = false;
		m_DelayDoubleJump = 0;
		m_Velocity.y = -400.f;
		if (m_IsRight)
		{
			m_Velocity.x = 200.f;
		}
		else if (m_IsSpace&&m_IsLeft)  m_Velocity.x = -200.f;
		
	}
	else if(m_IsSpace&&m_IsLeft && m_CanDoubleJump && !m_StopLoop)
	{
		--m_DashCharges;
		m_DelayDoubleJump = 0;
		m_CanDoubleJump = false;
		m_Velocity.x = -400.f;
	}
	else if (m_IsSpace&&m_IsRight && m_CanDoubleJump && !m_StopLoop)
	{
		m_CanDoubleJump = false;
		m_DelayDoubleJump = 0;
		--m_DashCharges;
		m_Velocity.x = 400.f;
	}
	else m_Direction = Direction::neutral;
	if (m_IsHit)
	{
		m_StopLoop = true;
		m_Direction = Direction::neutral;
	}
	switch (m_Direction)
	{
	case Hero::Direction::neutral:
		if (m_Velocity.x < -10.f)m_Velocity.x += 200.f * 3.f / 2.f * elapsedSec;
		else if (m_Velocity.x > 10.f)m_Velocity.x -= 200.f * 3.f / 2.f * elapsedSec;
		else m_Velocity.x = 0;
		break;
	case Hero::Direction::up:
		
	{
		//m_CanDoubleJump = false;
		//m_CanJump = false;
		m_Velocity.y = 400.f;
	}
		break;
	case Hero::Direction::left:
		m_IsFacingRight = false;
		if (m_Velocity.x > -205.f)
		m_Velocity.x = -200.f;
		else
			m_Velocity.x += 200.f * 3.f / 2.f * elapsedSec;
		break;
	case Hero::Direction::right:
		m_IsFacingRight = true;
		if (m_Velocity.x < 205.f)
		m_Velocity.x = 200.f;
		else
		m_Velocity.x -= 200.f * 3.f / 2.f * elapsedSec;

		break;
	case Hero::Direction::down:
		break;
	default:
		break;
	}
	utils::HitInfo hitinfoGravity{};
	utils::Raycast( m_Collision,Point2f{ m_Position.x+5.f, m_Position.y + 30.f }, Point2f{ m_Position.x+5.f,m_Position.y }, hitinfoGravity);
	utils::HitInfo hitinfoCeiling{};
	utils::Raycast(m_Collision, Point2f{ m_Position.x + 5.f, m_Position.y +35.f }, Point2f{ m_Position.x + 5.f,m_Position.y+m_Character.height }, hitinfoCeiling);

	if (m_IsAttacking)
	{
		m_AttackDelay = true;
		m_Velocity.x -= m_Velocity.x * 2 * elapsedSec;
		if (m_IsFacingRight)m_Sword = Rectf{ m_Position.x + m_Character.width,m_Position.y + m_Character.height /2.f,60.f,20.f };
		else m_Sword = Rectf{ m_Position.x - 60.f,m_Position.y + m_Character.height /2.f,60.f,20.f };
		if (m_AccummelatedTimeAttack > 0.5f)
		{
			m_IsAttacking = false;
		}
	}
	else m_Sword = Rectf{ -500.f,-500.f,0,0 };
	if (m_AccummelatedTimeAttack > 1.f) 
	{
		m_AttackDelay = false;
		m_AccummelatedTimeAttack = 0.f;
	}
	//std::cout << hitinfoWall.normal.x << std::endl;
	if (hitinfoWall.normal.x <= -0.9f)
	{
		m_Position.x = hitinfoWall.intersectPoint.x;
	}
	else if (hitinfoWall.normal.x >= 0.9f)
	{
		m_Position.x = hitinfoWall.intersectPoint.x - m_Character.width;
		m_IsSpecialAtt = false;
		m_LineWidth = 15.f;
	}
	std::cout << hitinfoCeiling.normal << std::endl;
	if (hitinfoCeiling.normal.y == 1.f)
	{
		
		m_Velocity.y = 0;
		m_CanDoubleJump = false;
		m_Position.y = hitinfoCeiling.intersectPoint.y-m_Character.height-1.f;
	}
	if (hitinfoGravity.normal.y == -1.f&&m_Direction!=Direction::up)
	{
		if (m_DashCharges < 5)
		{
			m_rechargeDashes += elapsedSec;
		}
		m_DelayDoubleJump = 0;
		m_Velocity.y = 0;
		m_CanJump = true;
		m_StopLoop = false;
		m_CanDoubleJump = false;
		m_Position.y = hitinfoGravity.intersectPoint.y;
	}
	else if (hitinfoGravity.normal.y != 1.f)
	{
		//if (!m_CanDoubleJump)
		if(m_CanJump)
		m_CanJump = false;
		m_Velocity.y -= 200.f * 5.f / 2.f * elapsedSec;
	}
	if(!m_IsHit)m_Character = Rectf{m_Position.x,m_Position.y,20.f,40.f};
	else m_Character = Rectf{ m_Position.x,m_Position.y,40.f,20.f };

	m_CenterPoint = Point2f{ m_Position.x + m_Character.width / 2.f,m_Position.y + m_Character.height / 2.f };
	if ( Vector2f{ m_CenterPoint,Point2f{m_MouseX,m_MouseY} }.Length() > 350.f)m_LineLength =350.f;
	else m_LineLength = Vector2f{ m_CenterPoint,Point2f{m_MouseX,m_MouseY} }.Length();
	if(!m_IsSpecialAtt)m_MouseFollow = Point2f{ Vector2f{m_CenterPoint,Point2f{m_MouseX,m_MouseY} }.Normalized() * m_LineLength + Vector2f{ m_CenterPoint} };

	if (m_IsSpecialAtt)
	{
		if (m_LineWidth > 1.f)m_LineWidth -= 15.f*elapsedSec;
		else
		{
			if (m_CenterPoint.x < m_MouseFollow.x - 10 || m_CenterPoint.x > m_MouseFollow.x + 10 || m_CenterPoint.y < m_MouseFollow.y - 10 || m_CenterPoint.y > m_MouseFollow.y + 10)
				m_Velocity = Vector2f{ m_MouseFollow.x - m_CenterPoint.x,m_MouseFollow.y - m_CenterPoint.y }.Normalized() * 500.f;
			else 
			{
				m_IsSpecialAtt = false;
				m_LineWidth = 15.f;
			}
		}
		

	}

	m_Position.x += m_Velocity.x * elapsedSec;
	m_Position.y += m_Velocity.y * elapsedSec;
}

void Hero::SetIsHit(const Rectf& hitbox)
{
	if (utils::IsOverlapping(m_Character, hitbox))m_IsHit=true;
}

void Hero::Draw() const
{
	utils::SetColor(Color4f{ 0.5f,0.5f,0.5f,1.f });
	utils::FillRect(Rectf{ m_Character.left - 57.f,m_Character.bottom + m_Character.height - 2.f + 5.f,28.f * 5,14.f });
	for (int index{}; index < 5; index++)
	{
		utils::SetColor(Color4f{ 0.f,0.f,0.f,1.f });
		utils::FillRect(Rectf{ m_Character.left - 56.f + 28.f * index,m_Character.bottom + m_Character.height - 1.f + 5.f,27.f,12.f });
	}
	for (int index{}; index < m_DashCharges; index++)
	{
		utils::SetColor(Color4f{ 0.7f,.7f,0.3f,1.f });
		utils::FillRect(Rectf{ m_Character.left - 55.f+28.f*index,m_Character.bottom + m_Character.height + 5.f,25.f,10.f });
	}
	utils::SetColor(Color4f{ 0.f,1.f,0.f,1.f });
	utils::DrawLine(m_CenterPoint, m_DirectionPoint,3.f);
	utils::SetColor(Color4f{ 0.f,0.f,0.f,1.f });
	utils::FillRect(Rectf{ m_Character.left-2.f,m_Character.bottom-2.f,m_Character.width+4.f,m_Character.height+4.f });
	utils::SetColor(Color4f{ 1.f,0.5f,0.f,1.f });
	if (m_IsAttacking)
		utils::FillRect(m_Sword);

	if(m_IsHit)utils::SetColor(Color4f{ 0.6f,0.2f,0.2f,1.f });
	else utils::SetColor(Color4f{ 1.f,1.f,1.f,1.f });
	utils::FillRect(m_Character);
	utils::SetColor(Color4f{ 1.f,0.0f,0.0f,1.f });
	//utils::DrawLine(m_CenterPoint, m_MouseFollow,m_LineWidth);
}

void Hero::SetCollision(const std::vector<Point2f>& collision)
{
	m_Collision = collision;
}

Rectf Hero::GetHitBoxAttack() const
{
	return m_Sword;
}

Point2f Hero::GetCenterPoint() const
{
	return m_CenterPoint;
}

Point2f Hero::GetCameraLocation() const
{
	return m_CameraLocation;
}

void Hero::SetIsAttacking()
{
	
	m_IsAttacking = true;
}

void Hero::SetIsSpecialAtt()
{
	m_IsSpecialAtt = true;
}

void Hero::SetMousePos(float x, float y)
{
	m_MouseX = x;
	m_MouseY = y;
}

bool Hero::GetReset()
{
	return m_Reset;
}

Rectf Hero::GetHitBoxCharacter()const
{
	return m_Character;
}
