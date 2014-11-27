#include "pch.h"

#include "ToolScene.h"
#include "SpriteTool/ToolSpriteEditLayer.h"
#include "GameManager.h"
#include "DataManager.h"

#define BOXWIDTH 170
#define BOXHEIGHT 60
#define WINSIZE_WIDTH 1280
#define WINSIZE_HEIGHT 640
#define FONTSIZE 30
#define LABELFONTSIZE 30
#define LABELWIDTH 200
#define LABELHEIGHT 100
#define ASSIGNNUM 20

ToolSpriteEditLayer::~ToolSpriteEditLayer()
{
	GET_DATA_MANAGER()->saveResourceData();
}

bool ToolSpriteEditLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setName("Edit");
	m_EditBoxs.reserve(10);
	
	m_IsATStateInList = true;
	m_CurrentSTInfoType = ST_END;
	m_CurrentATInfoType = AT_END;

	//저장된 Info값을 모두 Label로 만든 다음에 보여줄 곳을 지정한다
	createLabel("Type");
	createLabel("Delay");
	createLabel("FrameNum");
	assignLabels();
	//목록 버튼 만들기
	createListButton("ST_LIST", false);
	createListButton("AT_LIST", true);
	createAllMenuButton();
	assignMenuButtons();

	createEditBox("Type", FONTSIZE, 15, TYPE);
	createEditBox("Delay", FONTSIZE, 5, DELAY);
	createEditBox("FrameNum", FONTSIZE, 4, FRAMENUM);
	createEditBox("FileName", FONTSIZE - 10, 30, FILENAME, 27);
	createEditBox("Save", FONTSIZE, 1, SAVE);
	assignEditBox();
	assignFileNameBox();

	return true;
}

void ToolSpriteEditLayer::createEditBox(char* boxTitle, int fontSize, int maxLength, InfoOrder tag)
{
	if (boxTitle == nullptr) return;

	auto editBox = cocos2d::extension::EditBox::
		create(cocos2d::Size(BOXWIDTH, BOXHEIGHT),
		cocos2d::extension::Scale9Sprite::create("Graphic/SpriteToolEditBox.png"));
	editBox->setPlaceHolder(boxTitle);
	editBox->setFont("Corbel", fontSize);
	editBox->setTag(tag);
	editBox->setMaxLength(maxLength);
	editBox->setDelegate(this);
	this->addChild(editBox);

	m_EditBoxs.push_back(editBox);
}

void ToolSpriteEditLayer::createEditBox(char* boxTitle, int fontSize, int maxLength, InfoOrder tag, int boxNum)
{
	if (boxTitle == nullptr) return;

	char titleBuffer[MAX_CHAR];

	for (int i = 0; i < boxNum; ++i)
	{
		auto editBox = cocos2d::extension::EditBox::
			create(cocos2d::Size(BOXWIDTH, BOXHEIGHT / 3),
			cocos2d::extension::Scale9Sprite::create("Graphic/SpriteToolEditBox.png"));
		sprintf(titleBuffer, "%s%d", boxTitle, i+1);
		editBox->setPlaceHolder(titleBuffer);
		editBox->setFont("Corbel", fontSize);
		editBox->setTag(tag);
		editBox->setZOrder(i);
		editBox->setMaxLength(maxLength);
		editBox->setColor(cocos2d::Color3B::YELLOW);
		editBox->setVisible(true);
		editBox->setDelegate(this);
		
		this->addChild(editBox);
		m_FileNameBoxs.push_back(editBox);
	}
}



void ToolSpriteEditLayer::editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox)
{
}
void ToolSpriteEditLayer::editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox)
{
	if (editBox == nullptr) return;
	if (m_IsATStateInList)
	{
		if (editBox->getTag() == SAVE)
		{
			GET_DATA_MANAGER()->saveResourceData();
		}
	}
	else
	{
		if (editBox->getTag() == SAVE)
		{
			GET_DATA_MANAGER()->saveResourceData();
		}
	}
}

void ToolSpriteEditLayer::editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string& text)
{
}

