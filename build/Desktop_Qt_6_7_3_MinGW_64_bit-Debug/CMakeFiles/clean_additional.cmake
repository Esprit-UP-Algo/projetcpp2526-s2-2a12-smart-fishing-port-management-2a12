# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\integration_finale_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\integration_finale_autogen.dir\\ParseCache.txt"
  "_modules\\GestionDesNavires\\CMakeFiles\\module_navires_autogen.dir\\AutogenUsed.txt"
  "_modules\\GestionDesNavires\\CMakeFiles\\module_navires_autogen.dir\\ParseCache.txt"
  "_modules\\GestionDesNavires\\module_navires_autogen"
  "_modules\\ferdawes\\CMakeFiles\\module_employees_autogen.dir\\AutogenUsed.txt"
  "_modules\\ferdawes\\CMakeFiles\\module_employees_autogen.dir\\ParseCache.txt"
  "_modules\\ferdawes\\module_employees_autogen"
  "_modules\\gestion_stock_frigo_capture\\CMakeFiles\\module_captures_autogen.dir\\AutogenUsed.txt"
  "_modules\\gestion_stock_frigo_capture\\CMakeFiles\\module_captures_autogen.dir\\ParseCache.txt"
  "_modules\\gestion_stock_frigo_capture\\CMakeFiles\\module_stockage_autogen.dir\\AutogenUsed.txt"
  "_modules\\gestion_stock_frigo_capture\\CMakeFiles\\module_stockage_autogen.dir\\ParseCache.txt"
  "_modules\\gestion_stock_frigo_capture\\captures\\lot_poisson\\CMakeFiles\\lot_poisson_module_autogen.dir\\AutogenUsed.txt"
  "_modules\\gestion_stock_frigo_capture\\captures\\lot_poisson\\CMakeFiles\\lot_poisson_module_autogen.dir\\ParseCache.txt"
  "_modules\\gestion_stock_frigo_capture\\captures\\lot_poisson\\lot_poisson_module_autogen"
  "_modules\\gestion_stock_frigo_capture\\module_captures_autogen"
  "_modules\\gestion_stock_frigo_capture\\module_stockage_autogen"
  "_modules\\gestion_stock_frigo_capture\\stockage_frigo\\stockage_ui\\CMakeFiles\\stockage_core_autogen.dir\\AutogenUsed.txt"
  "_modules\\gestion_stock_frigo_capture\\stockage_frigo\\stockage_ui\\CMakeFiles\\stockage_core_autogen.dir\\ParseCache.txt"
  "_modules\\gestion_stock_frigo_capture\\stockage_frigo\\stockage_ui\\CMakeFiles\\stockage_embed_autogen.dir\\AutogenUsed.txt"
  "_modules\\gestion_stock_frigo_capture\\stockage_frigo\\stockage_ui\\CMakeFiles\\stockage_embed_autogen.dir\\ParseCache.txt"
  "_modules\\gestion_stock_frigo_capture\\stockage_frigo\\stockage_ui\\stockage_core_autogen"
  "_modules\\gestion_stock_frigo_capture\\stockage_frigo\\stockage_ui\\stockage_embed_autogen"
  "_modules\\interface_raef\\CMakeFiles\\module_quais_autogen.dir\\AutogenUsed.txt"
  "_modules\\interface_raef\\CMakeFiles\\module_quais_autogen.dir\\ParseCache.txt"
  "_modules\\interface_raef\\module_quais_autogen"
  "_modules\\transaction\\CMakeFiles\\module_ventes_autogen.dir\\AutogenUsed.txt"
  "_modules\\transaction\\CMakeFiles\\module_ventes_autogen.dir\\ParseCache.txt"
  "_modules\\transaction\\module_ventes_autogen"
  "integration_finale_autogen"
  )
endif()
