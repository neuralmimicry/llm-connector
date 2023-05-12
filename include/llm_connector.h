#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>

/**
 * @file llm_connector.h
 * @brief A header file that declares the functions to connect to multiple large language models (LLMs) and send text requests to them.
 *
 * @author Bard
 * @date 2023-05-11
 */

#ifndef LLM_CONNECTOR_H
#define LLM_CONNECTOR_H

/**
 * @brief Initializes the CURL library.
 *
 * @return A CURL handle, or NULL on error.
 */
CURL *init_curl();

/**
 * @brief Sets the URL to connect to.
 *
 * @param curl The CURL handle.
 * @param url The URL to connect to.
 */
void set_url(CURL *curl, const std::string &url);

/**
 * @brief Sets the request method to POST.
 *
 * @param curl The CURL handle.
 */
void set_post(CURL *curl);

/**
 * @brief Sets the request body.
 *
 * @param curl The CURL handle.
 * @param body The request body.
 */
void set_body(CURL *curl, const std::string &body);

/**
 * @brief Sets the response type to JSON.
 *
 * @param curl The CURL handle.
 * @param contentType The content type of the response.
 */
void set_json(CURL *curl, const std::string &contentType = "application/json");

/**
 * @brief Performs the request.
 *
 * @param curl The CURL handle.
 * @return The CURLcode, or CURLE_OK on success.
 */
CURLcode perform_request(CURL *curl);

/**
 * @brief Gets the response body.
 *
 * @param curl The CURL handle.
 * @return The response body.
 */
std::string get_response(CURL *curl);

/**
 * @brief Cleans up the CURL library.
 *
 * @param curl The CURL handle.
 */
void cleanup_curl(CURL *curl);

/**
 * @brief Sends a text request to an LLM.
 *
 * @param llm The LLM to send the request to.
 * @param text The text to send to the LLM.
 * @return The response from the LLM.
 */
std::string send_request(const std::string &llm, const std::string &text);

/**
 * @brief Sends output from one LLM to another LLM.
 *
 * @param fromLLM The LLM to send the output from.
 * @param toLLM The LLM to send the output to.
 * @param text The text to send to the toLLM.
 */
void send_output(const std::string &fromLLM, const std::string &toLLM, const std::string &text);

/**
 * @brief Logs all text exchanges.
 *
 * @param fromLLM The LLM that sent the text.
 * @param toLLM The LLM that received the text.
 * @param text The text that was sent.
 */
void log_exchange(const std::string &fromLLM, const std::string &toLLM, const std::string &text);

/**
 * @brief Reads the configuration file.
 *
 * @param llms_url The URL of the configuration file.
 */
void read_config(std::string &llms_url);

/**
 * @brief Updates the configuration file.
 *
 * @param llm The new LLM.
 * @param url The URL of the new LLM.
 */
void update_config(const std::string &llm, const std::string &url);

#endif // LLM_CONNECTOR_H
