#pragma once
#include "./includes.h"

template<typename T>
struct ParticleStartEndValue{
	T start{};
	T end{};

	constexpr ParticleStartEndValue() = default;
	constexpr ParticleStartEndValue(T startAndEnd) : start(startAndEnd), end(startAndEnd){}
	constexpr ParticleStartEndValue(T start, T end) : start(start), end(end){}
};

template<typename T>
struct ParticleVariationValue {
	T value{};
	T variation{};

	constexpr ParticleVariationValue() = default;
	constexpr ParticleVariationValue(T v) : value(v), variation(static_cast<T>(0)){}
	constexpr ParticleVariationValue(T v, T var) : value(v), variation(var){}
};

template<typename T>
using ParticleStartEndVariationValue = ParticleStartEndValue<ParticleVariationValue<T>>;


#define APPLY_START_END_VARIATION(ret, start, startVar, end, endVar, Name) \
    do { \
        (ret)->setStart##Name((start)); \
        (ret)->setStart##Name##Var((startVar)); \
        (ret)->setEnd##Name((end)); \
        (ret)->setEnd##Name##Var((endVar)); \
    } while(0)

#define APPLY_START_END(ret, start, end, Name) \
    do { \
        (ret)->setStart##Name((start)); \
        (ret)->setEnd##Name((end)); \
    } while(0)

#define APPLY_VARIATION(ret, value, var, Name) \
    do { \
        (ret)->set##Name((value)); \
        (ret)->set##Name##Var((var)); \
    } while(0)


/*

        particleSystem = CCParticleSystemQuad::create();
        particleSystem->initWithTotalParticles(maxParticles, true);
        this->texture("square.png")
*/



template <typename Derived>
class BaseParticleEditor {
protected:
	CCParticleSystemQuad* particleSystem;
	BaseParticleEditor(CCParticleSystemQuad* ps) : particleSystem(ps){}

    Derived& self() { return static_cast<Derived&>(*this); }
public:
	BaseParticleEditor(const BaseParticleEditor&) = delete;
	BaseParticleEditor& operator=(const BaseParticleEditor&) = delete;
    BaseParticleEditor(BaseParticleEditor&& other) noexcept
        : particleSystem(other.particleSystem) {
        other.particleSystem = nullptr;
    }

    BaseParticleEditor& operator=(BaseParticleEditor&& other) noexcept {
        if (this != &other) {
            particleSystem = other.particleSystem;
            other.particleSystem = nullptr;
        }
        return *this;
    }

	auto& size(float start, float startVar, float end, float endVar) {
		APPLY_START_END_VARIATION(particleSystem, start, startVar, end, endVar, Size);	
		return self();
	}

	auto& spin(float start, float startVar, float end, float endVar) {
        APPLY_START_END_VARIATION(particleSystem, start, startVar, end, endVar, Spin);
        return self();
    }

    auto& color(const ccColor4F& start, const ccColor4F& startVar, const ccColor4F& end, const ccColor4F& endVar) {
        APPLY_START_END_VARIATION(particleSystem, start, startVar, end, endVar, Color);
        return self();
    }

    auto& lifetime(float value, float variation) {
        APPLY_VARIATION(particleSystem, value, variation, Life);
        return self();
    }

    auto& duration(float value) {
        particleSystem->setDuration(value);
        return self();
    }

    auto& emissionRate(float value) {
        particleSystem->setEmissionRate(value);
        return self();
    }

	auto& spawnArea(const CCSize& size) {
		particleSystem->setPosVar({ size.width / 2.f, size.height / 2.f });
		return self();
	}

	auto& position(const CCPoint& pos) {
		particleSystem->setPosition(pos);
		return self();
	}

    auto& angle(float value, float variation) {//speedDirection for GravityParticles and startAngle for RadiusParticles
        APPLY_VARIATION(particleSystem, value, variation, Angle);
        return self();
    }

    auto& texture(const std::string& path) {
        particleSystem->setTexture(CCTextureCache::sharedTextureCache()->addImage(path.c_str(), false));
        return self();
    }

    CCParticleSystemQuad* get() {
        return particleSystem;
    }
};


class GravityParticleEditor : public BaseParticleEditor<GravityParticleEditor> {
public:
    GravityParticleEditor(CCParticleSystemQuad* ps) : BaseParticleEditor(ps) {
        particleSystem->setEmitterMode(kCCParticleModeGravity);
    }

    auto& gravity(const CCPoint& g) {
        particleSystem->setGravity(g);
        return *this;
    }

    auto& speed(float value, float variation) {
        APPLY_VARIATION(particleSystem, value, variation, Speed);
        return *this;
    }

    auto& radialAccel(float value, float variation) {
        APPLY_VARIATION(particleSystem, value, variation, RadialAccel);
        return *this;
    }

    auto& tangentialAccel(float value, float variation) {
        APPLY_VARIATION(particleSystem, value, variation, TangentialAccel);
        return *this;
    }

    auto& rotationIsDir(bool value) {
        particleSystem->setRotationIsDir(value);
        return *this;
    }
};

class RadiusParticleEditor : public BaseParticleEditor<RadiusParticleEditor> {
public:
    RadiusParticleEditor(CCParticleSystemQuad* ps) : BaseParticleEditor(ps) {
        particleSystem->setEmitterMode(kCCParticleModeRadius);
    }

    auto& radius(float start, float startVar, float end, float endVar) {
        APPLY_START_END_VARIATION(particleSystem, start, startVar, end, endVar, Radius);
        return *this;
    }

    auto& rotationPerSecond(float value, float variation) {
        APPLY_VARIATION(particleSystem, value, variation, RotatePerSecond);
        return *this;
    }
};


class ParticleSystemBuilder {
    static CCParticleSystemQuad* getBasic(unsigned int maxParticles) {
        auto ps = CCParticleSystemQuad::create();
        ps->initWithTotalParticles(maxParticles, true);
        GravityParticleEditor ret(ps);
        ret.texture("square.png")
            .angle(0.f, 360.f)
            .lifetime(1.f, 0.f)
            .emissionRate(maxParticles / 1.f)
            .size(5.f, 0.f, 5.f, 0.f)
            .duration(-1.f)
            .color(
				{ 1.0f, 1.f, 1.f, 1.0f }, { 0,0,0,0 },
				{ 1.0f, 1.f, 1.0f, 0.0f }, { 0,0,0,0 }
            );
        return ret.get();
    }
public:
    static GravityParticleEditor buildGravity(unsigned int maxParticles) {
        GravityParticleEditor ret(getBasic(maxParticles));
        ret.gravity({ 0, -250 });
        return std::move(ret);
    }

    static RadiusParticleEditor buildRadius(unsigned int maxParticles) {
        RadiusParticleEditor ret(getBasic(maxParticles));
        //TODO basic radius init
        return ret;
    }
};
