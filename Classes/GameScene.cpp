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

#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	GameScene* layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    auto colorLayer = LayerColor::create(Color4B(124, 21, 140, 255));
    this->addChild(colorLayer);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto totalScoreLabel = Label::createWithTTF("Score: 0", "fonts/arial.ttf", 30);
    totalScoreLabel->setTextColor(Color4B::ORANGE);
    totalScoreLabel->setPosition(visibleSize.width * 0.3, visibleSize.height * 0.95);
    totalScoreLabel->setTag(1);
    this->addChild(totalScoreLabel);

    auto bestScoreLabel = Label::createWithTTF("Best score: 0", "fonts/arial.ttf", 30);
    bestScoreLabel->setTextColor(Color4B::ORANGE);
    bestScoreLabel->setPosition(visibleSize.width * 0.7, visibleSize.height * 0.95);
    bestScoreLabel->setTag(2);
    this->addChild(bestScoreLabel);

    auto restartItem = MenuItemLabel::create(Label::createWithTTF("Restart", "fonts/arial.ttf", 30), CC_CALLBACK_1(GameScene::menuStartCallback, this));
    auto menu = Menu::create(restartItem, NULL);
    menu->setPosition(visibleSize.width / 2, visibleSize.height - restartItem->getContentSize().height);
    this->addChild(menu, 1);

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void GameScene::startGame()
{
    if (fieldNode)
    {
        fieldNode->removeAllChildren();
    }

    total_score = 0;
    auto totalScoreLabel = dynamic_cast<Label*>(this->getChildByTag(1));
    totalScoreLabel->setString("Score: " + std::to_string(total_score));

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    blockSize = std::min(visibleSize.height * 0.8 / height, visibleSize.width * 0.8 / width);

    fieldNode = Node::create();
    fieldNode->setTag(0);
    fieldNode->setPosition(Vec2((visibleSize.width - blockSize * width) / 2 + origin.x, (visibleSize.height - blockSize * height) / 2 + origin.y));

    this->addChild(fieldNode);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            int color = random(0, colors - 1);
            field[x][y] = color;
            auto block = createBlock(color, blockSize);
            block->setAnchorPoint(Vec2(0, 0));
            block->setPosition(Vec2(x * blockSize, y * blockSize));
            block->setTag(x * height + y);
            fieldNode->addChild(block);
        }
    }
}

Node* GameScene::createBlock(int color, float size)
{
    std::string colors[] = { "9-Slice/Colored/red.png", "9-Slice/Colored/green.png", "9-Slice/Colored/blue.png" };
    auto block = Sprite::create(colors[color]);   
    block->setContentSize(Size(size, size));
    return block;
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
    if (!touch)
    {
        return false;
    }

    Vec2 location = touch->getLocation();
    location -= fieldNode->getPosition();

    int x = location.x / blockSize;
    int y = location.y / blockSize;

    if (x >= 0 && x < width && y >= 0 && y < height && field[x][y] != -1)
    {
        int color = field[x][y];
        std::vector<Vec2> blocksToRemove;

        auto field_copy(field);
        findAdjacentBlocks(x, y, color, blocksToRemove, field_copy);

        if (blocksToRemove.size() >= 3)
        {
            removeBlocks(blocksToRemove);
            dropBlocks();
            if (best_score < total_score)
            {
                best_score = total_score;
                auto bestScoreLabel = dynamic_cast<Label*>(this->getChildByTag(2));
                bestScoreLabel->setString("Best score: " + std::to_string(best_score));
            }

            auto totalScoreLabel = dynamic_cast<Label*>(this->getChildByTag(1));
            totalScoreLabel->setString("Score: " + std::to_string(total_score));
        }
    }

    return true;
}

void GameScene::findAdjacentBlocks(int x, int y, int color, std::vector<Vec2>& blocks, std::vector<std::vector<int>>& field_copy)
{
    if (x < 0 || x >= width || y < 0 || y >= height || field_copy[x][y] != color)
    {
        return;
    }

    field_copy[x][y] = -1;
    blocks.push_back(Vec2(x, y));

    // Поиск смежных блоков
    findAdjacentBlocks(x + 1, y, color, blocks, field_copy);
    findAdjacentBlocks(x - 1, y, color, blocks, field_copy);
    findAdjacentBlocks(x, y + 1, color, blocks, field_copy);
    findAdjacentBlocks(x, y - 1, color, blocks, field_copy);

}

void GameScene::removeBlocks(const std::vector<Vec2>& blocks)
{
    for (const auto& block : blocks)
    {
        total_score++;
        int x = block.x;
        int y = block.y;
        field[x][y] = -1;
        auto blockNode = fieldNode->getChildByTag(x * height + y);
        if (blockNode)
        {
            blockNode->removeFromParent();
        }
    }
}

void GameScene::dropBlocks()
{
    for (int x = 0; x < width; x++)
    {
        int emptySpace = 0;
        for (int y = 0; y < height; y++)
        {
            if (field[x][y] == -1)
            {
                emptySpace++;
            }
            else if (emptySpace > 0)
            {
                field[x][y - emptySpace] = field[x][y];
                field[x][y] = -1;
                auto blockNode = fieldNode->getChildByTag(x * height + y);
                if (blockNode)
                {
                    blockNode->setPositionY(blockNode->getPositionY() - blockSize * emptySpace);
                    blockNode->setTag(x * height + (y - emptySpace));
                }
            }
        }
    }
}

void GameScene::menuStartCallback(Ref* pSender)
{
    startGame();
}