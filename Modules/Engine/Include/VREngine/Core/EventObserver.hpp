#pragma once

#include <VREngine/Core/Includes.hpp>
#include <VREngine/Core/Logger.hpp>

namespace vre {
    class IEvent {
       public:
        virtual ~IEvent() = default;
    };

    using EventCallbackHandle = std::size_t;

    template <typename Event, typename = std::enable_if_t<std::is_base_of<IEvent, Event>::value>>
    using EventCallback = void (*)(const Event &event);

    class EventObserver {
       public:
        static void Initialize();
        static void Shutdown();

        static bool IsInitialized();

        template <typename Event, typename Callback, typename = std::enable_if_t<std::is_base_of<IEvent, Event>::value>>
        static EventCallbackHandle AddCallback(Callback &&callback) {
            const EventType           eventType = typeid(Event).hash_code();
            IEventCallbackMap        &callbacks = g_CallbacksMap[eventType];
            const EventCallbackHandle handle    = callbacks.size();
            callbacks[handle]                   = [callback = callback](const IEvent *event) {
                callback(*static_cast<const Event *>(event));
            };
            return handle;
        }
        template <typename Event, typename = std::enable_if_t<std::is_base_of<IEvent, Event>::value>>
        static EventCallbackHandle AddCallback(EventCallback<Event> callback) {
            const EventType           eventType = typeid(Event).hash_code();
            IEventCallbackMap        &callbacks = g_CallbacksMap[eventType];
            const EventCallbackHandle handle    = callbacks.size();
            callbacks[handle]                   = [callback = callback](const IEvent *event) {
                callback(*static_cast<const Event *>(event));
            };
            return handle;
        }

        template <typename Event, typename T, typename = std::enable_if_t<std::is_base_of<IEvent, Event>::value>>
        static EventCallbackHandle AddCallback(T *self, void (T::*callback)(const Event &event)) {
            const EventType           eventType = typeid(Event).hash_code();
            IEventCallbackMap        &callbacks = g_CallbacksMap[eventType];
            const EventCallbackHandle handle    = callbacks.size();
            callbacks[handle]                   = [callback = callback, self = self](const IEvent *event) {
                (self->*callback)(*static_cast<const Event *>(event));
            };
            return handle;
        }

        template <typename Event, typename = std::enable_if_t<std::is_base_of<IEvent, Event>::value>>
        static void RemoveCallback(EventCallbackHandle handle) {
            const EventType    eventType = typeid(Event).hash_code();
            IEventCallbackMap &callbacks = g_CallbacksMap[eventType];
            callbacks.erase(handle);
        }

        template <typename Event, typename = std::enable_if_t<std::is_base_of<IEvent, Event>::value>>
        static void Process(const Event &event) {
            const EventType          eventType = typeid(Event).hash_code();
            const IEventCallbackMap &callbacks = g_CallbacksMap[eventType];

            for (const auto &[index, callback] : callbacks) {
                callback(&event);
            }
        }

       private:
        using EventType         = std::size_t;
        using IEventCallback    = std::function<void(const IEvent *)>;
        using IEventCallbackMap = std::map<EventCallbackHandle, IEventCallback>;

       private:
        static std::map<EventType, IEventCallbackMap> g_CallbacksMap;
        static bool                                   g_IsInitialized;
        static EventObserver                          g_State;

       private:
        EventObserver()  = default;
        ~EventObserver();
    };
}  // namespace vre