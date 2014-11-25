﻿#pragma once
#include "cocos2d.h"
#include "Util.h"
#include "DataType.h"


class Player;
class GameScene;
class CommonInfo;
class StageManager
{
public:
	StageManager();
	~StageManager();

	bool					init();
	void					initStage(int stageNum);
	const Player*			getPlayer();
	int						getStageNum();
	int						getRoomNum();
	StageData				getCurrentStageData();
	RoomData				getCurrentRoomData();
	bool					changeRoom(int roomNum, cocos2d::Point pos);
	bool					shakeRoom();
	bool					addObject(BaseComponent* object, int roomNum, cocos2d::Point position, RoomZOrder zOrder);

private:
	StageData				m_StageData;
	GameScene*				m_GameScene;
	int						m_CurrentStageNum;
};

