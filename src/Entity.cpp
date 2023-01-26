#include "Entity.h"
#include "Editor.h"

namespace Engine
{
    Entity* Entity::addChild()
    {
        Entity* t = new Entity(this);
        m_childs.push_back(t);
        return t;
    }
    void Entity::addChild(Entity* entity)
    {
        m_childs.push_back(entity);
    }

    //Returns true if there is recursion
    bool Entity::check_recursion(Entity* entity)
    {
        if (!m_parent)//no parent
            return false;

        if (entity == m_parent)
            return true;
        
        return m_parent->check_recursion(entity);
    }

    void Entity::drawEditorGUI_Properties()
    {
        Editor::Instance()->renderEntityEditorPropertiesGui(this);
    }

    void Entity::setParent(Entity* parent) {

        if (parent->check_recursion(this))
            return;

        //separate from actual parent
        if (m_parent) {
            auto iter = std::find(m_parent->m_childs.begin(), m_parent->m_childs.end(), this);//search this entity
            m_parent->m_childs.erase(iter);//delete from childs
        }

        m_parent = parent;
        
        transform.parent = &m_parent->transform;
        
        parent->m_childs.push_back(this);
    }
    bool Entity::isChild(Entity* entity)
    {
        auto iter = std::find(m_childs.begin(), m_childs.end(), entity);
        return iter != m_childs.end();
    }
}