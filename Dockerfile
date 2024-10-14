# @see https://hub.docker.com/_/ubuntu
FROM ubuntu:latest AS runner

# Define the default shell
SHELL [ "/bin/bash", "-c" ]

# Update platform and install sudo
RUN apt-get update --yes && apt-get install sudo --yes

# Create the 'ubuntu' user and set up the home directory
RUN useradd --create-home --shell /bin/bash ubuntu

WORKDIR /home/ubuntu

# Copy only the Ubuntu scripts inside the image
COPY script/ubuntu/ /home/ubuntu/script/ubuntu/
RUN chown ubuntu:ubuntu --recursive /home/ubuntu

# Install dependencies
RUN chmod u+x --recursive /home/ubuntu/script/ubuntu
RUN /home/ubuntu/script/ubuntu/install.sh

# Clean up image
RUN rm --force --recursive /home/ubuntu/script

ENV LOGGING_LEVEL=DEBUGGING