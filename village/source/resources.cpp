#include "config_file.h"
#include "image_8.h"
#include "image_32.h"
#include "resources.h"

void initialise_multi_manager(multi_manager& mm)
{
  auto image_resource_manager = std::make_unique<resource_manager<image>>( 
    resource_manager<image>::default_loader,
    [](){ return std::make_shared<image_32>(); }
  );
  mm.add_resource_manager(".png", std::move(image_resource_manager));

  auto config_file_resource_manager = std::make_unique<resource_manager<config_file>>( 
    resource_manager<config_file>::default_loader,
    [](){ return std::make_shared<text_config_file>(); }
  );
  mm.add_resource_manager(".txt", std::move(config_file_resource_manager));
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



