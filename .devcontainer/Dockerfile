FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install apt deps in one layer; --no-install-recommends keeps the image lean.
# libboost-graph-dev already pulls in libboost-dev, so no need to list both.
RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    make \
    curl \
    ca-certificates \
    libboost-graph-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

# The Makefile calls 'g++34' (an old Debian package alias for GCC 3.4).
# Create a symlink so the Makefile works without modification.
RUN ln -s /usr/bin/g++ /usr/local/bin/g++34

# Install Node.js and Claude Code in one layer to minimise layer count.
RUN curl -fsSL https://deb.nodesource.com/setup_22.x | bash - \
    && apt-get install -y --no-install-recommends nodejs \
    && rm -rf /var/lib/apt/lists/* \
    && npm install -g @anthropic-ai/claude-code