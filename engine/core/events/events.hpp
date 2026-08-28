#pragma once

#include <cstdint>

#include <list>
#include <unordered_map>

// callbacks are represented as uint32_t as the runtime will keep a list of lua functions
// the callbacks are just handles to lua functions defined by the runtime

struct Observer{
    std::list<uint32_t> callbacks;
    uint32_t ID;
};

class EventManager{
    private:
        static std::unordered_map<const char*,Observer> observers;

    public:
        static void createObserver(const char* name,uint32_t ID);
        static bool listen(const char* name,uint32_t callback);
        static void dispatch(const char* name);

        static void (*eventCallback)(int,int);
};