set (lib_name CommonLib)

set (lib_src MySocket.cpp
            Cmd_Off.cpp
            Cmd_Off.h
            cmd_on.cpp
            cmd_on.h
            Cmd_SetColor.cpp
            Cmd_SetColor.h
            DataStreamParser.cpp
            DataStreamParser.h
            data_type.h
            iCmd.h
            MySocket.cpp
            MySocket.h
            Parsing.cpp
            ParserMap.cpp
            ParserMap.h
            )

add_library (${lib_name} SHARED ${lib_src})


