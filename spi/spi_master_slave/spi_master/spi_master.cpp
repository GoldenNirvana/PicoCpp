#include "loop/common_data/common_variables.hpp"
#include "loop/main_core.hpp"
#include "utilities/peripheral_functions.hpp"

int start_app()
{
// mb need to delete crit_section
  critical_section_init(&criticalSection);
  if (!critical_section_is_initialized(&criticalSection))
  {
    activateError();
  }
  dark();
  setDefaultSettings();
  MainCore mainCore;
  mainCore.loop();
  return 0;
}

int main()
{
  return start_app();
}
