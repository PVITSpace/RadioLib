#ifndef _RADIOLIB_SI4432_H
#define _RADIOLIB_SI4432_H

#include "../../TypeDef.h"
#include "../../Module.h"
#include "Si443x.h"

/*!
  \class Si4432

  \brief Derived class for %Si4432 modules.
*/
class Si4432: public Si443x {
  public:

    // constructor

    /*!
      \brief Default constructor.

      \param mod Instance of Module that will be used to communicate with the radio chip.
    */
    Si4432(Module* mod);

#ifndef RADIOLIB_GODMODE
  protected:
#endif

#ifndef RADIOLIB_GODMODE
  private:
#endif
};

#endif
