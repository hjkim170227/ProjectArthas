﻿#include "pch.h"
#include "PenerateMissile.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "Effect.h"
#define PIE 3.1415926538

bool PenerateMissile::init()
{
	if (!Missile::init())
		return false;

	return true;
}

void PenerateMissile::update(float dTime)
{
	if (m_MaxDistance > 0)
	{
		//최대 사거리 도달하면 자동 삭제.
		if (m_StartPos.getDistance(getPosition()) > m_MaxDistance)
		{
			m_IsDead = true;
		}
	}
}

void PenerateMissile::enter()
{

}

void PenerateMissile::exit()
{
	m_IsDead = true;
}

void PenerateMissile::dead()
{
	setEnabled(false);
	removeChild(m_Sprite);
	m_IsUsable = true;
	removeFromParent();
}

void PenerateMissile::initMissile()
{
	m_Type = OT_MISSILE_PLAYER_LINEAR;
	m_IsDead = false;
	m_IsUsable = true;
	m_VelocityValue = 400; //임시 지정
}

void PenerateMissile::setAttribute(cocos2d::Point pos, Direction attackDir /*= DIR_NONE*/, float damage /*= 0*/, cocos2d::Size contentsSize /*= cocos2d::Size::ZERO*/, cocos2d::Vec2 velocity /*= cocos2d::Point::ZERO*/, cocos2d::Point targetPos /*= cocos2d::Point::ZERO*/)
{
	m_IsDead = false;
	m_IsUsable = false;
	m_IsPlayerMissile = true;
	m_Damage = damage;
	m_AttackDir = attackDir;
	m_TargetSize = contentsSize;
	m_StartTime = GET_GAME_MANAGER()->getMicroSecondTime();
	m_State = MST_NONE;

	m_Sprite = cocos2d::Sprite::create();
	addChild(m_Sprite);
	auto animate = cocos2d::Animate::create(GET_RESOURCE_MANAGER()->createAnimation(AT_MISSILE_PLAYER_LINEAR));


	m_Sprite->runAction(cocos2d::RepeatForever::create(animate));

	float distance = pos.getDistance(targetPos);

	float degree = 0;

	if (distance != 0)
	{
		m_Velocity.x = ((targetPos.x - pos.x) / distance) * m_VelocityValue;
		m_Velocity.y = ((targetPos.y - pos.y) / distance) * m_VelocityValue;
		degree = acos((pos.x - targetPos.x) / distance) * 180 / PIE;
	}
	else
	{
		m_Velocity.x = m_VelocityValue;
		m_Velocity.y = 0;
	}

	if (pos.y < targetPos.y)
	{
		m_Sprite->setRotation(degree);
	}
	else
	{
		m_Sprite->setRotation(-degree);
	}

	m_Sprite->setFlippedX(true);

	auto meterial = cocos2d::PhysicsMaterial(0, 0, 0);
	m_Body = cocos2d::PhysicsBody::createBox(cocos2d::Size(48, 48), meterial);
	m_Body->setContactTestBitmask(PHYC_MONSTER);
	m_Body->setCategoryBitmask(PHYC_MISSILE);
	m_Body->setCollisionBitmask(PHYC_MONSTER);
	m_Body->setTag(static_cast<int>(getType()));
	m_Body->setGravityEnable(false);
	m_Body->setMass(10);
	m_Body->setRotationEnable(false);
	m_Body->setVelocity(m_Velocity);
	m_Body->retain();
	m_StartPos = pos;
	setPhysicsBody(m_Body);
}

bool PenerateMissile::onContactBegin(cocos2d::PhysicsContact& contact)
{
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();
	auto componentA = (BaseComponent*)bodyA->getNode();
	auto componentB = (BaseComponent*)bodyB->getNode();
	BaseComponent* enemyComponent;
	bool isComponentA = true;

	if (componentA->getType() == getType())
	{
		enemyComponent = componentB;
		isComponentA = true;
	}
	else
	{
		enemyComponent = componentA;
		isComponentA = false;
	}

	if (enemyComponent->getPhysicsBody()->getCategoryBitmask() == PHYC_MONSTER)
	{
		if (m_IsPlayerMissile)
		{
			GET_EFFECT_MANAGER()->createEffect(ET_AIMING_MISSILE, 
				enemyComponent->getPosition() + cocos2d::Point(-5 + rand() % 10, -5 + rand() % 10))->enter();
		}
	}

	return false;
}

void PenerateMissile::setEnabled(bool enable)
{
	if (enable == true)
	{
		setPhysicsBody(m_Body);
	}
	else
	{
		setPhysicsBody(nullptr);
		if (m_Body != nullptr)
		{
			m_Body->removeFromWorld();
		}
	}
}

void PenerateMissile::setMaxDistance(float distance)
{
	m_MaxDistance = distance;
}
