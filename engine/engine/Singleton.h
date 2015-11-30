#pragma once

/** Set of macros to implement singletons.
*/

#define SINGLETON_HEADER(CLASSNAME)\
	private: static CLASSNAME* _instance; CLASSNAME();\
	public: static CLASSNAME* instance(); virtual ~CLASSNAME(); void cleanup();

#define SINGLETON_IMPLEMENTATION(CLASSNAME)\
    CLASSNAME* CLASSNAME::_instance = 0;\
    CLASSNAME* CLASSNAME::instance() {\
        if (_instance == 0) { _instance = new CLASSNAME(); }\
        return _instance; }\
    CLASSNAME::CLASSNAME() {}\
    CLASSNAME::~CLASSNAME() {}\
	void CLASSNAME::cleanup() { if (_instance != 0) { delete _instance; _instance = 0; } }

#define SINGLETON_IMPLEMENTATION_NO_CONSTRUCTOR(CLASSNAME)\
    CLASSNAME* CLASSNAME::_instance = 0;\
    CLASSNAME* CLASSNAME::instance() {\
        if (_instance == 0) { _instance = new CLASSNAME(); }\
        return _instance; }\
    CLASSNAME::~CLASSNAME() {}\
	void CLASSNAME::cleanup() { if (_instance != 0) { delete _instance; _instance = 0; } }

