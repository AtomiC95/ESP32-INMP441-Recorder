set(SOURCES ./src/main.cpp ./src/Microphone.cpp ./src/SDcard.cpp)
idf_component_register(SRCS ${SOURCES}
                            INCLUDE_DIRS . inc
                            REQUIRES freertos esp_common fatfs esp_hw_support )