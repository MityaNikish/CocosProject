/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "SettingsScene.h"

USING_NS_CC;

Scene* SettingsScene::createScene()
{
    auto scene = Scene::create();
    SettingsScene* layer = SettingsScene::create();
    scene->addChild(layer);
    return scene;
}

bool SettingsScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    auto colorLayer = LayerColor::create(Color4B(66, 0, 173, 255));
    this->addChild(colorLayer);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    createInputs();

    // Добавляем кнопку сохранения настроек
    //auto saveLabel = Label::createWithTTF("Save", "fonts/Marker Felt.ttf", 24);
    //auto saveMenuItem = MenuItemLabel::create(saveLabel, CC_CALLBACK_1(SettingsScene::onSave, this));
    //auto saveMenu = Menu::create(saveMenuItem, nullptr);
    //saveMenu->setPosition(Vec2(origin.x + visibleSize.width / 2,
    //    origin.y + visibleSize.height / 2 - 100));
    //this->addChild(saveMenu);

    auto saveMenuItem = MenuItemImage::create("save.png", "save_pressed.png", CC_CALLBACK_1(SettingsScene::onSave, this));
    auto saveMenu = Menu::create(saveMenuItem, nullptr);
    saveMenu->setAnchorPoint(Vec2(0.5, 0.5));
    saveMenu->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100, origin.y + visibleSize.height / 2 - 150));
    this->addChild(saveMenu);

    // Добавляем кнопку выхода из настроек
    //auto exitLabel = Label::createWithTTF("Exit", "fonts/Marker Felt.ttf", 24);
    //auto exitMenuItem = MenuItemLabel::create(exitLabel, CC_CALLBACK_1(SettingsScene::onExit, this));
    //auto exitMenu = Menu::create(exitMenuItem, nullptr);
    //exitMenu->setPosition(Vec2(origin.x + visibleSize.width / 2,
    //    origin.y + visibleSize.height / 2 - 200));
    //this->addChild(exitMenu);

    auto exitMenuItem = MenuItemImage::create("back.png", "back_pressed.png", CC_CALLBACK_1(SettingsScene::onExit, this));
    auto exitMenu = Menu::create(exitMenuItem, nullptr);
    exitMenu->setAnchorPoint(Vec2(0.5, 0.5));
    exitMenu->setPosition(Vec2(origin.x + visibleSize.width / 2 + 100, origin.y + visibleSize.height / 2 - 150));
    this->addChild(exitMenu);

    return true;
}

void SettingsScene::createInputs()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Создаем текстовое поле для ширины
    auto widthLabel = Label::createWithTTF("Width:", "fonts/arial.ttf", 24);
    widthLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100,
        origin.y + visibleSize.height / 2 + 50));
    this->addChild(widthLabel);

    //widthInput = TextFieldTTF::textFieldWithPlaceHolder("Enter width", "fonts/arial.ttf", 24);
    //widthInput->setPosition(Vec2(origin.x + visibleSize.width / 2 + 50,
    //    origin.y + visibleSize.height / 2 + 50));
    //this->addChild(widthInput);

    widthInput = ui::TextField::create("Enter width", "fonts/arial.ttf", 24);
    widthInput->setPosition(Vec2(origin.x + visibleSize.width / 2 + 50,
        origin.y + visibleSize.height / 2 + 50));
    this->addChild(widthInput);

    widthInput->addEventListener(
        [=](Ref* sender, ui::TextField::EventType type) 
        {
            if (type == ui::TextField::EventType::INSERT_TEXT) {
                std::string text = widthInput->getString();
                for (char c : text) {
                    if (!isdigit(c)) {
                        widthInput->setString(""); // Очистить текст, если не число
                        break;
                    }
                }
            }
        });

    // Создаем текстовое поле для высоты
    auto heightLabel = Label::createWithTTF("Height:", "fonts/arial.ttf", 24);
    heightLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100,
        origin.y + visibleSize.height / 2 - 50));
    this->addChild(heightLabel);

    //heightInput = TextFieldTTF::textFieldWithPlaceHolder("Enter height", "fonts/arial.ttf", 24);
    //heightInput->setPosition(Vec2(origin.x + visibleSize.width / 2 + 50,
    //    origin.y + visibleSize.height / 2 - 50));
    //this->addChild(heightInput);

    heightInput = ui::TextField::create("Enter height", "fonts/arial.ttf", 24);
    heightInput->setPosition(Vec2(origin.x + visibleSize.width / 2 + 50,
        origin.y + visibleSize.height / 2 - 50));
    this->addChild(heightInput);

    heightInput->addEventListener(
        [=](Ref* sender, ui::TextField::EventType type)
        {
            if (type == ui::TextField::EventType::INSERT_TEXT) {
                std::string text = heightInput->getString();
                for (char c : text) {
                    if (!isdigit(c)) {
                        heightInput->setString(""); // Очистить текст, если не число
                        break;
                    }
                }
            }
        });
}

void SettingsScene::onSave(cocos2d::Ref* sender)
{
    std::string widthStr = widthInput->getString();
    std::string heightStr = heightInput->getString();
    
    if (widthStr != "")
    {
        float newWidth = std::stof(widthStr);
        UserDefault::getInstance()->setFloatForKey("textFieldWidth", newWidth);
    }

    if (heightStr != "")
    {
        float newHeight = std::stof(heightStr);
        UserDefault::getInstance()->setFloatForKey("textFieldHeight", newHeight);
    }

    // Cохранения в UserDefault
    UserDefault::getInstance()->flush();
}

void SettingsScene::onExit(cocos2d::Ref* sender)
{
    Director::getInstance()->popScene();
}