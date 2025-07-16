#include "object_detection.h"

using namespace std;
using namespace cv;

Object_detection::Object_detection(const Config_cls& config) {
    m_server_ip_ = config.getObjectDetectorIp();
    m_server_port_ = config.getObjectDetectorPort();
}

size_t WriteCallback_OD(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t numBytes = size * nmemb;
    static_cast<std::string*>(userp)->append(static_cast<char*>(contents), numBytes);
    return numBytes;
}

void Object_detection::send_request(queue<result_data>& result_data_queue) {

    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Error initializing libcurl." << std::endl;
        return;
    }

    char buf_port[10];
    snprintf(buf_port, sizeof(buf_port), "%d", m_server_port_);
    string server_url = "http://" + m_server_ip_ + ":" + buf_port + "/detect_obj";

    curl_httppost* formpost = nullptr;
    curl_httppost* lastptr = nullptr;

    // 메모리 버퍼로 파일 전송
    curl_formadd(&formpost, &lastptr,
        CURLFORM_COPYNAME, "file",
        CURLFORM_BUFFER, (result_data_queue.front().cid + ".jpg").c_str(),
        CURLFORM_BUFFERPTR, result_data_queue.front().encoded_buffer.data(),
        CURLFORM_BUFFERLENGTH, result_data_queue.front().encoded_buffer.size(),
        CURLFORM_CONTENTTYPE, "image/jpeg",
        CURLFORM_END);

    struct curl_slist* headerlist = curl_slist_append(nullptr, "Expect:");

    curl_easy_setopt(curl, CURLOPT_URL, server_url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback_OD);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "(Object Detect)HTTP request failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "HTTP response received " << std::endl;

        result_data_queue.front().object_Detection_result = response;
        result_data_queue.front().object_Detection_result_state = true;
    }

    curl_formfree(formpost);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headerlist);
    curl_global_cleanup();
}

void Object_detection::send_request_task(queue<result_data>& result_data_queue) {
    // 객체 감지 요청을 수행하는 스레드
    pthread_setname_np(pthread_self(), "object_detection_thread");
    while (true) {
        if (!result_data_queue.empty()) {
            if(result_data_queue.front().image_save_state == true && 
                result_data_queue.front().object_Detection_result_state == false) {
                auto total_start_time = std::chrono::steady_clock::now();
                send_request(result_data_queue);
                auto total_end_time = std::chrono::steady_clock::now();

                int duration = std::chrono::duration_cast<std::chrono::milliseconds>(total_end_time - total_start_time).count();
                // spdlog::info("object_detection_thread: {} ms", duration);

                std::cout << "object_detection_thread: " << duration << " ms" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Object_detection::start_send_request_thread(queue<result_data>& result_data_queue) {
    // 객체 감지 요청 스레드를 생성하고 시작
    send_request_thread = thread(&Object_detection::send_request_task, this, ref(result_data_queue));
}