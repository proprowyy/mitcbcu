SET (CMAKE_SYSTEM_NAME eCos)

SET(CMAKE_C_COMPILER  /opt/ecos/gnutools/arm-eabi/bin/arm-eabi-gcc)
SET(CMAKE_CXX_COMPILER /opt/ecos/gnutools/arm-eabi/bin/arm-eabi-g++)

# here is the target environment located
#SET(CMAKE_FIND_ROOT_PATH   $ENV{PWD}  )
#SET(CMAKE_FIND_ROOT_PATH /home/benty/workspace/Projects/os/ecos/ProdCoreV2/os-PCV2/EAMP/3.x.x/3.x.x_install)
SET(CMAKE_FIND_ROOT_PATH /home/benty/workspace/Projects/os/ecos/freescale/cortexm/k70/core_k70150/ecos/ccu/install)
# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)