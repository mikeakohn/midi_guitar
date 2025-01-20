idf_component_register(
  SRCS
    Audio.cpp
    DCT.cpp
    FreqCalc.cpp
    Notes.cpp
    Processor.cpp
    UART.cpp
    guitarstick.cpp
  INCLUDE_DIRS "")

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-format)

