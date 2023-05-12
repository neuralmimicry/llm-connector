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
