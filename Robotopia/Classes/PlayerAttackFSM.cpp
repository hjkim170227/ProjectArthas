#include "pch.h"
#include "PlayerAttackFSM.h"
#include "GameManager.h"
#include "ComponentManager.h"
#include "TriggerManager.h"
#include "IdleState.h"
#include "AttackState.h"
#include "SeizeFireTrigger.h"
#include "KeyboardTrigger.h"

bool Arthas::PlayerAttackFSM::init()
{
	if(!FSMComponent::init())
		return false;
	m_Type = FSMT_PLAYER_ATTACK;
	return true;
}

void Arthas::PlayerAttackFSM::enter()
{
	auto idle = GET_COMPONENT_MANAGER()->createComponent<IdleState>();
	
	auto attack = GET_COMPONENT_MANAGER()->createComponent<AttackState>();
	attack->setAttribute(GET_COMP_PARENT(), 0.2f, 0.5f, 0, 0);

	SeizeFireTrigger* endAttackTrigger = GET_TRIGGER_MANAGER()->createTrigger<SeizeFireTrigger>();
	

	KeyboardTrigger* attackKeyHold = GET_TRIGGER_MANAGER()->createTrigger<KeyboardTrigger>();
	attackKeyHold->initKeyCode(KC_ATTACK, KS_PRESS | KS_HOLD);

	addComponent(idle);
	idle->addTransition(std::make_pair(attackKeyHold, attack));

	addComponent(attack);
	attack->addTransition(std::make_pair(endAttackTrigger, idle));

	m_NowState = idle;
}

void Arthas::PlayerAttackFSM::exit()
{

}
