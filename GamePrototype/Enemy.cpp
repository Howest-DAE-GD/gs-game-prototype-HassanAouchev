#include "pch.h"
#include "Enemy.h"
#include <iostream>

Enemy::Enemy(Point2f pos, float size):
	m_Character{ Rectf{pos.x,pos.y,40.f,80.f} },
	m_Position{ pos },
	m_isEnemyAttacking{},
	m_IsJumping{},
	m_CanJump{ true },
	m_IsAttacking{},
	m_IsHit{},
	m_Direction{ Direction::neutral },
	m_Claws{},
	m_AccummelatedTimeAttack{},
	m_IsFacingRight{ true },
	m_AttackDelay{},
	m_Size{size},
	m_isDead{},
	m_IsSpecialAtt{},
	m_LineWidth{15.f},
	m_LineLength{500.f},
	m_CenterPointHeroX{},
	m_CenterPointHeroY{},
	m_IsLocking{},
	m_CenterPoint{pos},
	m_Delay{1.25f},
	m_IsLeft{},
	m_IsRight{}
{
}

void Enemy::Update(float elapsedSec)
{
	if (m_IsAttacking || m_AttackDelay)m_AccummelatedTimeAttack += elapsedSec;
	if(m_IsLocking)m_Delay -= elapsedSec;
	utils::HitInfo hitinfoGravity{};
	utils::HitInfo hitinfoWall{};
	utils::HitInfo hitinfoChargeAttack{};
	utils::Raycast(m_Collision, Point2f{m_Position.x,m_Position.y+10.f}, Point2f{ m_Position.x + m_Character.width,m_Position.y+10.f }, hitinfoWall);
	utils::Raycast(m_Collision, Point2f{ m_Position.x+5.f, m_Position.y + 30.f }, Point2f{ m_Position.x+5.f,m_Position.y }, hitinfoGravity);
	utils::Raycast(m_Collision, m_CenterPoint, Point2f{ m_CenterPointHeroX,m_CenterPointHeroY }, hitinfoChargeAttack);
	if(!m_IsSpecialAtt&&m_IsLocking&&m_CenterPoint.x < m_MouseFollow.x - 10 )
	{
		m_IsLeft = false;
		m_IsRight = true;

	}
	else if(!m_IsSpecialAtt&&m_IsLocking&&m_CenterPoint.x > m_MouseFollow.x + 10) 
	{
		m_IsRight = false;
		m_IsLeft = true;
	}
	else
	{
		m_IsLeft = false;
		m_IsRight = false;
	}

	if (m_IsRight) m_Direction = Direction::right;
	else if (m_IsLeft) m_Direction = Direction::left;
	else m_Direction = Direction::neutral;

	
	switch (m_Direction)
	{
	case Enemy::Direction::neutral:
		if (!m_IsSpecialAtt)
		{
			if (m_Velocity.x < -10.f)m_Velocity.x += 200.f * 3.f / 2.f * elapsedSec;
			else if (m_Velocity.x > 10.f)m_Velocity.x -= 200.f * 3.f / 2.f * elapsedSec;
			else
			{
				m_Velocity.x = 0.f;
			}

		}
		else 
		{
			m_Velocity.x = 0.f;
			m_Velocity.y = 0.f;
		}
		break;
	case Enemy::Direction::up:
		if (m_CanJump)
		{
			m_CanJump = false;
			m_Velocity.y = 200.f;
		}
		break;
	case Enemy::Direction::left:
	{
		m_IsFacingRight = false;
		m_Velocity.x = -50.f;
	}
		break;
	case Enemy::Direction::right:
	{
		m_IsFacingRight = true;
		m_Velocity.x = 50.f;
	}
		break;
	case Enemy::Direction::down:
		break;
	default:
		break;
	}
	if (m_IsAttacking)
	{
		m_AttackDelay = true;
		m_Velocity.x -= m_Velocity.x * 2 * elapsedSec;
		if (m_IsFacingRight)m_Claws = Rectf{ m_Position.x + m_Character.width,m_Position.y + m_Character.height / 2.f,60.f,20.f };
		else m_Claws = Rectf{ m_Position.x - 60.f,m_Position.y + m_Character.height / 2.f,60.f,20.f };
		if (m_AccummelatedTimeAttack > 0.5f)
		{
			m_IsAttacking = false;
		}
	}
	else m_Claws = Rectf{ -500.f,-500.f,0,0 };
	if (m_AccummelatedTimeAttack > 1.f)
	{
		m_AttackDelay = false;
		m_AccummelatedTimeAttack = 0.f;
	}
	//std::cout << hitinfoWall.normal.x << std::endl;
	if (hitinfoWall.normal.x <= -0.9f)
	{
		m_Position.x = hitinfoWall.intersectPoint.x;
		m_IsSpecialAtt = false;
		m_Delay = 2.f;
		m_LineWidth = 15.f;
	}
	else if (hitinfoWall.normal.x <= -0.9f)
	{
		m_Position.x = hitinfoWall.intersectPoint.x-m_Character.width;
		m_IsSpecialAtt = false;
		m_Delay = 2.f;
		m_LineWidth = 15.f;
	}
	else if (hitinfoWall.normal.x >= 0.9f)
	{
		m_Position.x = hitinfoWall.intersectPoint.x - m_Character.width;
		m_IsSpecialAtt = false;
		m_Delay = 2.f;
		m_LineWidth = 15.f;
	}
	if (hitinfoGravity.normal.y == -1.f && m_Direction != Direction::up)
	{
		m_Velocity.y = 0;
		m_CanJump = true;
		m_Position.y = hitinfoGravity.intersectPoint.y;
	}
	//else if (hitinfoGravity.normal.y != 1.f) m_Velocity.y -= 200.f * 5.f / 2.f * elapsedSec;
	if (!m_IsHit)m_Character = Rectf{ m_Position.x,m_Position.y,m_Size/2.f,m_Size };
	else m_Character = Rectf{ m_Position.x,m_Position.y,m_Size,m_Size / 2.f };

	m_CenterPoint = Point2f{ m_Position.x + m_Character.width / 2.f,m_Position.y + m_Character.height / 2.f };
	
	if (Vector2f{ m_CenterPoint,Point2f{m_CenterPointHeroX,m_CenterPointHeroY} }.Length() >= 350.f)m_IsLocking = false;
	else m_IsLocking = true;
	if (hitinfoChargeAttack.normal.x != 0 || hitinfoChargeAttack.normal.y != 0) m_IsLocking = false;
	if (m_IsLocking &&m_Delay<0.f)
	{
		SetIsSpecialAtt();
	}
	if (!m_IsSpecialAtt)m_MouseFollow = Point2f{ Vector2f{m_CenterPoint,Point2f{m_CenterPointHeroX,m_CenterPointHeroY} }.Normalized() * m_LineLength + Vector2f{ m_CenterPoint} };

	if (m_IsSpecialAtt)
	{
		if (m_LineWidth > 1.f&&m_IsLocking)m_LineWidth -= 20.f * elapsedSec;
		else
		{
			if (m_CenterPoint.x > m_MouseFollow.x - 10 && m_CenterPoint.x < m_MouseFollow.x + 10)
			{
				m_IsSpecialAtt = false;
				m_LineWidth = 15.f;
				m_Delay = 1.25f;
			}

			else if (m_CenterPoint.x < m_MouseFollow.x - 10 || m_CenterPoint.x > m_MouseFollow.x + 10 || m_CenterPoint.y < m_MouseFollow.y - 10 || m_CenterPoint.y > m_MouseFollow.y + 10)
				m_Velocity = Vector2f{ m_MouseFollow.x - m_CenterPoint.x,m_MouseFollow.y - m_CenterPoint.y }.Normalized() * 750.f;
			
		}


	}
	m_Position.x += m_Velocity.x * elapsedSec;
	m_Position.y += m_Velocity.y * elapsedSec;
}

