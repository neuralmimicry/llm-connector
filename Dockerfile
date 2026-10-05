# 1st stage, build the application
FROM gcc:latest AS builder
LABEL authors="pbisaacs"

WORKDIR /app
COPY . /app/

RUN apt-get update && apt-get install -y \
    libcurl4-openssl-dev cmake

RUN cmake . && make

# 2nd stage, build the final Docker image
FROM debian:bullseye-slim

WORKDIR /app

# Copy the built binary from the 1st stage
COPY --from=builder /app/llm_connector .

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libcurl4-openssl-dev && \
    rm -rf /var/lib/apt/lists/*

EXPOSE 8080

CMD ["/app/llm_connector"]

# OCI metadata (final stage) so GHCR links the package to its source repository.
LABEL org.opencontainers.image.source="https://github.com/neuralmimicry/llm-connector" \
      org.opencontainers.image.url="https://github.com/neuralmimicry/llm-connector" \
      org.opencontainers.image.description="LLM connector: conversations between multiple LLMs" \
      org.opencontainers.image.vendor="NeuralMimicry" \
      org.opencontainers.image.licenses="GPL-3.0-only"
