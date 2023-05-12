#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>

/**
 * @file llm_connector.cpp
 * @brief A C++ program to connect to multiple large language models (LLMs) and send text requests to them.
 *
 * @author Paul B. Isaac's
 * @date 2023-05-11
 */

#include "llm_connector.h"

int main() {
    // Initialize CURL
    CURL *curl = init_curl();
    if (curl == NULL) {
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
            continue;
        }

        // Log the exchange
        log_exchange(llm, "", text);

        // Check if the response contains a new LLM
        std::string new_llm;
        if (std::regex_match(response, std::regex("New LLM: (.+)"))) {
            new_llm = std::regex_replace(response, std::regex("New LLM: (.+)"), "$1");
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
