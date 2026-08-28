
extern "C" {
    void skInit();

    void skEventCallback(void (*eventCallback)(int,int));
    void skLog(const char* message);
}