void ToolSpriteEditLayer::editBoxReturn(cocos2d::extension::EditBox* editBox)
{
	//여기서 값이 바뀌어야 한다. 
	if (editBox == nullptr) return;

	int changedType = 0;
	float changedDelay = 0.0f;
	int changedFrameNum = 0;

	if (m_IsATStateInList)
	{
		switch (editBox->getTag())
		{
		case TYPE:
			changedType = atoi(editBox->getText());
			if (m_CurrentATInfoType == AT_END)
			{
				//파일안에 없다
				//푸쉬백해줘야한다
				AnimationInfo tmpInfo;
				tmpInfo.m_FrameNum = 0;
				tmpInfo.m_Delay = 0;
				tmpInfo.m_AnimationName[0][0] = '\0';
				tmpInfo.m_Type = (ResourceType)changedType;
				GET_DATA_MANAGER()->getAnimationInfos().push_back(tmpInfo);
			}
			else
			{
				for (auto& pAniInfo : GET_DATA_MANAGER()->getAnimationInfos())
				{
					if (pAniInfo.m_Type == m_CurrentATInfoType)
					{
						pAniInfo.m_Type = (ResourceType)changedType;
					}
				}
			}
			break;
		case DELAY:
			if (m_CurrentATInfoType != AT_END)
			{
				changedDelay = atof(editBox->getText());
				for (auto& pAniInfo : GET_DATA_MANAGER()->getAnimationInfos())
				{
					if (pAniInfo.m_Type == m_CurrentATInfoType)
					{
						pAniInfo.m_Delay = changedDelay;
					}
				}
			}
			break;
		case FRAMENUM:
			if (m_CurrentATInfoType != AT_END)
			{
				changedFrameNum = atoi(editBox->getText());
				for (auto& pAniInfo : GET_DATA_MANAGER()->getAnimationInfos())
				{
					if (pAniInfo.m_Type == m_CurrentATInfoType)
					{
						pAniInfo.m_FrameNum = changedFrameNum;
					}
				}
			}
			break;
		case FILENAME:
			if (m_CurrentATInfoType != AT_END)
			{
				for (auto& pAniInfo : GET_DATA_MANAGER()->getAnimationInfos())
				{
					if (pAniInfo.m_Type == m_CurrentATInfoType)
					{
						strcpy(pAniInfo.m_AnimationName[editBox->getZOrder()],
								 editBox->getText());
					}
				}
			}
			break;
		case SAVE:
			GET_DATA_MANAGER()->saveResourceData();
			break;
		default:
			break;
		}
	}
	else if (!m_IsATStateInList)
	{
		switch (editBox->getTag())
		{
		case TYPE:
			changedType = atoi(editBox->getText());
			if (m_CurrentATInfoType == AT_END)
			{
				//Json 파일안에 전혀 없는 내용이다 
				//푸쉬백해줘야한다
				SpriteInfo tmpInfo;
				tmpInfo.m_SpriteName[0] = '\0';
				tmpInfo.m_Type = (ResourceType)changedType;
				GET_DATA_MANAGER()->getSpriteInfos().push_back(tmpInfo);
			}
			else
			{
				for (auto& pSprInfo : GET_DATA_MANAGER()->getSpriteInfos())
				{
					if (pSprInfo.m_Type == m_CurrentATInfoType)
					{
						pSprInfo.m_Type = (ResourceType)changedType;
					}
				}
			}
		case FILENAME:
			if (m_CurrentSTInfoType != AT_END)
			{
				for (auto& pSprInfo : GET_DATA_MANAGER()->getSpriteInfos())
				{
					if (pSprInfo.m_Type == m_CurrentSTInfoType)
					{
						strcpy(pSprInfo.m_SpriteName,
							   editBox->getText());
					}
				}
			}
			break;
		case SAVE:
			GET_DATA_MANAGER()->saveResourceData();
			break;
		default:
			break;
		}

	}

}

void ToolSpriteEditLayer::update(float dTime)
{
}

