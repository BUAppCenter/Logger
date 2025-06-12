
# Logger 프로젝트

## 개요

`Logger`는 C++로 작성된 모듈형 시스템으로, 다음과 같은 기능을 제공합니다:

- **카메라 캡처**: 실시간 카메라 데이터를 캡처하여 처리합니다.
- **이미지 저장**: 캡처된 이미지를 저장하여 이후 분석 또는 기록에 활용합니다.
- **객체 탐지**: 이미지나 비디오 데이터에서 객체를 탐지하고 결과를 반환합니다.
- **MQTT Publish**: 탐지된 객체 정보나 카메라 상태 등의 데이터를 MQTT 프로토콜을 통해 다른 시스템에 발행(publish)합니다.

## 프로젝트 구조

```
Logger/
├── camera/
│   ├── camera.cpp        # 카메라 관련 코드
│   ├── camera.h          # 카메라 헤더 파일
│   ├── create_cid.cpp    # CID 생성 코드
│   └── create_cid.h      # CID 헤더 파일
├── config/
│   ├── Config.cpp        # 설정 관련 코드
│   └── Config.h          # 설정 헤더 파일
├── Mqtt/
│   ├── Mqtt.cpp          # MQTT 관련 코드
│   └── Mqtt.h            # MQTT 헤더 파일
├── object_detection/
│   ├── object_detection.cpp  # 객체 탐지 코드
│   └── object_detection.h    # 객체 탐지 헤더 파일
├── result_data/
│   └── result_data.h     # 결과 데이터 처리
├── Logger.cpp            # 메인 진입점 코드
└── Makefile              # 빌드 스크립트
```

## 기능 설명

### 카메라 캡처

- 실시간 카메라에서 영상을 캡처하여 처리합니다. 
- 카메라는 다양한 해상도와 형식을 지원하며, `camera.cpp`에서 설정 및 제어합니다.

### 이미지 저장

- 캡처된 이미지는 로컬 디스크에 저장됩니다.
- 이미지는 날짜/시간, 카메라 정보 등으로 네이밍하여 파일로 저장되며, 저장 경로는 `config.json`에서 설정할 수 있습니다.

### 객체 탐지

- 카메라로부터 캡처된 이미지에서 객체를 탐지합니다.
- 객체 탐지는 `object_detection.cpp`에서 처리되며, 탐지된 객체는 MQTT 메시지로 전송됩니다.

### MQTT Publish

- 탐지된 객체 정보와 카메라 상태를 MQTT 프로토콜을 사용하여 다른 시스템에 발행합니다.
- `Mqtt.cpp`는 MQTT 서버와의 연결을 설정하고, 데이터를 전송하는 역할을 합니다.

## 빌드 방법

1. `Logger` 디렉토리로 이동합니다.
   ```bash
   cd Logger
   ```
2. `make` 명령어로 빌드를 실행합니다.
   ```bash
   make
   ```

   빌드가 완료되면 실행 파일이 생성됩니다.

## 설정

### config.json

- **카메라 설정**:
  - 해상도: width x height
  - FPS: (초당 프레임 수)
  - 저장 경로: (사용자 지정 경로)

- **객체 탐지 서버 설정**:
  - 서버 IP 주소: (사용자 지정 IP)
  - 서버 포트: (사용자 지정 포트)

- **MQTT 설정**:
  - MQTT 토픽: (사용자 지정 토픽픽)
  - MQTT 클라이언트 ID: (사용자 지정 ID)
  - MQTT 브로커 IP 주소: (사용자 지정 IP)
  - MQTT 브로커 포트: (사용자 지정 포트)

```json
{
  "camera": {
    "resolution": "1920x1080",
    "capture_interval": 1000
  },
  "storage": {
    "image_path": "./images"
  },
  "mqtt": {
    "broker": "mqtt://localhost",
    "port": 1883,
    "topic": "camera/object_detection"
  },
  "object_detection": {
    "detection_threshold": 0.5
  }
}
```

### 설정 파일의 주요 항목:

- **카메라 설정**:
  - `resolution`: 카메라 해상도 설정 (예: `1920x1080`).
  - `capture_interval`: 이미지 캡처 간격을 밀리초 단위로 설정 (예: `1000`ms).

- **이미지 저장**:
  - `image_path`: 캡처된 이미지가 저장될 경로 설정.

- **MQTT 설정**:
  - `broker`: MQTT 브로커 주소 설정 (예: `mqtt://localhost`).
  - `port`: MQTT 서버 포트 설정 (기본값: `1883`).
  - `topic`: 발행할 MQTT 토픽 설정 (예: `camera/object_detection`).

- **객체 탐지 설정**:
  - `detection_threshold`: 객체 탐지의 민감도를 설정하는 임계값 (0.0 ~ 1.0 범위).

## 사용 방법

1. `config.json` 파일을 적절하게 수정합니다.
2. `make` 명령어로 빌드를 수행합니다.
3. 빌드된 실행 파일을 실행하여 로그 수집 및 처리를 시작합니다.

## 라이선스

(라이선스 정보를 추가하세요. 예: MIT, GPL 등)

## 기여

기여는 언제든지 환영합니다. 기능 개선이나 버그 리포트를 PR 또는 이슈로 제출해 주세요.
