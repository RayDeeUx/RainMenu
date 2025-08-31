#pragma once
#include "./includes.h"

CCAction* shakeAction(int count = 4, float strength = 0.3f, float speed = 0.05f) {
	auto moveLeft = CCMoveBy::create(speed, { -strength, 0 });
	auto moveRight = moveLeft->reverse();
	return CCRepeat::create(
		CCSequence::create(
			moveLeft, moveRight, nullptr
		), count
	);
}