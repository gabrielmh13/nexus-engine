#include "engine/core/EventBus.hpp"

#include <typeindex>

namespace NexusEngine {
    void EventBus::Process(){
        for(auto& event : m_Queue){
            auto it = m_Subscribers.find(std::type_index(typeid(*event)));
            if(it == m_Subscribers.end()) continue;
            for(auto& handler : it->second){
                handler->Execute(*event);
            }
        }

        m_Queue.clear();
    }
}