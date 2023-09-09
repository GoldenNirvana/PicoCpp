#include "loop/common_data/common_variables.hpp"
#include "loop/main_core.hpp"
#include "utilities/peripheral_functions.hpp"

int start_app()
{
  critical_section_init(&criticalSection);
  if (!critical_section_is_initialized(&criticalSection))
  {
    activateError();
  }

  setDefaultSettings();
  MainCore mainCore;
  mainCore.loop();
  return 0;
}

int main()
{
  return start_app();
}
