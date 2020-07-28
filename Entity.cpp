#include "Entity.h"

namespace Engine
{
    Entity* Entity::addChild()
    {
        Entity* t = new Entity(this);
        m_childs.push_back(t);
        return t;
    }

    void    Entity::setParent(Entity* parent) {
        //separate from actual parent
        if (m_parent) {
            auto iter = std::find(m_parent->m_childs.begin(), m_parent->m_childs.end(), this);//search this entity
            m_parent->m_childs.erase(iter);//delete from childs
        }

        m_parent = parent;
    }
}