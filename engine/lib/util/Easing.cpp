/*
 * The MIT License (MIT)
 * Copyright (c) 2011 GClue, inc.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Easing.h"

#include <math.h>


static float linear(float t, float b, float c, float d) {
  return c * t / d + b;
}

static float easingQuadIn(float t, float b, float c, float d) {
	t /= d;
	return c * t * t + b;
}

static float easingQuadOut(float t, float b, float c, float d) {
	t /= d;
	return -c * t * (t - 2) + b;
}

static float easingQuadInOut(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1) {
		return c / 2 * t * t + b;
	}
	t--;
	return -c / 2 * (t * (t - 2) - 1) + b;
}

static float easingOutBounce(float t, float b, float c, float d) {
	if ((t /= d) < (1 / 2.75)) {
		return c * (7.5625 * t * t) + b;
	} else if (t < (2 / 2.75)) {
		return c * (7.5625 * (t -= (1.5 / 2.75)) * t + .75) + b;
	} else if (t < (2.5 / 2.75)) {
		return c * (7.5625 * (t -= (2.25 / 2.75)) * t + .9375) + b;
	} else {
		return c * (7.5625 * (t -= (2.625 / 2.75)) * t + .984375) + b;
	}
}

static float easingInBounce(float t, float b, float c, float d) {
	return c - easingOutBounce(d - t, 0, c, d) + b;
}

static float easingInOutBounce(float t, float b, float c, float d) {
	if (t < d / 2) {
		return easingInBounce(t * 2, 0, c, d) * .5 + b;
	} else {
		return easingOutBounce(t * 2 - d, 0, c, d) * .5 + c * .5 + b;
	}
}

static float easingInBack(float t, float b, float c, float d) {
	float s = 1.70158;
	return c * (t /= d) * t * ((s + 1) * t - s) + b;
}

static float easingOutBack(float t, float b, float c, float d) {
	float s = 1.70158;
	return c * ((t = t / d - 1) * t * ((s + 1) * t + s) + 1) + b;
}

static float easingInOutBack(float t, float b, float c, float d) {
	float s = 1.70158;
	if ((t /= d / 2) < 1) {
		return c / 2 * (t * t * (((s *= (1.525)) + 1) * t - s)) + b;
	} else {
		return c / 2 * ((t -= 2) * t * (((s *= (1.525)) + 1) * t + s) + 2) + b;
	}
}

static float easingCubicIn(float t, float b, float c, float d) {
	return c * (t /= d) * t * t + b;
}

static float easingCubicOut(float t, float b, float c, float d) {
	return c * ((t = t / d - 1) * t * t + 1) + b;
}

static float easingCubicInOut(float t, float b, float c, float d) {
	return ((t /= d / 2) < 1) ? c / 2 * t * t * t + b : c / 2 * ((t -= 2) * t * t + 2) + b;
}


static float easingExpoIn(float t, float b, float c, float d) {
	return t == 0 ? b : c * pow(2, 10 * (t / d - 1)) + b;
}

static float easingExpoOut(float t, float b, float c, float d) {
	return t == d ? b + c : c * (1 - pow(2, -10 * t / d)) + b;
}

static float easingExpoInOut(float t, float b, float c, float d) {
	if (t == 0) {
		return b;
	}
	if (t == d) {
		return b + c;
	}
	if ((t /= d / 2.0) < 1.0) {
		return c / 2 * pow(2, 10 * (t - 1)) + b;
	}
	return c / 2 * (2 - pow(2, -10 * --t)) + b;
}


static float easingQuartIn(float t, float b, float c, float d) {
	return c * (t /= d) * t * t * t + b;
}

static float easingQuartOut(float t, float b, float c, float d) {
	return -c * ((t = t / d - 1) * t * t * t - 1) + b;
}

static float easingQuartInOut(float t, float b, float c, float d) {
	if ((t /= d / 2) < 1) {
		return c / 2 * t * t * t * t + b;
	}
	return -c / 2 * ((t -= 2) * t * t * t - 2) + b;
}

static float easingQuintIn(float t, float b, float c, float d) {
	return c * (t /= d) * t * t * t * t + b;
}

static float easingQuintOut(float t, float b, float c, float d) {
	return c * ((t = t / d - 1) * t * t * t * t + 1) + b;
}

static float easingQuintInOut(float t, float b, float c, float d) {
	if ((t /= d / 2) < 1) {
		return c / 2 * t * t * t * t * t + b;
	}
	return c / 2 * ((t -= 2) * t * t * t * t + 2) + b;
}

static float easingSineIn(float t, float b, float c, float d) {
	return -c * cos(t / d * (M_PI / 2)) + c + b;
}

static float easingSineOut(float t, float b, float c, float d) {
	return c * sin(t / d * (M_PI / 2)) + b;
}

static float easingSineInOut(float t, float b, float c, float d) {
	return -c / 2 * (cos(M_PI * t / d) - 1) + b;
}


Easing::Easing(int type) {
	switch (type) {
	default:
	case EASING_LINEAR:
		ease = linear;
		break;
	case EASING_IN_QUAD:
		ease = easingQuadIn;
		break;
	case EASING_OUT_QUAD:
		ease = easingQuadOut;
		break;
	case EASING_IN_OUT_QUAD:
		ease = easingQuadInOut;
		break;
	case EASING_IN_BOUNCE:
		ease = easingInBounce;
		break;
	case EASING_OUT_BOUNCE:
		ease = easingOutBounce;
		break;
	case EASING_IN_OUT_BOUNCE:
		ease = easingInOutBounce;
		break;
	case EASING_IN_BACK:
		ease = easingInBack;
		break;
	case EASING_OUT_BACK:
		ease = easingOutBack;
		break;
	case EASING_IN_OUT_BACK:
		ease = easingInOutBack;
		break;
	case EASING_IN_CUBIC:
		ease = easingCubicIn;
		break;
	case EASING_OUT_CUBIC:
		ease = easingCubicOut;
		break;
	case EASING_IN_OUT_CUBIC:
		ease = easingCubicInOut;
		break;
	case EASING_IN_EXPO:
		ease = easingExpoIn;
		break;
	case EASING_OUT_EXPO:
		ease = easingExpoOut;
		break;
	case EASING_IN_OUT_EXPO:
		ease = easingExpoInOut;
		break;
	case EASING_IN_QUART:
		ease = easingQuartIn;
		break;
	case EASING_OUT_QUART:
		ease = easingQuartOut;
		break;
	case EASING_IN_OUT_QUART:
		ease = easingQuartInOut;
		break;
	case EASING_IN_QUINT:
		ease = easingQuintIn;
		break;
	case EASING_OUT_QUINT:
		ease = easingQuintOut;
		break;
	case EASING_IN_OUT_QUINT:
		ease = easingQuintInOut;
		break;
	case EASING_IN_SINE:
		ease = easingSineIn;
		break;
	case EASING_OUT_SINE:
		ease = easingSineOut;
		break;
	case EASING_IN_OUT_SINE:
		ease = easingSineInOut;
		break;
	}
}

Easing::~Easing() {
}

float Easing::easing(double t) {
	if (t > duration) {
		return begin + delta;
	}
	return ease(t, begin, delta, duration);
}
