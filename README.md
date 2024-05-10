# OpenGL-Communicator
 An OpenGL application for communication using windows sockets
Project made for my collage
<p align="center">
<b>Client app demo</b><br>
    <img width="520" src="https://github.com/Panogrodek/OpenGL-Communicator/blob/main/Repo/1.jpg">
</p>

# How to install
1. Download repository<br>
2. Open build.bat to make visual studio files<br>
3. Compile the project. Binaries should be located in bin folder<br>
<br>
<b>WARNING!</b> You'll need to setup your visual studio to launch multiple projects or run the server app in the background.

# Rendering
The Polonium library is a wrapper around OpenGL library and is responsible for the client app rendering process.
# Vendor
This project uses multiple tools and libraries:<br>
<a href = https://www.glfw.org/>glfw</a><br>
<a href = "https://glew.sourceforge.net/">glew</a><br>
<a href = https://github.com/g-truc/glm>glm</a><br>
<a href = https://github.com/ocornut/imgui>imgui</a><br>
<a href = "https://github.com/gabime/spdlog">spdlog</a><br>
<a href = "https://github.com/nothings/stb/blob/master/stb_image.h">stbimage</a><br>
<a href = "https://premake.github.io/">premake</a><br>
# Networking
This project uses sockets based mainly on <a href = "https://youtube.com/playlist?list=PLcacUGyBsOIA1YUqvKd7p8uvsG_FCt7-U&si=x1EUh9kNSatfyQGw">Jpres tutorial</a>.
# Controls and usage
This project is a simple communicator used for client-server based communication.
It has been tested on LAN as well as using NAT with receivers outside of network.<br>
To type characters into textbox use keys from your keyboard and press <b>enter</b>.<br>
<b>Enter + shift</b> creates a newline.<br>
# Commands
The server accepts few commands:<br>
<b>/n nick</b> -> changes your name that is shown to others<br>
<b>/w nick message</b> -> whispers to another recipient (send a private message)<br>
Client can see the list of all commands using <b>/h</b>
