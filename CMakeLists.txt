cmake_minimum_required(VERSION 3.10) # 指定CMake的最小版本要求

project(dsp) # 定義項目名稱

# 創建一個靜態庫 "helloworld"
add_library(helloworld STATIC helloworld.c)
# 將 data_processing.c 編譯為靜態庫
add_library(data_processing STATIC data_processing.c)



# 創建一個執行文件 "main"
add_executable(main main.c)




# 鏈接靜態庫 "helloworld" 到執行文件 "main"
target_link_libraries(main helloworld)
# 鏈接 data_processing 庫到主程序
target_link_libraries(main data_processing)