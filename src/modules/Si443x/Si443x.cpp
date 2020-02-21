#include "Si443x.h"

Si443x::Si443x(Module* mod) : PhysicalLayer(SI443X_FREQUENCY_STEP_SIZE, SI443X_MAX_PACKET_LENGTH) {
  _mod = mod;
}
