#pragma once
#include "./includes.h"
#include "./Random.h"
#include "./RepeatingMoveingSprite.h"

class CloudLayer : public CCLayer {
	virtual bool init() {
		if (!CCLayer::init()) return false;
		
		for (int i = 0; i < 10; i++) {
			addCloud(Mod::get()->expandSpriteName("cloud" + std::to_string(Random::Int(1, 5)) + ".png").data());
		}

		return true;
	}
	void addCloud(const char* filename) {
		auto size = CCDirector::get()->getWinSize();
		auto cloud = RepeatingMovingSprite::createWithSpriteFrameName(filename, { size.width * 1.3f, 0.f}, { - size.width * 0.3f, 0.f}, Random::Float(20.f, 40.f), [size](RepeatingMovingSprite* self) {
			self->setMoveDuration(Random::Float(20.f, 40.f));
			self->setPositionY(Random::Float(0.f, size.height));
			self->getSprite()->setScale(Random::Float(0.6, 1.0));
			self->getSprite()->setOpacity(static_cast<int>(Random::Int(40, 70)));
		});
		cloud->setPositionY(Random::Float(0.f, size.height));
		cloud->getSprite()->setScale(Random::Float(0.6, 1.0));
		cloud->getSprite()->setOpacity(static_cast<int>(Random::Int(40, 70)));
		this->addChild(cloud);

		cloud->getSprite()->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
	}
public:
	static CloudLayer* create() {
		auto ret = new CloudLayer();
		if (ret && ret->init()) {
			ret->autorelease();
			return ret;
		}
		delete ret;
		return nullptr;
	}
};