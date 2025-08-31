#include "./includes.h"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/GameManager.hpp>
#include "./RainMenuLayer.h"

class $modify(GameManager) {
	gd::string getMenuMusicFile() {
		return gd::string("menuLoop.mp3"_spr);
	}
};

class MenuLayerManager {
	static bool originalMenuLayer;

	void reloadMenuLayerAndToggleState(CCObject*) {
		originalMenuLayer = !originalMenuLayer;
		CCDirector::get()->replaceScene(CCTransitionFade::create(0.3f, MenuLayer::scene(false)));
	}
public:
	static CCNode* getNode(){	
		auto menuLayerSwitcherMenu = CCMenu::create();
		menuLayerSwitcherMenu->setPosition({ 0.f, 0.f });

		auto menuLayerSwitcherLabel = CCLabelBMFont::create(originalMenuLayer ? "to rain layer" : "to original layer", "Lemonmilk.fnt"_spr);
		menuLayerSwitcherLabel->setOpacity(30);
		menuLayerSwitcherLabel->setScale(0.3f);
		auto menuLayerSwitcherBtn = CCMenuItemSpriteExtra::create(menuLayerSwitcherLabel, menuLayerSwitcherMenu, menu_selector(MenuLayerManager::reloadMenuLayerAndToggleState));
		menuLayerSwitcherBtn->setPosition({ 30.f, 5.f });
		menuLayerSwitcherMenu->addChild(menuLayerSwitcherBtn);

		return menuLayerSwitcherMenu;
	}

	static bool getState() {
		return originalMenuLayer;
	}
};
bool MenuLayerManager::originalMenuLayer = false;


class $modify(MenuLayer) {
	static cocos2d::CCScene* scene(bool isVideoOptionsOpen) {
		if (MenuLayerManager::getState()) return MenuLayer::scene(isVideoOptionsOpen);

		auto scene = CCScene::create();
		auto layer = RainMenuLayer::create();

		layer->addChild(MenuLayerManager::getNode(), 50);

		scene->addChild(layer);
		return scene;
	}
	bool init() {
		if (!MenuLayer::init()) return false;

		this->addChild(MenuLayerManager::getNode());

		return true;
	}
};