void ToolSpriteEditLayer::createMenuButton(ResourceType type, bool isAT)
{
	char title[30];
	memset(title, NULL, sizeof(title)*sizeof(char));
	sprintf(title, "%d", type);


	//이거 적용해되 되는지 의문이네 
	/*for (auto& pEditBox : m_EditBoxs)
	{
		if (pEditBox->getTag() == TYPE)
		{
			pEditBox->setText(title);
		}
	}*/

	if (isAT)
	{
		auto playButton = cocos2d::MenuItemFont::create(title, CC_CALLBACK_1(ToolSpriteEditLayer::ATMenuButtonCallback, this));
		auto menu = cocos2d::Menu::create(playButton, NULL);
		menu->setScale(0.5f);
		menu->setVisible(false);
		playButton->setTag(type);
		this->addChild(menu);
		m_ATMenuButtons.push_back(menu);
	}
	else
	{
		auto playButton = cocos2d::MenuItemFont::create(title, CC_CALLBACK_1(ToolSpriteEditLayer::STMenuButtonCallback, this));
		auto menu = cocos2d::Menu::create(playButton, NULL);
		menu->setScale(0.5f);
		menu->setVisible(false);
		playButton->setTag(type);
		this->addChild(menu);
		m_STMenuButtons.push_back(menu);
	}

}


void ToolSpriteEditLayer::createListButton(std::string string, bool isAT)
{
	cocos2d::Point pos;
	pos.x = 100;
	pos.y = WINSIZE_HEIGHT * 0.92f;
	cocos2d::Menu* menu;
	if (isAT)
	{
		auto playButton = cocos2d::MenuItemFont::create(string, CC_CALLBACK_1(ToolSpriteEditLayer::ATListShow, this));
		menu = cocos2d::Menu::create(playButton, NULL);
		menu->setPosition(pos);
	}
	else
	{
		auto playButton = cocos2d::MenuItemFont::create(string, CC_CALLBACK_1(ToolSpriteEditLayer::STListShow, this));
		menu = cocos2d::Menu::create(playButton, NULL);
		pos.y -= BOXHEIGHT;
		menu->setPosition(pos);
	}

	this->addChild(menu);
}


void ToolSpriteEditLayer::ATMenuButtonCallback(cocos2d::Ref* sender)
{
	initFileNameBox();
	m_CurrentATInfoType = AT_END;
	m_CurrentSTInfoType = ST_END;
	m_IsATStateInList = true;
	auto button = (cocos2d::MenuItemFont*) sender;
	char typeBuf[50] = { 0, };
	char frameNumBuf[50] = { 0, };
	char delayBuf[50] = { 0, };
	
	bool isExistTypeInJson = false;
	ResourceType typeInJson;

	for (auto pAniInfo : GET_DATA_MANAGER()->getAnimationInfos())
	{
		if (button->getTag() == pAniInfo.m_Type)
		{
			isExistTypeInJson = true;
			typeInJson = (ResourceType)button->getTag();
			m_CurrentATInfoType = pAniInfo.m_Type;
			break;
		}
	}

	if (isExistTypeInJson)
	{
		_itoa(GET_DATA_MANAGER()->getAnimationInfo(typeInJson).m_Type, typeBuf, 10);
		_itoa(GET_DATA_MANAGER()->getAnimationInfo(typeInJson).m_FrameNum, frameNumBuf, 10);
		sprintf(delayBuf, "%f", GET_DATA_MANAGER()->getAnimationInfo(typeInJson).m_Delay);
	}
	else
	{
		strcpy_s(typeBuf, "Undecided");
		strcpy_s(frameNumBuf, "Undecided");
		strcpy_s(delayBuf, "Undecided");

	}

	//에딧 박스(화면상 가운데 위쪽)에 내용채워넣기
	for (auto pEditBox : m_EditBoxs)
	{
		if (pEditBox->getTag() == TYPE)
		{
			pEditBox->setText(typeBuf);
		}
		else if (pEditBox->getTag() == DELAY)
		{
			pEditBox->setText(delayBuf);
		}
		else if(pEditBox->getTag() == FRAMENUM)
		{
			pEditBox->setText(frameNumBuf);
		}
	}

	//화면상 정 가운데 가장 많은 파일네임 박스에 내용채워넣기
	if (isExistTypeInJson)
	{
		for (int i = 0; i < GET_DATA_MANAGER()->getAnimationInfo(typeInJson).m_FrameNum; ++i)
		{
			m_FileNameBoxs[i]->setText(GET_DATA_MANAGER()->getAnimationInfo(typeInJson).m_AnimationName[i]);
		}
	}
}


