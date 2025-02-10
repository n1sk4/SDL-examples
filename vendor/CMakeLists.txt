add_subdirectory(SDL)

add_library(vendor INTERFACE)

add_library(qr_code STATIC
    QR-Code-generator/cpp/qrcodegen.cpp
)
target_include_directories(qr_code PUBLIC QR-Code-generator/cpp)


target_link_libraries(vendor INTERFACE SDL3::SDL3 qr_code)
