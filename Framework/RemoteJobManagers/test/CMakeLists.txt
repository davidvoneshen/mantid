if(CXXTEST_FOUND)
  include_directories(SYSTEM
                      ${CXXTEST_INCLUDE_DIR}
)

  cxxtest_add_test(RemoteJobManagersTest ${TEST_FILES})
  target_link_libraries(RemoteJobManagersTest
                        LINK_PRIVATE
                        ${TCMALLOC_LIBRARIES_LINKTIME}
                        API
                        Types
                        Kernel
                        RemoteJobManagers
                        ${Boost_LIBRARIES}
                        ${POCO_LIBRARIES}
                        gmock
)
  add_dependencies(FrameworkTests RemoteJobManagersTest)
  # Test data. Not using any for now. Remember to uncomment if data is added for
  # these remote job managers add_dependencies ( RemoteJobManagersTest
  # StandardTestData )

  # Add to the 'FrameworkTests' group in VS
  set_property(TARGET RemoteJobManagersTest PROPERTY FOLDER "UnitTests")

endif()