void ToolSpriteEditLayer::STMenuButtonCallback(cocos2d::Ref* sender)
{
	initFileNameBox();
	m_CurrentATInfoType = AT_END;
	m_CurrentSTInfoType = ST_END;
	m_IsATStateInList = false;
	bool isExistTypeInJson = false;
	ResourceType typeInJson;
	
	auto button = (cocos2d::MenuItemFont*) sender;
	char buf[30];
	memset(buf, NULL, sizeof(char)* 30);
	char typeBuf[30];
	memset(typeBuf, NULL, sizeof(char)* 30);

	for (auto pSprInfo : GET_DATA_MANAGER()->getSpriteInfos())
	{
		if (button->getTag() == pSprInfo.m_Type)
		{
			isExistTypeInJson = true;
			typeInJson = (ResourceType)button->getTag();
			m_CurrentSTInfoType = pSprInfo.m_Type;
			break;
		}
	}

	if (isExistTypeInJson)
	{
		_itoa(GET_DATA_MANAGER()->getSpriteInfo(typeInJson).m_Type, typeBuf, 10);
	}
	else
	{
		strcpy_s(typeBuf, "Undecided");
	}

	for (auto pEditBox : m_EditBoxs)
	{
		if (pEditBox->getTag() == TYPE)
		{
			pEditBox->setText(typeBuf);
		}
	}

	if (isExistTypeInJson)
	{
		m_FileNameBoxs[0]->setText(GET_DATA_MANAGER()->getSpriteInfo(typeInJson).m_SpriteName);
	}
	
}



void ToolSpriteEditLayer::ATListShow(cocos2d::Ref* sender)
{
	for (auto pSTButton : m_STMenuButtons)
	{
		pSTButton->setVisible(false);
	}

	for (auto pATButton : m_ATMenuButtons)
	{
		pATButton->setVisible(true);
	}
}

void ToolSpriteEditLayer::STListShow(cocos2d::Ref* sender)
{
	for (auto pATButton : m_ATMenuButtons)
	{
		pATButton->setVisible(false);
	}

	for (auto pSTButton : m_STMenuButtons)
	{
		pSTButton->setVisible(true);
	}
}



void ToolSpriteEditLayer::createLabel(std::string title)
{
	auto label = cocos2d::Label::createWithSystemFont(title, "Corbel", LABELFONTSIZE,
													  cocos2d::Size(LABELWIDTH, LABELHEIGHT),
													  cocos2d::TextHAlignment::CENTER);
	m_Labels.push_back(label);
	this->addChild(label);
}

void ToolSpriteEditLayer::assignLabels()
{
	cocos2d::Point pos;
	pos.x = WINSIZE_WIDTH* 0.3f;
	pos.y = WINSIZE_HEIGHT*0.92f;
	for (auto pLabel:m_Labels)
	{
		pLabel->setPosition(pos);
		pos.x += LABELWIDTH * 1.2;
	}
}

void ToolSpriteEditLayer::assignEditBox()
{
	if (m_EditBoxs.empty()) return;

	cocos2d::Point pos;
	pos.x = WINSIZE_WIDTH * 0.35f;
	pos.y = WINSIZE_HEIGHT * 0.88f;

	for (auto pEditBox : m_EditBoxs)
	{
		pEditBox->setPosition(pos);
		pos.x += BOXWIDTH * 1.3f;
	}
}

void ToolSpriteEditLayer::assignFileNameBox()
{

	cocos2d::Point pos;
	pos.x = WINSIZE_WIDTH* 0.3f;
	pos.y = WINSIZE_HEIGHT*0.78f + 60;

	for (unsigned int i = 0; i < m_FileNameBoxs.size(); ++i)
	{
		if (i != 0 && (i % 9) == 0)
		{
			pos.x += BOXWIDTH * 1.2f;
			pos.y = WINSIZE_HEIGHT*0.80f;
		}
		else
		{
			pos.y -= 60;
		}
		m_FileNameBoxs[i]->setPosition(pos);
	}
	
}

