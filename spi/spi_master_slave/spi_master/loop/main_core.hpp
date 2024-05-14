#ifndef PICO_EXAMPLES_MAIN_CORE_HPP
#define PICO_EXAMPLES_MAIN_CORE_HPP


class MainCore
{
 public:

  MainCore();

  void loop();

 private:
  static void parse(std::vector<int32_t> &vec);
  static void parse(std::vector<int32_t> &vec,std::vector<int32_t> &vparams);
  static void launchOnCore1();
  void setAlgCodeNone();
};


#endif
