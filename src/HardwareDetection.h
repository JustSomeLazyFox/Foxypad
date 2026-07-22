#pragma once

#include <libudev.h>

#include <string>

std::string findTouchDeviceNode(struct udev *udevCtx);