#ifndef __UI_WATER_H
#define __UI_WATER_H

#include "UICloseWindow.h"

class UISlider;
class UIText;
class UIButton;
class UICheckBox;
class UIMapViewGUI;

class UIWater : public UIWindow
{
public:
	UIWater(UIMapViewGUI *setGui);

	void resize();

	void updatePos(int newTileX, int newTileY);
	void updateData();

	void setWaterTrans(float val);
	void setWaterHeight(float val);
	void changeWaterHeight(UIFrame::Ptr, int someint);
	void changeWaterHeightChunk(UIFrame::Ptr, int someint, int i, int j);
	void addWaterLayer(UIFrame::Ptr, int);
	void deleteWaterLayer(UIFrame::Ptr ptr, int someint);
	void changeWaterType(int waterint);
	void openWaterTypeBrowser(UIFrame::Ptr /*ptr*/, int someint);
	void autoGen(UIFrame::Ptr ptr, int someint);
	void AddWater(UIFrame::Ptr ptr, int someint);
	void CropWater(UIFrame::Ptr ptr, int someint);

private:
	static const int winWidth = 180;
	static const int winHeight = 350;

	UIMapViewGUI *mainGui;
	UISlider *waterOpacity;
	UIText *waterLevel;
	UIButton *waterType;
	UIButton *waterGen;
	UIButton *addWater;
	UIButton *cropWater;
	UISlider *waterGenFactor;
  UICheckBox *displayAllLayers;
  UIText *waterLayer;

	int tileX;
	int tileY;
};

#endif
