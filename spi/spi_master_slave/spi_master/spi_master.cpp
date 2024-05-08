#include <cstring>

#include "loop/common_data/common_variables.hpp"
#include "loop/main_core.hpp"
#include "utilities/hardcoded_functions.hpp"
#include "physical_devices/scanner.hpp"

uint32_t DEBUG_LEVEL = 2;

int start_app()
{
  if (std::strcmp(HARDWAREVERSION.c_str(),"0.1")) { scanner=new  Scanner(confighardwarev0); }
  else                                            { scanner=new  Scanner(confighardwarev1); }
 // multicore_launch_core1(RX_core::launchOnCore1);
  scanner->hardware->setDefaultSettings();
  critical_section_init(&criticalSection);
  if (!critical_section_is_initialized(&criticalSection))
  {
    scanner->hardware->activateError();
  }
  
  MainCore mainCore;
  mainCore.loop();
  delete(scanner);
  return 0;
}

int main()
{
  return start_app();
}
