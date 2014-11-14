#include "pch.h"
#include "TrapUnderground.h"
#include "GameManager.h"
#include "ComponentManager.h"
#include "ResourceManager.h"
#include "AnimationComponent.h"
#include "SpriteComponent.h"
#include "PhysicsComponent.h"


bool Arthas::TrapUnderground::init()
{
	if (!Tile::init())
	{
		return false;
	}

	return true;
}

void Arthas::TrapUnderground::update(float dTime)
{

	if (m_AccumulatedTime - m_StartTime > m_Duration)
	{
		((PhysicsComponent*)getComponent(CT_PHYSICS))->setEnabled(true);
		((AnimationCompnent*)getComponent(CT_ANIMATION))->enter();

		m_AccumulatedTime = GET_GAME_MANAGER()->getTime().tv_sec;
		m_StartTime = GET_GAME_MANAGER()->getTime().tv_sec;

	}
	else
	{
		m_AccumulatedTime += GET_GAME_MANAGER()->getTime().tv_sec;
		//�̰� �Ź��ϴ°� �̻��ѵ�
		((PhysicsComponent*)getComponent(CT_PHYSICS))->setEnabled(false);
		((AnimationCompnent*)getComponent(CT_ANIMATION))->exit();
	}

}


void Arthas::TrapUnderground::initTile(cocos2d::Point origin, cocos2d::Size physicalSize, cocos2d::Size spriteSize)
{
	//3�ʿ� �ѹ� �����ϰ�
	m_Duration = 5;
	m_AccumulatedTime = 0;
	m_StartTime = 0;
	m_IsPrevTimeCheck = true;


	auto aniComponent = GET_COMPONENT_MANAGER()->createComponent<AnimationCompnent>();
	addComponent(aniComponent);
	aniComponent->setAnimation(AT_TRAP_UNDERGROUND, this, 2);
	
	//�ִϸ��̼� ų ���� �����ӿ� ���缭 ���� �浹�� �Ͼ�� �Ϸ��� ��� �ؾ���?
	initPhysicsBody(cocos2d::Size(69, 60));
	((PhysicsComponent*)getComponent(CT_PHYSICS))->setEnabled(false);
	
}

void Arthas::TrapUnderground::enter()
{
}

void Arthas::TrapUnderground::exit()
{
}