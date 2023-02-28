# CMakeList.txt: CMakeProject1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
# cmake的默认路径为CMakeLists.txt 的路径
# cmake的最低版本
cmake_minimum_required (VERSION 3.8)

# 设置项目	名称
project(ProjectDemo LANGUAGES C CXX)

######第三方库
# 设置变量
set(OpenCV_DIR C://opencv/build)
set(Tensorflos_DIR E://libtensorflow-gpu-windows-x86_64-2.6.0)
#设置工程包含当前目录，非必须
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#设置自动生成moc文件,QT必备设置
set(CMAKE_AUTOMOC ON)
# 设置外部依赖包
find_package(OpenCV REQUIRED)
find_package(Qt5Widgets)  
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Network)
# 设置包含的目录
include_directories("src/Camer")
include_directories("src/Communication")
include_directories("src/Form Files")
include_directories("src/Header Files")
include_directories("src/Source Files")
include_directories("src/Tensorflow")
include_directories("src/UserAdmini")

include_directories(${OpenCV_INCLUDE_DIRS})   #Opencv
include_directories(${Tensorflos_DIR}/include)   #Tensorflow

include_directories("include file/Dalsa/CameraInterface")
include_directories("C://Program Files/Teledyne DALSA/Sapera/Classes/Basic")
include_directories("C://Program Files/Teledyne DALSA/Sapera/include")
# 设置链接库搜索目录
link_directories(${OpenCV_LIBRARY_DIRS})   #Opencv
link_directories(${Tensorflos_DIR}/lib)   #Tensorflow

link_directories("C://Program Files/Teledyne DALSA/Sapera/Lib/Win64")

#######可执行文件
#调用file glob命令通过通配符匹配所有的C++源文件 并存放在SRC_FILES
#file(GLOB SRC_FILES
#"${PROJECT_SOURCE_DIR}/src/*.h"
#"${PROJECT_SOURCE_DIR}/src/*.cpp"
#"${PROJECT_SOURCE_DIR}/src/*.ui"
#)

#查找当前文件夹中的所有源代码文件，也可以通过Set命令将所有文件设置为一个变量
FILE(GLOB_RECURSE  SRC_FILES "src/*.cpp") 
#同理，查找当前文件夹中所有的头文件
FILE(GLOB_RECURSE  HEAD_FILES "src/*.h") 
#同理，查找当前文件夹中所有的ui文件
FILE(GLOB_RECURSE  UI_FILES "src/*.ui") 

#通过Ui文件生成对应的头文件，一定要添加
qt5_wrap_ui(WRAP_FILES ${UI_FILES})

#将ui文件和生成文件整理在一个文件夹中，非必须
#source_group("Ui" FILES ${UI_FILES} ${WRAP_FILES})

#生成可执行文件
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES} ${HEAD_FILES} ${RCC_FILES} ${WRAP_FILES})

#添加资源文件 #根据需要添加
#set(RCC_FILES rcc.qrc)

# 设置 target 需要链接的库
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)   #C++标准库
target_link_libraries(${CMAKE_PROJECT_NAME}  ${OpenCV_LIBS})   #Opencv的动态链接库
target_link_libraries(${CMAKE_PROJECT_NAME}  Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network)   #QT的动态链接库
target_link_libraries(${CMAKE_PROJECT_NAME} tensorflow.dll)   #C版Tensorflow的动态链接库 
target_link_libraries(${CMAKE_PROJECT_NAME} SapClassBasic.dll)   #DALSA的动态链接库 
# TODO: 如有需要，请添加测试并安装目标。









#壬戌之秋，七月既望，苏子与客泛舟游于赤壁之下。清风徐来，水波不兴。
#举酒属客，诵明月之诗，歌窈窕之章。少焉，月出于东山之上，徘徊于斗牛之间。
#白露横江，水光接天。纵一苇之所如，凌万顷之茫然。
#浩浩乎如冯虚御风，而不知其所止；飘飘乎如遗世独立，羽化而登仙。
#
#于是饮酒乐甚，扣舷而歌之。歌曰：“桂棹兮兰桨，击空明兮溯流光。渺渺兮予怀，望美人兮天一方。”
#客有吹洞箫者，倚歌而和之。其声呜呜然，如怨如慕，如泣如诉，余音袅袅，不绝如缕。舞幽壑之潜蛟，泣孤舟之嫠妇。
#
#苏子愀然，正襟危坐而问客曰：“何为其然也？”客曰：“月明星稀，乌鹊南飞，此非曹孟德之诗乎？
#西望夏口，东望武昌，山川相缪，郁乎苍苍，此非孟德之困于周郎者乎？方其破荆州，下江陵，顺流而东也，
#舳舻千里，旌旗蔽空，酾酒临江，横槊赋诗，固一世之雄也，而今安在哉？
#况吾与子渔樵于江渚之上，侣鱼虾而友麋鹿，驾一叶之扁舟，举匏樽以相属。
#寄蜉蝣于天地，渺沧海之一粟。哀吾生之须臾，羡长江之无穷。挟飞仙以遨游，抱明月而长终。
#知不可乎骤得，托遗响于悲风。”
#
#苏子曰：“客亦知夫水与月乎？逝者如斯，而未尝往也；盈虚者如彼，而卒莫消长也。
#盖将自其变者而观之，则天地曾不能以一瞬；自其不变者而观之，则物与我皆无尽也，而又何羡乎!
#且夫天地之间，物各有主,苟非吾之所有，虽一毫而莫取。
#惟江上之清风，与山间之明月，耳得之而为声，目遇之而成色，取之无禁，用之不竭，
#是造物者之无尽藏也，而吾与子之所共适。”
#
#客喜而笑，洗盏更酌。肴核既尽，杯盘狼籍。相与枕藉乎舟中，不知东方之既白。