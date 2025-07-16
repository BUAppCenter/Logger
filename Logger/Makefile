# === 컴파일러 및 옵션 ===
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 $(shell pkg-config --cflags opencv4)

# === include 경로 ===
INCLUDES = -I. -Iconfig -IMqtt -Iobject_detection -Icamera -Iresult_data

# === 외부 라이브러리 (기본 가정: 시스템에 설치됨) ===
OPENCV = `pkg-config --cflags --libs opencv4`
LIBS = $(OPENCV) -lpaho-mqttpp3 -lpaho-mqtt3as -lcurl -lspdlog -ljsoncpp -ldl -lpthread

# === 소스 파일 ===
SRCS = Logger.cpp \
       config/Config.cpp \
       Mqtt/Mqtt.cpp \
       object_detection/object_detection.cpp \
       camera/create_cid.cpp \
       camera/camera.cpp \

# === 오브젝트 파일 (자동 생성) ===
OBJS = Logger.o \
       config/Config.o \
       Mqtt/Mqtt.o \
       object_detection/object_detection.o \
       camera/create_cid.o \
       camera/camera.o \

# === 최종 실행파일 이름 ===
TARGET = Logger

# === 기본 빌드 규칙 ===
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# === clean 규칙 ===
clean:
	rm -f $(OBJS) $(TARGET)
