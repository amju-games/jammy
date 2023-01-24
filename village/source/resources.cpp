#include "image_8.h"
#include "image_32.h"
#include "resources.h"

void initialise_multi_manager(multi_manager& mm)
{
  resource_manager<image> image_resource_manager(
    resource_manager<image>::default_loader,
    [](){ return std::make_shared<image_8>(); }
  );
  mm.add_resource_manager(".png", image_resource_manager);
}

multi_manager& resources()
{
  static multi_manager mm;

  [[maybe_unused]] static bool do_this_once_only = []()
  {
    initialise_multi_manager(mm);
    return true; 
  }();

  return mm;
}



