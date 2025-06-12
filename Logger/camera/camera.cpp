#include "camera.h"

Camera_cls::Camera_cls(const Config_cls& config) {
    width = config.getWidth();
    height = config.getHeight();
    fps = config.getFps();
    orifile_path = config.getOrifilePath();
    open_camera();
    lamping_time();
    set_frame();
}

Camera_cls::~Camera_cls() {
    if (cap.isOpened()) {
        cap.release();
        cout << "Camera released in destructor" << endl;
    }
}

void Camera_cls::open_camera() {
    int deviceID = 0;
    int apiID = CAP_V4L2;
    cap.open(deviceID, apiID);

    if (!cap.isOpened()) {
        cerr << "camera doesn't open" << endl;
        exit(0);
    }
}

void Camera_cls::lamping_time() {        //초기 프레임 버리기
    Mat temp;
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera -- lamping time\n";
    }
    for (int i = 0; i < 20; i++) {
        cap >> temp;
    }
    temp.release();
}

int Camera_cls::set_frame() {
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));  //코덱 설정 : Motion JPEG
    cap.set(CAP_PROP_FRAME_WIDTH, width);
    cap.set(CAP_PROP_FRAME_HEIGHT, height);
    cap.set(CAP_PROP_FPS, fps);

    cout << "Frame Width: " << cvRound(cap.get(CAP_PROP_FRAME_WIDTH)) << endl;
    cout << "Frame Height: " << cvRound(cap.get(CAP_PROP_FRAME_HEIGHT)) << endl;
    cout << "FPS : " << cvRound(cap.get(CAP_PROP_FPS)) << endl;

    Mat img(Size(width, height), CV_8UC3, Scalar(0));       //(크기, BGR, 전체 검정 배경경)
    this->currentFrame = img.clone();                       //깊은 복사사
    img.release();

    return 0;
}

void Camera_cls::capture_frame(result_data& result_data) {

    cap.read(currentFrame);

    std::cout << "Capture success " << std::endl;

    // 이미지 인코딩
    vector<uchar> buf;
    vector<int> params = {IMWRITE_JPEG_QUALITY, 90}; // JPEG 품질 설정
    imencode(".jpg", currentFrame, buf, params);
    
    result_data.cid = getCID();
    result_data.BGR_frame = currentFrame;
    result_data.encoded_buffer = move(buf);
}

void Camera_cls::image_save(result_data& result_data) {
    Mat original;
    string m_cid = result_data.cid;
    result_data.BGR_frame.copyTo(original);
    string img_name = orifile_path + m_cid + ".png";

    imwrite(img_name, original);
    std::cout << "Save success " << std::endl;

    original.release();
}
