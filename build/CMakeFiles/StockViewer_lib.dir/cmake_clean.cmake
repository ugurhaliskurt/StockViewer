file(REMOVE_RECURSE
  "libStockViewer_lib.pdb"
  "libStockViewer_lib.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/StockViewer_lib.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
