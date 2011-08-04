#include "UIAppInfo.h"

#include <string>

#include "Model.h" // Model
#include "ModelManager.h" // ModelManager
#include "Noggit.h" // arial14
#include "UIMapViewGUI.h" // UIMapViewGUI
#include "UIMinimizeButton.h" // UIMinimizeButton
#include "UIModel.h" // UIModel
#include "UIText.h" // UIText

UIAppInfo::UIAppInfo( float xPos, float yPos, float w, float h, UIMapViewGUI* setGui )
: UICloseWindow( xPos, yPos, w, h, "Application Info", true )
, mainGui( setGui )
, theInfos( new UIText( 8.0f, 7.0f, arial14, eJustifyLeft ) )
, mModelToLoad( "World\\AZEROTH\\ELWYNN\\PASSIVEDOODADS\\Trees\\CanopylessTree01.m2" )
{
  this->addChild( this->theInfos );

  UIModel* myTestmodel = new UIModel( 10.0f, 0.0f, w, h );
  myTestmodel->setModel( ModelManager::item( ModelManager::add( mModelToLoad ) ) );
  this->addChild( myTestmodel );
}

UIAppInfo::~UIAppInfo()
{
  ModelManager::delbyname( mModelToLoad );
}

void UIAppInfo::setText( const std::string& t )
{
  this->theInfos->setText( t );
}