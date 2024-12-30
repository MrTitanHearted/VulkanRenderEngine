#include <VREngine/Core/EventObserver.hpp>

namespace vre {
    std::map<EventObserver::EventType, EventObserver::IEventCallbackMap>
                  EventObserver::g_CallbacksMap{};
    bool          EventObserver::g_IsInitialized{false};
    EventObserver EventObserver::g_State{};

    void EventObserver::Initialize() {
        DVRE_ASSERT(!g_IsInitialized, "vre::EventObserver must be shut down before initializing");
        DLOG_INFO("Initializing vre::EventObserver");

        g_IsInitialized = true;
    }

    void EventObserver::Shutdown() {
        DVRE_ASSERT(g_IsInitialized, "vre::EventObserver must be initialized before shutting down");
        DLOG_INFO("Shutting vre::EventObserver down");

        g_CallbacksMap.clear();

        g_IsInitialized = false;
    }

    bool EventObserver::IsInitialized() {
        return g_IsInitialized;
    }

    EventObserver::~EventObserver() {
        VRE_ASSERT(!g_IsInitialized, "vre::EventObserver must be shut down before closing");
    }
};  // namespace vre