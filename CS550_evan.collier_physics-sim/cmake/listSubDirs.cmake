# Gets all subdirectories of a given directory
MACRO(listSubDirs result curdir)
    FILE(GLOB_RECURSE children LIST_DIRECTORIES true
         ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${child})
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()