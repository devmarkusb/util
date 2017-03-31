option(DEPLOYMENT_BUILD "do a final release build for deployment" $ENV{DeployArtifacts})

set(DEPLOY_TARGET "" CACHE STRING "Special deployment choice (if OS is not sufficient) out of: winstore, ... or download (default, also if left empty)")
if (DEPLOY_TARGET STREQUAL "winstore")
    add_definitions(-DTOO_DEPLOY_TO_WINSTORE)
#elseif (DEPLOY_TARGET STREQUAL "steam")
#    add_definitions(-DTOO_DEPLOY_TO_STEAM)
endif ()
