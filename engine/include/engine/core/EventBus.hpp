#pragma once

#include <unordered_map>
#include <typeindex>
#include <vector>
#include <memory>
#include <functional>

namespace NexusEngine {
    class IEvent {
    public:
        virtual ~IEvent() = default;
    };

    class IEventHandler {
    public:
        virtual ~IEventHandler() = default;
        virtual void Execute(IEvent&) = 0;
    };

    template<typename TEvent>
    class EventHandler : public IEventHandler {
    public:
        EventHandler(std::function<void(TEvent&)> callback)
            : m_Callback(std::move(callback)) {}

        void Execute(IEvent& event) override {
            m_Callback(static_cast<TEvent&>(event));
        }

    private:
        std::function<void(TEvent&)> m_Callback;
    };

    class EventBus {
    public:
        template<typename TEvent, typename Callback>
        void Subscribe(Callback&& cb){
            auto [it, _] = m_Subscribers.try_emplace(std::type_index(typeid(TEvent)));
           it->second.push_back(std::make_unique<EventHandler<TEvent>>(std::forward<Callback>(cb)));
        }

        template<typename TEvent>
        void Publish(TEvent&& event){

        }

        void Process();
    private:
        std::unordered_map<std::type_index, std::vector<std::unique_ptr<IEventHandler>>> m_Subscribers;
        std::vector<std::unique_ptr<IEvent>> m_Queue;
    };
}