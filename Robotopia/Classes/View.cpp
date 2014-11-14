#include "pch.h"
#include "View.h"
#include "GameLayer.h"
#include "GameManager.h"
#include "StageManager.h"
#include "DataManager.h"




void Arthas::View::setViewPort(cocos2d::Layer* layer, cocos2d::Point playerPosInRoomLayer, cocos2d::Point anchorPoint)
{
	
	int curFloorIdx = GET_STAGE_MANAGER()->getStageNum();
	int curRoomIdx = GET_STAGE_MANAGER()->getRoomNum();

	if (curFloorIdx == -1 || curRoomIdx == -1) return;

	cocos2d::Size curRoomSize;
	cocos2d::Point curRoomLayerPos;
	cocos2d::Point playerPosInGameLayer;

	Arthas::RoomData curRoomData = GET_DATA_MANAGER()->getRoomData(curFloorIdx, curRoomIdx);
	cocos2d::Size tileSize = GET_DATA_MANAGER()->getTileSize();

	float windowWidth = cocos2d::Director::getInstance()->getWinSize().width;
	float windowHeight = cocos2d::Director::getInstance()->getWinSize().height;
 	float anchorX = windowWidth * anchorPoint.x;
	float anchorY = windowHeight * anchorPoint.y;

	curRoomLayerPos.x = curRoomData.x * tileSize.width;
	curRoomLayerPos.y = curRoomData.y * tileSize.height;

	curRoomSize.width = curRoomData.width * tileSize.width;
	curRoomSize.height = curRoomData.height * tileSize.height;

	playerPosInGameLayer.x = curRoomLayerPos.x + playerPosInRoomLayer.x;
	playerPosInGameLayer.y = curRoomLayerPos.y + playerPosInRoomLayer.y;

	if (playerPosInRoomLayer.x + anchorX > curRoomSize.width)
	{
		anchorX = playerPosInRoomLayer.x - (curRoomSize.width - windowWidth);
	}
	if (playerPosInRoomLayer.x - anchorX < 0)
	{
		//�÷��̾ Room�� ������ ���� �ִ� ���� 
		anchorX = playerPosInRoomLayer.x;
		//���࿡ 0�����ϸ� ���� �� ������ ������ �ʴ´�.  
		//anchorX = 0;
	}


	if (playerPosInRoomLayer.y + anchorY > curRoomSize.height)
	{
		anchorY = playerPosInRoomLayer.y - (curRoomSize.height - windowHeight);
	}
	if (playerPosInRoomLayer.y - anchorY < 0)
	{
		anchorY = playerPosInRoomLayer.y;
	}

	layer->setPosition(anchorX - playerPosInGameLayer.x, anchorY - playerPosInGameLayer.y);

//	layer->setPosition(anchorX - playerPosInRoomLayer.x, anchorY - playerPosInRoomLayer.y);
}



void Arthas::View::setViewPortWithHighlight(cocos2d::Layer* layer, cocos2d::Rect standardRect)
{
	float windowWidth = cocos2d::Director::getInstance()->getWinSize().width;
	float windowHeight = cocos2d::Director::getInstance()->getWinSize().height;
	cocos2d::Point centerAnchor(0.5f, 0.5f);
	float ratioX = windowWidth / standardRect.size.width;
	float ratioY = windowHeight / standardRect.size.height;

	layer->setPosition(windowWidth * centerAnchor.x - standardRect.origin.x,
					   windowHeight * centerAnchor.y - standardRect.origin.y);
	layer->setScale(ratioX, ratioY);

	return;
}



void Arthas::View::setViewPortShake(cocos2d::Layer* layer, cocos2d::Point playerPosInRoomLayer, cocos2d::Point anchorPoint)
{

	int curFloorIdx = GET_STAGE_MANAGER()->getStageNum();
	int curRoomIdx = GET_STAGE_MANAGER()->getRoomNum();

	if (curFloorIdx == -1 || curRoomIdx == -1) return;

	cocos2d::Size curRoomSize;
	cocos2d::Point curRoomLayerPos;
	cocos2d::Point playerPosInGameLayer;

	Arthas::RoomData curRoomData = GET_DATA_MANAGER()->getRoomData(curFloorIdx, curRoomIdx);
	cocos2d::Size tileSize = GET_DATA_MANAGER()->getTileSize();

	float windowWidth = cocos2d::Director::getInstance()->getWinSize().width;
	float windowHeight = cocos2d::Director::getInstance()->getWinSize().height;
	float anchorX = windowWidth * anchorPoint.x;
	float anchorY = windowHeight * anchorPoint.y;

	curRoomLayerPos.x = curRoomData.x * tileSize.width;
	curRoomLayerPos.y = curRoomData.y * tileSize.height;

	curRoomSize.width = curRoomData.width * tileSize.width;
	curRoomSize.height = curRoomSize.height * tileSize.height;

	playerPosInGameLayer.x = curRoomLayerPos.x + playerPosInRoomLayer.x;
	playerPosInGameLayer.y = curRoomLayerPos.y + playerPosInRoomLayer.y;

	if (playerPosInRoomLayer.x + anchorX > curRoomSize.width)
	{
		anchorX = playerPosInRoomLayer.x - (curRoomSize.width - windowWidth);
	}
	if (playerPosInRoomLayer.x - anchorX < 0)
	{
		anchorX = playerPosInRoomLayer.x;
		//���࿡ 0�����ϸ� ���� �� ������ ������ �ʴ´�. 
		//anchorX = 0;
	}
	if (playerPosInRoomLayer.y + anchorY > curRoomSize.height)
	{
		anchorY = playerPosInRoomLayer.y - (curRoomSize.height - windowHeight);
	}
	if (playerPosInRoomLayer.y - anchorY < 0)
	{
		anchorY = playerPosInRoomLayer.y;
	}

	layer->setPosition(anchorX - playerPosInGameLayer.x, anchorY - playerPosInGameLayer.y); 

	anchorX += (10 + rand() % 90) / 5;
	anchorY += (10 + rand() % 90) / 5;

	layer->setPosition(anchorX - playerPosInRoomLayer.x, anchorY - playerPosInRoomLayer.y);


	/*timeval tv;
	timeval prevTV;
	cocos2d::gettimeofday(&tv, nullptr);
	long tvMillisec = tv.tv_usec / 1000 + tv.tv_sec * 1000;
	long prevMillisec = tvMillisec;

	while (tvMillisec - prevMillisec > 5000)
	{
	cocos2d::gettimeofday(&tv, nullptr);
	long tvMillisec = tv.tv_usec / 1000 + tv.tv_sec * 1000;
	}
	*/

	return;

}
