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
	CCLabelBMFont* switcherLabel;

	void reloadMenuLayerAndToggleState(CCObject*) {
		CCScene* scene = CCScene::get();
		if (!scene) return;
		auto menuLayer = scene->getChildByID("MenuLayer");
		auto rainMenuLayer = scene->getChildByID("RainMenuLayer"_spr);
		if (!menuLayer || !rainMenuLayer) return;

		originalMenuLayer = !originalMenuLayer;
		menuLayer->setVisible(originalMenuLayer);
		rainMenuLayer->setVisible(!originalMenuLayer);
		switcherLabel->setString(originalMenuLayer ? "to rain layer" : "to original layer");
	}

public:
	static CCNode* getNode() {	
		auto menuLayerSwitcherMenu = CCMenu::create();
		menuLayerSwitcherMenu->setPosition({ 0.f, 0.f });

		auto menuLayerSwitcherLabel = CCLabelBMFont::create(originalMenuLayer ? "to rain layer" : "to original layer", "Lemonmilk.fnt"_spr);
		menuLayerSwitcherLabel->setOpacity(30);
		menuLayerSwitcherLabel->setScale(0.3f);
		menuLayerSwitcherLabel->setID("rain-menu-toggle-label"_spr);
		switcherLabel = menuLayerSwitcherLabel;
		auto menuLayerSwitcherBtn = CCMenuItemSpriteExtra::create(menuLayerSwitcherLabel, menuLayerSwitcherMenu, menu_selector(MenuLayerManager::reloadMenuLayerAndToggleState));
		menuLayerSwitcherBtn->setPosition({ 30.f, 5.f });
		menuLayerSwitcherBtn->setID("rain-menu-toggle"_spr);
		menuLayerSwitcherMenu->addChild(menuLayerSwitcherBtn);
		menuLayerSwitcherMenu->setID("rain-menu-menu"_spr);

		return menuLayerSwitcherMenu;
	}

	static bool getState() {
		return originalMenuLayer;
	}
};
bool MenuLayerManager::originalMenuLayer = false;


class $modify(MenuLayer) {
	static cocos2d::CCScene* scene(bool isVideoOptionsOpen) {
		CCScene* scene = MenuLayer::scene(isVideoOptionsOpen);

		auto layer = RainMenuLayer::create();
		scene->addChild(layer);
		scene->addChild(MenuLayerManager::getNode());

		bool origMenuLayer = MenuLayerManager::getState();
		scene->getChildByID("MenuLayer")->setVisible(origMenuLayer);
		layer->setVisible(!origMenuLayer);
		return scene;
	}
};