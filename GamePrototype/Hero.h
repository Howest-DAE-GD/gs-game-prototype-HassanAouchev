#pragma once
#include <utils.h>
#include <SoundEffect.h>
class Hero
{
public:
	enum class Direction
	{
		neutral,up,left,right,down
	};
	Hero(Point2f pos);
	void update(float elapsedSec);
	void SetIsHit(const Rectf& hitbox);
	void Draw()const;
	void SetCollision(const std::vector<Point2f>& collision);
	Rectf GetHitBoxCharacter()const;
	Rectf GetHitBoxAttack()const;
	Point2f GetCenterPoint()const;
	Point2f GetCameraLocation()const;
	void SetIsAttacking();
	void SetIsSpecialAtt();
	void SetMousePos(float x, float y);
	bool GetReset();
private:

	Rectf m_Character;
	bool m_isHeroAttacking{};
	bool m_IsJumping{};

	bool m_IsLeft;
	bool m_IsRight;
	bool m_IsUp;
	bool m_IsDown;
	bool m_CanJump;
	bool m_IsAttacking;
	bool m_IsSpecialAtt;
	bool m_IsHit;
	bool m_IsFacingRight;
	bool m_IsSpace;

	float m_MouseX;
	float m_MouseY;
	float m_LineWidth;
	Point2f m_MouseFollow;
	float m_LineLength;
	Point2f m_CenterPoint;

	Point2f m_Position;
	Direction m_Direction;
	Vector2f m_Velocity;
	Rectf m_Sword;
	float m_AccummelatedTimeAttack;
	bool m_AttackDelay;
	std::vector<Point2f> m_Collision;
	Point2f m_CameraLocation;
	bool m_StopLoop;
	bool m_CanDoubleJump;
	float m_DelayDoubleJump;
	int m_DashCharges;
	float m_rechargeDashes;
	Point2f m_DirectionPoint;
	bool m_Reset;
	float m_deathDelay;
};

