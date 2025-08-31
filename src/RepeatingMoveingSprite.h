#pragma once
#include "./includes.h"

class RepeatingMovingSprite : public CCNode {
    CCSprite* sprite;
    CCPoint startPoint;
    CCPoint endPoint;
    float moveDuration;
    std::function<void(RepeatingMovingSprite* self)> onCycleComplete;

public:
    RepeatingMovingSprite() : sprite(nullptr), moveDuration(1.f) {}

    bool initWithSpriteFrameName(
        const char* frameName,
        const CCPoint& start,
        const CCPoint& end,
        float duration,
        std::function<void(RepeatingMovingSprite* self)> callback = nullptr
    ) {
        if (!CCNode::init()) return false;

        sprite = CCSprite::createWithSpriteFrameName(frameName);
        if (!sprite) return false;

        this->addChild(sprite);

        startPoint = start;
        endPoint = end;
        moveDuration = duration;
        onCycleComplete = std::move(callback);

        startCycle();
        return true;
    }

    bool initWithFile(
        const char* filename,
        const CCPoint& start,
        const CCPoint& end,
        float duration,
        std::function<void(RepeatingMovingSprite* self)> callback = nullptr
    ) {
        if (!CCNode::init()) return false;

        sprite = CCSprite::create(filename);
        if (!sprite) return false;

        this->addChild(sprite);

        startPoint = start;
        endPoint = end;
        moveDuration = duration;
        onCycleComplete = std::move(callback);

        startCycle();
        return true;
    }

    void setStartEndPoints(const CCPoint& start, const CCPoint& end) {
        startPoint = start;
        endPoint = end;
    }

    void setMoveDuration(float duration) {
        moveDuration = duration;
    }

private:
    void startCycle() {
        this->stopAllActions();
		sprite->setPosition(startPoint);
        auto moveForward = CCMoveTo::create(moveDuration, endPoint);
        auto callback    = CCCallFunc::create(this, callfunc_selector(RepeatingMovingSprite::handleCycleComplete));
        auto sequence = CCSequence::create(moveForward, callback, nullptr);
        sprite->runAction(sequence);
    }

    void handleCycleComplete() {
        if (onCycleComplete) {
            onCycleComplete(this);
        }
        startCycle();
    }

public:
	CCSprite* getSprite() const { return sprite; }
    static RepeatingMovingSprite* createWithSpriteFrameName(
        const char* frameName,
        const CCPoint& start,
        const CCPoint& end,
        float duration,
        std::function<void(RepeatingMovingSprite* self)> callback = nullptr
    ) {
        auto ret = new RepeatingMovingSprite();
        if (ret && ret->initWithSpriteFrameName(frameName, start, end, duration, callback)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    static RepeatingMovingSprite* createWithFile(
        const char* filename,
        const CCPoint& start,
        const CCPoint& end,
        float duration,
        std::function<void(RepeatingMovingSprite* self)> callback = nullptr
    ) {
        auto ret = new RepeatingMovingSprite();
        if (ret && ret->initWithFile(filename, start, end, duration, callback)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};
