# Dockerfile for Numerical Labs Environment

# Use Ubuntu 22.04 LTS as the base image
FROM ubuntu:22.04

# Avoid prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install essential build tools, LAPACK, BLAS (dependency for LAPACK), NLopt, and FLTK
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    gfortran \
    liblapack-dev \ 
    libblas-dev \
    libnlopt-dev \
    libfltk1.3-dev \
    # Add any other common utilities you might need, e.g., git, vim, nano
    # git \
    # vim \
    && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Create a working directory inside the container
WORKDIR /app

# Set the default command to open a bash shell when the container starts
CMD ["bash"]
