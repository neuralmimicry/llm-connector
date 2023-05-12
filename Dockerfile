# Use an official C++ base image
FROM gcc:latest
LABEL authors="pbisaacs"

# Set the working directory in the container to /app
WORKDIR /app

# Copy the current directory contents into the container at /app
COPY . /app/

# Install any needed dependencies...
RUN apt-get update && apt-get install -y \
    libcurl4-openssl-dev cmake

# Compile your C++ code
RUN cmake .

# Expose port 8080 for the app to listen on
EXPOSE 8080

# Define environment variable (optional)
# ENV NAME World

# Run the app when the container launches
CMD ["/app/llm_connector"]

