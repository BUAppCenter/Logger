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

void Object_detection::send_request(result_data& result_data) {

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
        CURLFORM_BUFFER, (result_data.cid + ".jpg").c_str(),
        CURLFORM_BUFFERPTR, result_data.encoded_buffer.data(),
        CURLFORM_BUFFERLENGTH, result_data.encoded_buffer.size(),
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
        result_data.object_Detection_result = response;
    }

    curl_formfree(formpost);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headerlist);
    curl_global_cleanup();
}
