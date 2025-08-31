#include "./includes.h"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/GameManager.hpp>
#include "./RainMenuLayer.h"

class $modify(GameManager) {
	gd::string getMenuMusicFile() {
		return gd::string("menuLoop.mp3"_spr);
	}
};
class $modify(MenuLayer) {
	static cocos2d::CCScene* scene(bool isVideoOptionsOpen) {
		auto scene = CCScene::create();
		auto layer = RainMenuLayer::create();
		scene->addChild(layer);
		return scene;

	}
};
