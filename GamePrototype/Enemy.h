#pragma once
#include <utils.h>
class Enemy
{
public:
	enum class Direction
	{
		neutral, up, left, right, down
	};
	Enemy(Point2f pos,float size);
	void Update(float elapsedSec);
	Point2f GetPoint2f()const;
	void SetIsHit(const Rectf& hitbox);
	void Draw()const;
	void SetCollision(const std::vector<Point2f>& collision);
	void SetCenterPointHero(const Point2f& herpos);
	void SetIsSpecialAtt();
	Rectf GetHitBoxCharacter()const;
	Rectf GetHitBoxAttack()const;
private:

	bool m_isEnemyAttacking{};
	bool m_IsJumping{};
	bool m_CanJump;
	bool m_IsAttacking;
	bool m_IsSpecialAtt;
	bool m_IsHit;
	bool m_IsFacingRight;
	bool m_IsLocking;

	float m_CenterPointHeroX;
	float m_CenterPointHeroY;
	float m_LineWidth;
	Point2f m_MouseFollow;
	float m_LineLength;
	Point2f m_CenterPoint;

	float m_Size;
	Rectf m_Character;
	bool m_isDead;
	bool m_IsLeft;
	bool m_IsRight;
	Point2f m_Position;
	Direction m_Direction;
	Vector2f m_Velocity;
	Rectf m_Claws;
	float m_AccummelatedTimeAttack;
	float m_Delay;
	bool m_AttackDelay;
	std::vector<Point2f> m_Collision;
};

