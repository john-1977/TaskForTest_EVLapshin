set (NAME my_client)

set (SRC    client.cpp

        )

set (HEADERS ../common/MySocket.h)


add_executable( ${NAME} ${SRC} ${HEADERS})

target_include_directories (${NAME}
                            PRIVATE
                            ${PLATFORM_ROOT_DIRECTORY}/src/common
                        )

target_link_libraries (${NAME} PRIVATE CommonLib
                        )
