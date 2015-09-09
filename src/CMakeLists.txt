add_subdirectory(tktk)

if(TKTK_BUILD_UNIT_TESTS)
    add_subdirectory(tests)
endif(TKTK_BUILD_UNIT_TESTS)

if(TKTK_BUILD_ARCANOID)
    add_subdirectory(arcanoid)
endif(TKTK_BUILD_ARCANOID)
