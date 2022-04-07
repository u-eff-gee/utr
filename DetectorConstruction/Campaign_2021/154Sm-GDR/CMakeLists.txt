# Register the CMake cache variable TARGET with type string and default value 154Sm if no value is already in the cache
set(TARGET "154Sm" CACHE STRING "Set the target to be used in the DetectorConstruction")

# Register the valid choices (PROPERTY STRINGS) in ccmake for the CMake cache variable TARGET
set_property(CACHE TARGET PROPERTY STRINGS "154Sm" "140Ce" "None")

# Define the preprocessor definition TARGET with the value being the CMake variable TARGET's value but with quotes added
# add_compile_definitions(TARGET="${TARGET}")

# Alternative to the preprocessor definition:
# Use configure_file to create the header DetectorConstructionConfig.hh from the template DetectorConstructionConfig.configure_file.hh, which then contains the definition of an proper c++ string variable TARGET
# Advantage: Is only in scope if this header is included and doesn't require a full project recompilation if the CMake variable TARGET is changed (unlike when using a global preprocessor definition).
# Disadvantage: The automatically generated header file pollutes the source directory (one could copy it to some directory in the build directory instead though).
configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/DetectorConstructionConfig.configure_file.hh"
  "${CMAKE_CURRENT_LIST_DIR}/DetectorConstructionConfig.hh"
)