void ToolSpriteEditLayer::createAllMenuButton()
{
	for (ResourceType typeEnum = AT_START; typeEnum < AT_END; typeEnum = ResourceType(typeEnum + 1))
	{
		if (typeEnum == AT_PLAYER_END)
		{
			typeEnum = AT_MISSILE_START;
		}
		else if (typeEnum == AT_MISSILE_END)
		{
			typeEnum = AT_TRAP_START;
		}
		else if (typeEnum == AT_TRAP_END)
		{
			typeEnum = AT_MONSTER_START;
		}
		createMenuButton(typeEnum, true);
	}

	for (ResourceType typeEnum = ST_START; typeEnum < ST_END; typeEnum = ResourceType(typeEnum + 1))
	{
		if (typeEnum == ST_ETC_END)
		{
			typeEnum = ST_STEAM_START;
		}
		else if (typeEnum == ST_STEAM_END)
		{
			typeEnum = ST_GEAR_START;
		}
		else if (typeEnum == ST_GEAR_END)
		{
			typeEnum = ST_MAPWIN_START;
		}
		else if (typeEnum == ST_MAPWIN_END)
		{
			typeEnum = ST_MINIMAP_START;
		}
		else if (typeEnum == ST_MINIMAP_END)
		{
			typeEnum = ST_CHARWIN_START;
		}
		else if (typeEnum == ST_CHARWIN_END)
		{
			typeEnum = ST_HP_START;
		}
		else if (typeEnum == ST_HP_END)
		{
			typeEnum = ST_GAMEMENU_START;
		}
		else if (typeEnum == ST_GAMEMENU_END)
		{
			typeEnum = ST_MAPTOOL_START;
		}
		createMenuButton(typeEnum, false);
	}
}

void ToolSpriteEditLayer::assignMenuButtons()
{
	cocos2d::Point pos;

	pos.x = -WINSIZE_WIDTH * 0.22f;
	pos.y = WINSIZE_HEIGHT * 0.5f;
	int count = 0;

	for (auto pATButton : m_ATMenuButtons)
	{
		pATButton->setPosition(pos);
		pos.y -= BOXHEIGHT * 0.4f;
		++count;
		if (count % ASSIGNNUM == 0)
		{
			pos.x += BOXWIDTH * 0.35f;
			pos.y = WINSIZE_HEIGHT * 0.6f;
		}
	}

	pos.x = -WINSIZE_WIDTH * 0.22f;
	pos.y = WINSIZE_HEIGHT * 0.5f;
	count = 0;
	for (auto pSTButton : m_STMenuButtons)
	{
		pSTButton->setPosition(pos);
		pos.y -= BOXHEIGHT * 0.4f;
		++count;
		if (count % ASSIGNNUM == 0)
		{
			pos.x += BOXWIDTH * 0.35f;
			pos.y = WINSIZE_HEIGHT * 0.5f;
		}
	}
}

void ToolSpriteEditLayer::initFileNameBox()
{
	char buf[30];
	for (unsigned int i = 0; i < m_FileNameBoxs.size(); ++i)
	{
		sprintf(buf, "FileName%d", i+1);
		m_FileNameBoxs[i]->setText(buf);
	}
}

AnimationInfo ToolSpriteEditLayer::getAniMationInfo() const
{
	AnimationInfo tmpInfo;
	if (m_CurrentATInfoType != AT_END || m_CurrentSTInfoType == ST_END)
	{
		for (auto pAniInfo : GET_DATA_MANAGER()->getAnimationInfos())
		{
			if (pAniInfo.m_Type == m_CurrentATInfoType)
			{
				tmpInfo = pAniInfo;
			}
		}
	}
	else if (m_CurrentATInfoType == AT_END || m_CurrentSTInfoType != ST_END)
	{
		for (auto pSprInfo : GET_DATA_MANAGER()->getSpriteInfos())
		{
			if (pSprInfo.m_Type == m_CurrentSTInfoType)
			{
				tmpInfo.m_FrameNum = 1;
				strcpy(tmpInfo.m_AnimationName[0], pSprInfo.m_SpriteName);
			}
		}
	}
	else
	{
		tmpInfo.m_Type = AT_END;
		tmpInfo.m_Delay = 0;
		tmpInfo.m_FrameNum = 1;
		strcpy(tmpInfo.m_AnimationName[0], "block.png");
	}
	
	return tmpInfo;
}

ResourceType ToolSpriteEditLayer::getCurrentATInfoType() const
{
	return m_CurrentATInfoType;
}


ResourceType ToolSpriteEditLayer::getCurrentSTInfoType() const
{
	return m_CurrentSTInfoType;
}
















