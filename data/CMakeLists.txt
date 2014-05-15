project(data)

qtcreator_add_project_resources(${CMAKE_CURRENT_SOURCE_DIR})

install(DIRECTORY ./ DESTINATION ${CMAKE_INSTALL_DATADIR}/data)
