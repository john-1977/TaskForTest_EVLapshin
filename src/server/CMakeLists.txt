set (NAME my_server)

set (SOURCE server.cpp )
#../common/MySocket.cpp)

#set (HEADERS MyServer.h)


add_executable (${NAME} ${SOURCE} )
#${HEADER})

target_include_directories (${NAME}
                            PRIVATE
                            ${PLATFORM_ROOT_DIRECTORY}/src/common
                        )

target_link_libraries (${NAME} PRIVATE CommonLib
                        )
