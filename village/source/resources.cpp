#include "config_file.h"
#include "image_8.h"
#include "image_32.h"
#include "nice_font.h"
#include "resources.h"

void initialise_multi_manager(multi_manager& mm)
{
  auto nice_font_resource_manager = std::make_unique<resource_manager<nice_font>>( 
    resource_manager<nice_font>::default_loader,
    [](){ return std::make_shared<nice_font>(); }
  );
  mm.add_resource_manager(".font", std::move(nice_font_resource_manager));

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