Point2f Enemy::GetPoint2f() const
{
	return m_Position;
}

void Enemy::SetIsHit(const Rectf& hitbox)
{
	if (utils::IsOverlapping(m_Character, hitbox))m_IsHit = true;
}

void Enemy::Draw() const
{

	utils::SetColor(Color4f{ 0.f,0.f,0.f,1.f });
	utils::FillRect(Rectf{ m_Character.left - 2.f,m_Character.bottom - 2.f,m_Character.width + 4.f,m_Character.height + 4.f });
	utils::SetColor(Color4f{ 1.f,0.0f,0.f,1.f });
	if (m_IsAttacking)
		utils::FillRect(m_Claws);

	if (m_IsHit)utils::SetColor(Color4f{ 0.6f,0.2f,0.2f,1.f });
	else utils::SetColor(Color4f{ 1.f,0.2f,0.2f,1.f });
	utils::FillRect(m_Character);

	utils::SetColor(Color4f{ 1.f,0.0f,0.0f,1.f });
	if(m_IsLocking)utils::DrawLine(m_CenterPoint, m_MouseFollow, m_LineWidth);
}

void Enemy::SetCollision(const std::vector<Point2f>& collision)
{
	m_Collision = collision;
}

void Enemy::SetCenterPointHero(const Point2f& herpos)
{
	m_CenterPointHeroX = herpos.x;
	m_CenterPointHeroY = herpos.y;
}

void Enemy::SetIsSpecialAtt()
{
	m_IsSpecialAtt = true;
}

Rectf Enemy::GetHitBoxCharacter() const
{
	return m_Character;
}

Rectf Enemy::GetHitBoxAttack() const
{
	if (m_LineWidth<1.f)return m_Character;
	else return m_Claws;
}
