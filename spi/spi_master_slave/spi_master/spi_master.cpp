#include "loop/common_data/common_variables.hpp"
#include "loop/main_core.hpp"
#include "utilities/hardcoded_functions.hpp"

uint32_t DEBUG_LEVEL = 2;

int start_app()
{
// mb need to delete crit_section
 
  scanner=new  Scanner;
  scanner->hardware->setDefaultSettings();
  //scanner->hardware->dark(); 
  critical_section_init(&criticalSection);
  if (!critical_section_is_initialized(&criticalSection))
  {
     scanner->hardware->activateError();
  }
  
  MainCore mainCore;
  mainCore.loop();
  return 0;
}

int main()
{
  return start_app();
}
