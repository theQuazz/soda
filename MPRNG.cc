#include "MPRNG.h"

MPRNG &get_random(void) {
  static MPRNG rng;
  return rng;
}
