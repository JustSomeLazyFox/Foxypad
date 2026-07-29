#include "mathutils.h"

#include <algorithm>
#include <linux/input-event-codes.h>
#include <vector>

bool isInRange(float value, float min, float max) { return value >= min && value <= max; }

bool isKeypadKey(int keyCode) {
  // clang-format off
  const std::vector<int> otherKeypadKeycodes = {
      KEY_KPASTERISK,
      KEY_KPJPCOMMA,
      KEY_KPENTER,
      KEY_KPSLASH,
      KEY_KPEQUAL,
      KEY_KPPLUSMINUS,
      KEY_KPCOMMA,
      KEY_KPLEFTPAREN,
      KEY_KPRIGHTPAREN
  };
  // // clang-format on
  bool isKeyCodeInOtherKeypadKeyCodes = std::ranges::find(otherKeypadKeycodes, keyCode) != otherKeypadKeycodes.end();
  return isInRange(keyCode, 71, 83) || isKeyCodeInOtherKeypadKeyCodes;
}