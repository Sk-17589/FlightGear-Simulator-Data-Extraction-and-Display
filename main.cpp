#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set the URL of the FlightGear HTTP server (confirm the correct URL)
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5500/json/");

        // Set the write callback function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else {
            // Process the JSON data
            try {
                auto json = nlohmann::json::parse(readBuffer);
                std::cout << "Flight Data: " << json.dump(4) << std::endl;
            }
            catch (nlohmann::json::exception& e) {
                std::cerr << "JSON parse error: " << e.what() << std::endl;
            }
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}
