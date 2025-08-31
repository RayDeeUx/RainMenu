#include "./includes.h"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/GameManager.hpp>
#include "./RainMenuLayer.h"

class $modify(GameManager) {
	static void onModify(auto& self) {
		if (Loader::get()->isModLoaded("elnexreal.menuloop_randomizer")) (void) self.setHookPriorityAfterPost("GameManager::getMenuMusicFile", "elnexreal.menuloop_randomizer");
		else (void) self.setHookPriority("GameManager::getMenuMusicFile", Priority::Replace);
	}
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
        auto scene = MenuLayer::scene(isVideoOptionsOpen);
        auto menuLayer = scene->getChildByType<MenuLayer>(0);
        menuLayer->setVisible(false);
        auto layer = RainMenuLayer::create();
        scene->addChild(layer);
        layer->idkDoSomeToggleThingForMenuLayer(menuLayer);
        return scene;
    }
	bool init() {
		if (!MenuLayer::init()) return false;

		this->addChild(MenuLayerManager::getNode());

		return true;
	}
};
