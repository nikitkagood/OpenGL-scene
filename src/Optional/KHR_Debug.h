#pragma once

//optional header

//part of KHR_debug: https://www.khronos.org/registry/OpenGL/extensions/KHR/KHR_debug.txt
//OpenGL version 4.3 is required
//those constants are not defined in GLEW library so they are here
//used for user Debug Output
//i.e.: glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, DEBUG_TYPE_OTHER, 100, DEBUG_SEVERITY_NOTIFICATION ...)
//also: glDebugMessageCallback; if no callback is registered, then messages are stored in a log 
//Messages from the context's log can be fetched with this function: glGetDebugMessageLog

//to turn debugging on: glEnable(GL_DEBUG_OUTPUT); must be called before glewInit

#define DEBUG_TYPE_OTHER 0x8251
#define DEBUG_SEVERITY_NOTIFICATION 0x826B

//add it: glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, DEBUG_TYPE_OTHER, 100, DEBUG_SEVERITY_NOTIFICATION, 128, debug_msg_buffer);