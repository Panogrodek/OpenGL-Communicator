workspace "Communicator"
	startproject "Client"
	architecture "x64"
	
	configurations {
		"debug",
		"release"
	}
	
	filter {
		"platforms:Win64"
	}
	
	system "Windows"
	
include "Client"
include "Polonium"
include "Server"