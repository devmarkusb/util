find_library(JSON_SCHEMA_VALIDATOR_LIBRARY NAMES libnlohmann_json_schema_validator.a nlohmann_json_schema_validator REQUIRED)
message(STATUS "found json-schema-validator: ${JSON_SCHEMA_VALIDATOR_LIBRARY}")
add_library(libjson-schema-validator STATIC IMPORTED GLOBAL)
set_target_properties(libjson-schema-validator PROPERTIES
    IMPORTED_LOCATION ${JSON_SCHEMA_VALIDATOR_LIBRARY}
)
