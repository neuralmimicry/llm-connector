/**
 * @file llm_connector.cpp
 * @brief Functions for interacting with LLMs via CURL.
 */

#include "llm_connector.h"

// Standard library includes
#include <iostream>
#include <fstream>
#include <regex>

// CURL includes
#include <curl/curl.h>

// Private function to log errors
void log_error(const std::string &error_message) {
    std::ofstream error_log("error_log.txt", std::ios::app);
    error_log << error_message << "\n";
    error_log.close();
}

/**
 * @brief Initialize CURL
 * @return CURL pointer
 */
CURL *init_curl() {
    CURL *curl = curl_easy_init();
    if (curl == nullptr) {
        log_error("Error initializing CURL");
    }
    return curl;
}

/**
 * @brief Sets the URL to connect to
 * @param curl CURL pointer
 * @param url URL to connect to
 */
void set_url(CURL *curl, const std::string &url) {
    // Validating the URL would ideally involve checking it with a regex or similar
    // This is just a very basic sanity check
    if (url.substr(0, 5) != "https") {
        log_error("Unsafe URL: " + url);
        return;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
}

/**
 * @brief Sets the request body
 * @param curl CURL pointer
 * @param body Request body
 */
void set_body(CURL *curl, const std::string &body) {
    // Perform input validation on 'body' before setting it
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
}

/**
 * @brief Sets the response type to JSON
 * @param curl CURL pointer
 * @param contentType Content type
 */
void set_json(CURL *curl, const std::string &contentType) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("Content-Type: " + contentType).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
}

/**
 * @brief Sets the request method to POST
 * @param curl CURL pointer
 */
void set_post(CURL *curl) {
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
}

/**
 * @brief Performs the request
 * @param curl CURL pointer
 * @return CURLcode The result of the operation
 */
CURLcode perform_request(CURL *curl) {
    return curl_easy_perform(curl);
}

/**
 * @brief Gets the response body
 * @param curl CURL pointer
 * @return std::string Response body
 */
std::string get_response(CURL *curl) {
    std::string response;
    // Fill in with code to get the response from the CURL handle
    return response;
}

/**
 * @brief Cleans up the CURL library
 * @param curl CURL pointer
 */
void cleanup_curl(CURL *curl) {
    curl_easy_cleanup(curl);
}

/**
 * @brief Sends a text request to an LLM
 * @param llm URL of the LLM
 * @param text Text request to be sent
 * @return std::string Response from the LLM
 */
std::string send_request(const std::string &llm, const std::string &text) {
    CURL *curl = init_curl();
    set_url(curl, llm);
    set_post(curl);
    set_body(curl, text);
    set_json(curl, "application/json");
    perform_request(curl);
    std::string response = get_response(curl);
    cleanup_curl(curl);
    return response;
}

/**
 * @brief Sends output from one LLM to another LLM
 * @param fromLLM The LLM sending the output
 * @param toLLM The LLM receiving the output
 * @param text Text to be sent
 */
void send_output(const std::string &fromLLM, const std::string &toLLM, const std::string &text) {
    std::string output = send_request(fromLLM, text);
    send_request(toLLM, output);
}

/**
 * @brief Logs all text exchanges
 * @param fromLLM The LLM that sent the text
 * @param toLLM The LLM that received the text
 * @param text The text that was exchanged
 */
void log_exchange(const std::string &fromLLM, const std::string &toLLM, const std::string &text) {
    std::ofstream log("log.txt", std::ios::app);
    log << "From: " << fromLLM << "\nTo: " << toLLM << "\nText: " << text << "\n\n";
    log.close();
}

/**
 * @brief Reads the configuration file
 * @param llms_url Reference to a string where the LLM URL will be stored
 */
void read_config(std::string &llms_url) {
    std::ifstream config("config.txt");
    if (config) {
        std::getline(config, llms_url);
    }
    config.close();
}

/**
 * @brief Updates the configuration file
 * @param llm The LLM to be added
 * @param url The URL of the new LLM
 */
void update_config(const std::string &llm, const std::string &url) {
    std::ofstream config("config.txt", std::ios::app);
    config << "," << llm << ":" << url;
    config.close();
}

/**
 * @brief Main function
 * @return int Exit code
 */
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