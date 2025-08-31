#pragma once
#include "./includes.h"

enum class ScaleMode {
	Cover, Contain
};

static void scaleToFit(CCNode* sprite, const CCSize& containerSize, ScaleMode mode) {
    if (!sprite) return;

    auto s = sprite->getContentSize();

    float scaleX = containerSize.width  / s.width;
    float scaleY = containerSize.height / s.height;

    float scale = (mode == ScaleMode::Cover)
        ? std::max(scaleX, scaleY)
        : std::min(scaleX, scaleY);

    sprite->setScale(scale);
}
