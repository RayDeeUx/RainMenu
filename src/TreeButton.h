#pragma once
#include "./includes.h"
#include "./RainbowColorGenerator.h"
#include "./ParticleSystemBuilder.h"

class TreeButton : public CCMenuItemSpriteExtra {
	CCSprite* blackSprite;
	CCSprite* rainbowSprite;
	CCLabelBMFont* label;
	std::function<void()> callback;
	CCParticleSystemQuad* glowEffect;

	RainbowColorGenerator rcg;

	TreeButton(float rcgPeriod) : rcg(rcgPeriod){}
	
	virtual void selected() override {
		CCMenuItemSpriteExtra::selected();
		label->runAction(CCFadeIn::create(0.5f));

		rainbowSprite->setOpacity(255);
		glowEffect->setOpacity(255);
	}

	virtual void unselected() override {
		CCMenuItemSpriteExtra::unselected();
		label->runAction(CCFadeOut::create(0.5f));
	}

	virtual void activate() override {
		CCMenuItemSpriteExtra::unselected();
		if (callback)
			callback();
	}

	virtual bool init(const char* frameName, std::string label, float scaleAnimationFadeDuration, std::function<void()> callback) {
		this->callback = callback;
		blackSprite = CCSprite::createWithSpriteFrameName(frameName);
		if (!blackSprite) return false;
		blackSprite->setColor({ 0, 0, 0 });

		rainbowSprite = CCSprite::createWithSpriteFrameName(frameName);
		blackSprite->addChild(rainbowSprite);
		rainbowSprite->setPosition(blackSprite->getContentSize() / 2.f);

		if (!CCMenuItemSpriteExtra::init(blackSprite, nullptr, this, nullptr)) 
			return false;

		this->label = CCLabelBMFont::create(label.c_str(), "Lemonmilk.fnt"_spr);
		this->label->limitLabelWidth(blackSprite->getContentWidth(), 0.4, 0.1);
		this->label->setAlignment(kCCTextAlignmentCenter);
		this->label->setPosition({ blackSprite->getContentWidth() / 2.f, -1.f });
		blackSprite->addChild(this->label);
		this->label->setOpacity(0.f);

		blackSprite->runAction(
			CCRepeatForever::create(
				CCSequence::createWithTwoActions(
					CCEaseInOut::create(
						CCScaleBy::create(scaleAnimationFadeDuration, 0.4), 2
					),
					CCEaseInOut::create(
						CCScaleBy::create(6 - scaleAnimationFadeDuration, 2.5), 2
					)
				)
			)
		);

		rainbowSprite->runAction(
			CCRepeatForever::create(
				CCSequence::createWithTwoActions(
					CCEaseInOut::create(
						CCFadeOut::create(scaleAnimationFadeDuration), 2
					),
					CCEaseInOut::create(
						CCFadeIn::create(6 - scaleAnimationFadeDuration), 2
					)
				)
			)
		);


		glowEffect = ParticleSystemBuilder::buildGravity(200)
			.spawnArea({30.f, 30.f}) 
			.gravity({0.f, 0.f})  
			.angle(0.f, 180.f)     
			.lifetime(2.f, 0.15f)  
			.color(
				{1.f, 0.f, 0.f, 0.4f},
				{0.f, 0.f, 0.f, 0.f},  
				{1.f, 0.f, 0.f, 0.4f},
				{0.f, 0.f, 0.f, 0.f}   
			)
			.size(21.f, 11.f, 1.f, 20.f)
			.speed(0.f, 5.f)
			.radialAccel(1.f, 5.f)
			.tangentialAccel(0.f, 0.f)
			.emissionRate(200.f / 2.f)
			.texture("glow.png"_spr)
			.get();

		glowEffect->runAction(
			CCRepeatForever::create(
				CCSequence::createWithTwoActions(
					CCEaseInOut::create(
						CCScaleTo::create(scaleAnimationFadeDuration, 0.4), 2
					),
					CCEaseInOut::create(
						CCScaleTo::create(6 - scaleAnimationFadeDuration, 0.6), 2
					)
				)
			)
		);
		glowEffect->setBlendAdditive(true);
		glowEffect->setPosition(blackSprite->getContentSize() / 2.f);
		blackSprite->addChild(glowEffect, -1);

		return true;
	}

	virtual void onEnter() override {
        CCMenuItemSpriteExtra::onEnter();
        this->scheduleUpdate();
    }
	
	void update(float dt) override {
		CCMenuItemSpriteExtra::update(dt);
		auto rcgColor = rcg.get(dt);
		ccColor3B color = ccc3(
			(GLubyte)(rcgColor.r * 255.0f),
			(GLubyte)(rcgColor.g * 255.0f),
			(GLubyte)(rcgColor.b * 255.0f)
		);
		rainbowSprite->setColor(color);
		label->setColor(color);
		glowEffect->setStartColor({ rcgColor.r, rcgColor.g, rcgColor.b, 0.4f });
		glowEffect->setEndColor({ rcgColor.r, rcgColor.g, rcgColor.b, 0.4f });

	}
public:
	void setScale(float scale) override {
		this->blackSprite->setScale(scale);
	}

	void hideRainbowAndGlow() {
		this->rainbowSprite->setVisible(false);
		this->glowEffect->setVisible(false);
	}

	void showRainbowAndGlow() {
		this->rainbowSprite->setVisible(true);
		this->glowEffect->setVisible(true);
	}

	static TreeButton* create(const char* frameName, std::string label, float scaleAnimationFadeDuration, std::function<void()> callback) {
		auto ret = new TreeButton(scaleAnimationFadeDuration);
		if (ret && ret->init(frameName, label, scaleAnimationFadeDuration, callback)) {
			ret->autorelease();
			return ret;
		}
		delete ret;
		return nullptr;
	}
};
