# Also link against libz for static's sake.
SET(FREETYPE_LIBRARIES ${FREETYPE_LIBRARIES} z)
SET_TARGET_PROPERTIES( noggit PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/media/Info.plist" )

# Copy the binary to bin/.
INSTALL(TARGETS noggit
  BUNDLE DESTINATION ${CMAKE_CURRENT_BINARY_DIR} COMPONENT Runtime
  RUNTIME DESTINATION bin COMPONENT Runtime
)

INCLUDE(InstallRequiredSystemLibraries)

SET(APPS "${CMAKE_CURRENT_BINARY_DIR}/noggit.app")

SET(DIRS ${OPENGL_LIBRARY_DIR} ${FREETYPE_LIBRARY_DIR} ${SDL_LIBRARY_DIR} ${STORMLIB_LIBRARY_DIR} ${Boost_LIBRARY_DIR} ${GLEW_LIBRARY_DIR})
INSTALL(CODE " include(BundleUtilities) fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\" " COMPONENT Runtime)

SET(CPACK_BUNDLE_NAME "Noggit 3")
SET(CPACK_BUNDLE_PLIST "${CMAKE_SOURCE_DIR}/media/Info.plist")
SET(CPACK_BUNDLE_ICON "${CMAKE_SOURCE_DIR}/media/noggit.icns")
SET(CPACK_DMG_VOLUME_NAME "Noggit 3")
SET(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_SOURCE_DIR}/media/noggit.png")
SET(CPACK_GENERATOR "DragNDrop")
SET(CPACK_PACKAGE_FILE_NAME "Noggit 3")
SET(CPACK_PACKAGE_VERSION "SDL 1.3")
INCLUDE(CPack)