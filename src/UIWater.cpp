#include "UIWater.h"

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "Environment.h"
#include "Noggit.h" // fonts
#include "revision.h"
#include "UIText.h"
#include "UIButton.h"
#include "UICheckBox.h"
#include "UITexture.h"
#include "UISlider.h"
#include "UIMapViewGUI.h"
#include "Misc.h"
#include "World.h"
#include "Video.h" // video
#include <iostream>
#include <sstream>
#include "UIWaterTypeBrowser.h"

#include "Log.h"

void toggleDisplayAllLayers(bool b, int);

UIWater::UIWater(UIMapViewGUI *setGui)
	: UIWindow((float)video.xres() / 2.0f - (float)winWidth / 2.0f, (float)video.yres() / 2.0f - (float)winHeight / 2.0f - (float)(video.yres() / 4), (float)winWidth, (float)winHeight), mainGui(setGui)
{
	addChild(new UIText(78.5f, 2.0f, "Water edit", app.getArial14(), eJustifyCenter));


	waterOpacity = new UISlider(5.0f, 40.0f, 169.0f, 255.0f, 0.0f);
	waterOpacity->setValue(1);
	waterOpacity->setText("Opacity: ");
	waterOpacity->setFunc(boost::bind(&UIWater::setWaterTrans, this, _1));
	addChild(waterOpacity);
	addChild(new UIText(5.0f, 50.0f, "shift + numpad +/-", app.getArial12(), eJustifyLeft));

	addChild(new UIText(60.0f, 74.0f, "Level: ", app.getArial12(), eJustifyLeft));
	waterLevel = new UIText(95.0f, 74.0f, "0.0", app.getArial12(), eJustifyLeft);
	addChild(waterLevel);

	addChild(new UIButton(5.0f, 90.0f, 38.0f, 30.0f,
		"-100",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Disabled.blp",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Down.blp",
		boost::bind(&UIWater::changeWaterHeight, this, _1, _2),
		-100)
		);

	addChild(new UIButton(45.0f, 90.0f, 23.0f, 30.0f,
		"-10",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Disabled.blp",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Down.blp",
		boost::bind(&UIWater::changeWaterHeight, this, _1, _2),
		-10)
		);

	addChild(new UIButton(70.0f, 90.0f, 19.0f, 30.0f,
		"-1",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Disabled.blp",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Down.blp",
		boost::bind(&UIWater::changeWaterHeight, this, _1, _2),
		-1)
		);

	addChild(new UIButton(91.0f, 90.0f, 19.0f, 30.0f,
		"+1",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Disabled.blp",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Down.blp",
		boost::bind(&UIWater::changeWaterHeight, this, _1, _2),
		1)
		);

	addChild(new UIButton(112.0f, 90.0f, 23.0f, 30.0f,
		"+10",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Disabled.blp",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Down.blp",
		boost::bind(&UIWater::changeWaterHeight, this, _1, _2),
		10)
		);

	addChild(new UIButton(137.0f, 90.0f, 38.0f, 30.0f,
		"+100",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Disabled.blp",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Down.blp",
		boost::bind(&UIWater::changeWaterHeight, this, _1, _2),
		100)
		);
	addChild(new UIText(5.0f, 108.f, "numpad +/- (alt *2, ctrl *5)", app.getArial12(), eJustifyLeft));

	waterType = new UIButton(5.0f, 135.0f, 170.0f, 30.0f,
		"Type: none",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Disabled.blp",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Down.blp",
		boost::bind(&UIWater::openWaterTypeBrowser, this, _1, _2),
		100);

	addChild(waterType);

	waterGenFactor = new UISlider(5.0f, 185.0f, 169.0f, 100.0f, 0.0f);
	waterGenFactor->setValue(0.5f);
	waterGenFactor->setText("Opacity Gen Factor: ");
	addChild(waterGenFactor);

	waterGen = new UIButton(5.0f, 205.0f, 170.0f, 30.0f,
		"Auto Opacity",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Disabled.blp",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Down.blp",
		boost::bind(&UIWater::autoGen, this, _1, _2),
		100);

	addChild(waterGen);

	addWater = new UIButton(5.0f, 225.0f, 170.0f, 30.0f,
		"Fill with water",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Disabled.blp",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Down.blp",
		boost::bind(&UIWater::AddWater, this, _1, _2),
		100);
	addChild(addWater);

	cropWater = new UIButton(5.0f, 245.0f, 170.0f, 30.0f,
		"Crop water",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Disabled.blp",
		"Interface\\BUTTONS\\UI-DialogBox-Button-Down.blp",
		boost::bind(&UIWater::CropWater, this, _1, _2),
		100);
	addChild(cropWater);

  displayAllLayers = new UICheckBox(5.0f, 270.0f, "Show all layers", [] (bool b, int)
  {
    Environment::getInstance()->displayAllWaterLayers = b;
  }, 0);
  displayAllLayers->setState(Environment::getInstance()->displayAllWaterLayers);
  addChild(displayAllLayers);

  UIText *txt = new UIText(5.0f, 300.0f, app.getArial12(), eJustifyLeft);
  txt->setText("Current layer:");
  addChild(txt);


  waterLayer = new UIText(90.0f, 322.0f, app.getArial12(), eJustifyCenter);
  waterLayer->setText(std::to_string(Environment::getInstance()->currentWaterLayer + 1));
  addChild(waterLayer);

  addChild(new UIButton(28.0f, 320.0f, 50.0f, 30.0f,
    "<<",
    "Interface\\BUTTONS\\UI-DialogBox-Button-Disabled.blp",
    "Interface\\BUTTONS\\UI-DialogBox-Button-Down.blp",
    [this] (UIFrame::Ptr, int) 
    {
      size_t layer = std::max(0, Environment::getInstance()->currentWaterLayer - 1);
      waterLayer->setText(std::to_string(layer + 1));
      Environment::getInstance()->currentWaterLayer = layer;
    },
    0)
    );

  addChild(new UIButton(102.0f, 320.0f, 50.0f, 30.0f,
    ">>",
    "Interface\\BUTTONS\\UI-DialogBox-Button-Disabled.blp",
    "Interface\\BUTTONS\\UI-DialogBox-Button-Down.blp",
    [this](UIFrame::Ptr, int)
    {
      size_t layer = std::min(4, Environment::getInstance()->currentWaterLayer + 1);
      waterLayer->setText(std::to_string(layer + 1));
      Environment::getInstance()->currentWaterLayer = layer;
    },
    0)
  );

	updateData();
}

