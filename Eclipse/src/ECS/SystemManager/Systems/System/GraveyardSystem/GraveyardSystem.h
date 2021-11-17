#pragma once

#include "../System.h"

namespace Eclipse
{
    class GraveyardSystem : public System
    {
    public:
        void Update() override;
        bool CommenceBurial(DestroyEvent de);

        template <typename T>
        void RemoveDuplicates(std::vector<Entity>& list);
    private:
        std::vector<Entity> m_Graveyard;
        bool HasFuneralStarted{ false };
    };

    template<typename T>
    inline void GraveyardSystem::RemoveDuplicates(std::vector<Entity>& list)
    {
        // O(n) time complexity
        std::vector<MultiEntity>::iterator itr = list.begin();
        std::unordered_set<int> s;

        for (auto curr = list.begin(); curr != list.end(); ++curr) {
            if (s.insert(curr->ID).second)
                *itr++ = *curr;
        }

        list.erase(itr, list.end());
    }
}
