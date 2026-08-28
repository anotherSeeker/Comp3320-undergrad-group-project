
extern "C" {
    bool skInit();
    void skRun();

    void skEventCallback(void (*eventCallback)(int,int,void*));
    void skLog(const char* message);
    void skListen(const char* eventName,int callback);
}