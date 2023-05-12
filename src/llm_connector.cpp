#include "llm_connector.h"

// Standard library includes
#include <iostream>
#include <fstream>
#include <regex>

// CURL includes
#include <curl/curl.h>

// Initialize CURL
CURL *init_curl() {
    return curl_easy_init();
}

// Sets the URL to connect to
void set_url(CURL *curl, const std::string &url) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
}

// Sets the request method to POST
void set_post(CURL *curl) {
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
}

// Sets the request body
void set_body(CURL *curl, const std::string &body) {
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
}

// Sets the response type to JSON
void set_json(CURL *curl, const std::string &contentType) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("Content-Type: " + contentType).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
}

// Performs the request
CURLcode perform_request(CURL *curl) {
    return curl_easy_perform(curl);
}

// Gets the response body
std::string get_response(CURL *curl) {
    std::string response;
    // Fill in with code to get the response from the CURL handle
    return response;
}

// Cleans up the CURL library
void cleanup_curl(CURL *curl) {
    curl_easy_cleanup(curl);
}

// Sends a text request to an LLM
std::string send_request(const std::string &llm, const std::string &text) {
    CURL *curl = init_curl();
    set_url(curl, llm);
    set_post(curl);
    set_body(curl, text);
    set_json(curl);
    perform_request(curl);
    std::string response = get_response(curl);
    cleanup_curl(curl);
    return response;
}

// Sends output from one LLM to another LLM
void send_output(const std::string &fromLLM, const std::string &toLLM, const std::string &text) {
    std::string output = send_request(fromLLM, text);
    send_request(toLLM, output);
}

// Logs all text exchanges
void log_exchange(const std::string &fromLLM, const std::string &toLLM, const std::string &text) {
    std::ofstream log("log.txt", std::ios::app);
    log << "From: " << fromLLM << "\nTo: " << toLLM << "\nText: " << text << "\n\n";
    log.close();
}

// Reads the configuration file
void read_config(std::string &llms_url) {
    std::ifstream config("config.txt");
    if (config) {
        std::getline(config, llms_url);
    }
    config.close();
}

// Updates the configuration file
void update_config(const std::string &llm, const std::string &url) {
    std::ofstream config("config.txt", std::ios::app);
    config << "," << llm << ":" << url;
    config.close();
}
int main() {
    // Initialize CURL
    CURL *curl = init_curl();
    if (curl == NULL) {
        std::cerr << "Error initializing CURL" << std::endl;
        return 1;
    }

    // Read the configuration file
    std::string llms_url;
    read_config(llms_url);

    // Split the LLMs URL into an array
    std::vector<std::string> llms;
    std::stringstream ss(llms_url);
    std::string llm;
    while (std::getline(ss, llm, ',')) {
        llms.push_back(llm);
    }

    // Set the text to send
    std::string text = "Hello, world!";

    // Loop over the LLMs
    for (auto llm : llms) {
        // Send the text request to the LLM
        std::string response = send_request(llm, text);

        // If the LLM responded with an error, stop
        if (response.empty()) {
            std::cerr << "Error: no response from LLM" << std::endl;
            continue;
        }

        // Log the exchange
        log_exchange(llm, "", text);

        // Check if the response contains a new LLM
        std::string new_llm;
        std::regex new_llm_pattern("New LLM: (.+)");
        std::smatch matches;
        if (std::regex_search(response, matches, new_llm_pattern)) {
            new_llm = matches[1].str();
        }

        // If a new LLM was found, update the configuration file
        if (!new_llm.empty()) {
            update_config(new_llm, llm + "/v1/text");
        }
    }

    // Clean up
    cleanup_curl(curl);
    return 0;
}
