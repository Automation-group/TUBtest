
MACRO (lfQT4_WRAP_CPP outfiles outdir )
  QT4_GET_MOC_INC_DIRS(moc_includes)
  FOREACH (it ${ARGN})
    GET_FILENAME_COMPONENT(it ${it} ABSOLUTE)
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    SET(outfile ${outdir}/moc_${outfile}.cxx)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      COMMAND ${QT_MOC_EXECUTABLE}
      ARGS ${moc_includes} -o ${outfile} ${it}
      DEPENDS ${it})
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH(it)
ENDMACRO (lfQT4_WRAP_CPP)

MACRO(lfQT4_WRAP_UI outfiles outdir )
  FOREACH(it ${ARGN})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile ${outdir}/ui_${outfile}.h)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
        COMMAND ${QT_UIC_EXECUTABLE}
        ARGS -DQT_NO_KEYWORDS -o ${outfile} ${infile}
        MAIN_DEPENDENCY ${infile})
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH(it)
ENDMACRO(lfQT4_WRAP_UI)


MACRO (lfQT4_ADD_RESOURCES outfiles outdir )
  FOREACH (it ${ARGN})
    GET_FILENAME_COMPONENT(outfilename ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    GET_FILENAME_COMPONENT(rc_path ${infile} PATH)
    SET(outfile ${outdir}/qrc_${outfilename}.cxx)
    #  parse file for dependencies
    FILE(READ "${infile}" _RC_FILE_CONTENTS)
    STRING(REGEX MATCHALL "<file>[^<]*" _RC_FILES "${_RC_FILE_CONTENTS}")
    SET(_RC_DEPENDS)
    FOREACH(_RC_FILE ${_RC_FILES})
      STRING(REGEX REPLACE "^<file>" "" _RC_FILE "${_RC_FILE}")
      SET(_RC_DEPENDS ${_RC_DEPENDS} "${rc_path}/${_RC_FILE}")
    ENDFOREACH(_RC_FILE)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      COMMAND ${QT_RCC_EXECUTABLE}
      ARGS -name ${outfilename} -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile}
      DEPENDS ${_RC_DEPENDS})
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH (it)
ENDMACRO (lfQT4_ADD_RESOURCES)