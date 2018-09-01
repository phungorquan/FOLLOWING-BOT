DW1000

Dựa trên code mbed BLE_Beacon ở https://github.com/ARMmbed/mbed-os-example-ble

Cài mbed tool trên máy
Vào thư mục chạy:
    mbed deploy
    # Sửa cấu hình để phù hợp với mạch nrf52
    cp targets.json mbed-os/targets/targets.json

Trong thư mục source có hai file main_anchor.cpp và main_tag.cpp, xóa bỏ hoặc
comment toàn bộ một file (ví dụ build code cho thẻ thì comment toàn bộ file
main_anchor.cpp và bỏ comment main_tag.cpp nếu cần) rồi chạy lệnh sau để build:
    mbed compile -t GCC_ARM -m NRF52_DK

Chuyển code lên thiết bị:
    cp ./.build/NRF52_DK/GCC_ARM/mbed5.6.hex /media/*/DAPLINK

Xem thông tin debug nếu có:
    sudo microcom -p /dev/ttyACM0 -s 115200

Trong file source/main_anchor.cpp và source/main_tag.cpp cần chỉnh address cho
mỗi thiết bị với address là integer, address anchor đang cố định là từ 1 đến 3, 
tag là từ 4 trở đi, tìm đến dòng có node.address, ví dụ để address là 9 trong
source/main_tag.cpp:
    node.address = 9;

Tag sẽ print liên tục khoảng cách từ nó đến anchor.

