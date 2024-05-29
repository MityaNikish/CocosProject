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

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::Layer {

public:
	virtual bool init();
	static cocos2d::Scene* createScene();
	CREATE_FUNC(GameScene);

    void startGame();
    cocos2d::Node* createBlock(int color, float size);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void findAdjacentBlocks(int x, int y, int color, std::vector<cocos2d::Vec2>& blocks, std::vector<std::vector<int>>& field_copy);
    void removeBlocks(const std::vector<cocos2d::Vec2>& blocks);
    void dropBlocks();
    void menuStartCallback(cocos2d::Ref* pSender);

private:
    const int width = 5;
    const int height = 5;
    const int colors = 3;
    int blockSize = 48;
    cocos2d::Node* fieldNode;
    std::vector<std::vector<int>> field = std::vector<std::vector<int>>(width, std::vector<int>(height, 0));
    size_t total_score;
    size_t best_score = 0;
};

#endif // __GAME_SCENE_H__