void UIWater::updatePos(int newTileX, int newTileY)
{
	if (tileX == newTileX && tileY == newTileY) return;

	tileX = newTileX;
	tileY = newTileY;

	updateData();
}

void UIWater::updateData()
{
	float h = gWorld->HaveSelectWater(tileX, tileY);
	if (h)
	{
		std::stringstream ms;
		ms << h;
		waterLevel->setText(ms.str());
	}
	else
	{
		std::stringstream ms;
		ms << gWorld->getWaterHeight(tileX, tileY);
		waterLevel->setText(ms.str());
	}
	waterOpacity->value = (gWorld->getWaterTrans(tileX, tileY) / 255.0f);

	std::stringstream mt;
	mt << gWorld->getWaterType(tileX, tileY) << " - " << LiquidTypeDB::getLiquidName(gWorld->getWaterType(tileX, tileY));

	waterType->setText(mt.str());
}

void UIWater::resize()
{
	// fixed so no resize
}

void UIWater::setWaterTrans(float val)
{
	if (std::fmod(val, 0.1f) > 0.1f) return; //reduce performence hit
	gWorld->setWaterTrans(tileX, tileY, (unsigned char)val);
}

void UIWater::addWaterLayer(UIFrame::Ptr /*ptr*/, int /*someint*/)
{
	gWorld->addWaterLayer(tileX, tileY, 0.0f, (unsigned char)(waterOpacity->value * 255));
}

void UIWater::deleteWaterLayer(UIFrame::Ptr /*ptr*/, int /*someint*/)
{
	gWorld->deleteWaterLayer(tileX, tileY);
}

void UIWater::setWaterHeight(float val)
{
	if (std::fmod(val, 0.1f) > 0.1f) return; //reduce performence hit
	gWorld->setWaterHeight(tileX, tileY, val);
}

void UIWater::changeWaterHeight(UIFrame::Ptr /*ptr*/, int someint)
{
	gWorld->setWaterHeight(tileX, tileY, ((float)someint));
	updateData();
}

void UIWater::openWaterTypeBrowser(UIFrame::Ptr /*ptr*/, int someint)
{
	if (this->mainGui->guiWaterTypeSelector->hidden())
		this->mainGui->guiWaterTypeSelector->show();
	else
		this->mainGui->guiWaterTypeSelector->hide();
}

void UIWater::changeWaterType(int waterint)
{
	gWorld->setWaterType(tileX, tileY, waterint);
	updateData();
}

void UIWater::autoGen(UIFrame::Ptr ptr, int someint)
{
	gWorld->autoGenWaterTrans(tileX, tileY, (int)waterGenFactor->value * 100);
	updateData();
}

void UIWater::AddWater(UIFrame::Ptr ptr, int someint)
{
	gWorld->AddWaters(tileX, tileY);
	updateData();
}

void UIWater::CropWater(UIFrame::Ptr ptr, int someint)
{
	gWorld->CropWaterADT(tileX, tileY);
	updateData();
}

void toggleDisplayAllLayers(bool b, int)
{
  Environment::getInstance()->displayAllWaterLayers = b;
}