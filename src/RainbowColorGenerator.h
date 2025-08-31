#pragma once
#include <algorithm>

struct RGB {
	float r, g, b;
};

struct HSL {
	float h, s, l;
};

class RainbowColorGenerator {
public:
    RainbowColorGenerator(float rainbowPeriodSeconds)
        : period(rainbowPeriodSeconds), hue(0.0f)
    {}

    RGB get(float dt) {
        hue += 360.0f * (dt / period);
        if (hue >= 360.0f) hue -= 360.0f;
        if (hue < 0.0f)   hue += 360.0f;

        HSL hsl { hue, 1.0f, 0.5f };
        return HSLtoRGB(hsl);
    }

private:
    float period;
    float hue;

    static RGB HSLtoRGB(const HSL& hsl) {
        float C = (1.0f - std::fabs(2.0f * hsl.l - 1.0f)) * hsl.s;
        float X = C * (1.0f - std::fabs(fmod(hsl.h / 60.0f, 2.0f) - 1.0f));
        float m = hsl.l - C / 2.0f;

        float r = 0, g = 0, b = 0;
        if (hsl.h < 60)      { r = C; g = X; }
        else if (hsl.h < 120){ r = X; g = C; }
        else if (hsl.h < 180){ g = C; b = X; }
        else if (hsl.h < 240){ g = X; b = C; }
        else if (hsl.h < 300){ r = X; b = C; }
        else                 { r = C; b = X; }

        return { r + m, g + m, b + m };
    }
};
