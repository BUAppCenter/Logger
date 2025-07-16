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

void Camera_cls::capture_frame(queue<result_data>& result_data_queue) {

unsigned int captured_threshold = 100;

    result_data data;
    while (true) {
        if (result_data_queue.size() > captured_threshold) {
            cout << "Queue exceeds its threshold" << endl;
            // spdlog::info("Queue exceeds its threshold");
            this_thread::sleep_for(chrono::milliseconds(800));
            lamping_time();
            continue;
        }
        auto frame_start_time = std::chrono::steady_clock::now();

        cap.read(currentFrame);
        auto frame_end_time = std::chrono::steady_clock::now();
        int duration = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end_time - frame_start_time).count();

        vector<uchar> buf;
        vector<int> params = {IMWRITE_JPEG_QUALITY, 90}; // JPEG 품질 설정
        imencode(".jpg", currentFrame, buf, params);

        data.cid = getCID();
        data.BGR_frame = currentFrame;
        data.encoded_buffer = move(buf);
        result_data_queue.push(data);

        std::cout << "capture_thread: " << duration << " ms" << std::endl;
        // spdlog::info("capture_thread: {} ms", duration);
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
    data.BGR_frame.release();
}

void Camera_cls::capture_frame_task(queue<result_data>& result_data_queue) {
    // 캡처 작업을 수행하는 스레드
    pthread_setname_np(pthread_self(), "capture_thread");
    while (true) {
        if (set_frame() == -1) {
            cerr << "Error: init_frame failed." << endl;
        }
        capture_frame(result_data_queue);
        cout << "    result_data_queue size : " << result_data_queue.size() << endl;
        this_thread::sleep_for(chrono::milliseconds(800));
    }
}

void Camera_cls::start_capture_thread(queue<result_data>& result_data_queue) {
    // 캡처 스레드를 생성하고 시작  
    capture_thread = thread(&Camera_cls::capture_frame_task, this, ref(result_data_queue));
}

void Camera_cls::image_save(queue<result_data>& result_data_queue) {
    Mat original;
    string m_cid = result_data_queue.front().cid;
    result_data_queue.front().BGR_frame.copyTo(original);
    string img_name = orifile_path + m_cid + ".png";

    imwrite(img_name, original);
    result_data_queue.front().image_save_state = true;
    std::cout << "Save success " << std::endl;

    original.release();
}

void Camera_cls::image_save_task(queue<result_data>& result_data_queue) {
    // 이미지 저장 작업을 수행하는 스레드
    pthread_setname_np(pthread_self(), "image_save_thread");
    while (true) {
        if (!result_data_queue.empty()) {
            if (!result_data_queue.front().cid.empty() && result_data_queue.front().image_save_state == false) {
                auto total_start_time = std::chrono::steady_clock::now();
                image_save(result_data_queue);
                auto total_end_time = std::chrono::steady_clock::now();

                int duration = std::chrono::duration_cast<std::chrono::milliseconds>(total_end_time - total_start_time).count();
                std::cout << "image_save_thread: " << duration << " ms" << std::endl;
                // spdlog::info("T1: {} ms", duration);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Camera_cls::start_save_thread(queue<result_data>& result_data_queue) {
    // 이미지 저장 스레드를 생성하고 시작
    image_save_thread = thread(&Camera_cls::image_save_task, this, ref(result_data_queue));
}