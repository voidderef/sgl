#pragma once

#include <utility>
#include <vector>

#include "MenuItem.h"

namespace sgl {

class MenuPage
{
public:
    explicit MenuPage(std::string name, std::vector<MenuPage*> children,
            std::vector<MenuItem*> items) :
        m_name(std::move(name)),
        m_refParent(nullptr),
        m_children(std::move(children)),
        m_items(std::move(items)),
        m_itemSelected(0)
    {
        for (auto& it : m_children) {
            it->m_refParent = this;
        }
    }

    virtual ~MenuPage() {
        for (auto& it : m_items) {
            delete it;
        }

        for (auto& it : m_children) {
            delete it;
        }
    }

    const std::string& GetName() const {
        return m_name;
    }

    virtual void Activate(bool activate) {
        m_items[m_itemSelected]->SetHighlighted(activate);
    }

    void NextItem() {
        m_items[m_itemSelected]->SetHighlighted(false);

        if (++m_itemSelected >= m_items.size()) {
            m_itemSelected = 0;
        }

        m_items[m_itemSelected]->SetHighlighted(true);
    }

    void PreviousItem() {
        m_items[m_itemSelected]->SetHighlighted(false);

        if (m_itemSelected == 0) {
            m_itemSelected = m_items.size() - 1;
        } else {
            m_itemSelected--;
        }

        m_items[m_itemSelected]->SetHighlighted(true);
    }

    MenuItem* GetCurrentItem() const {
        return m_items[m_itemSelected];
    }

    MenuPage* GetRefChildPage(size_t idx) const {
        return m_children[idx];
    }

    MenuPage* GetRefParent() const {
        return m_refParent;
    }

protected:
    std::vector<MenuItem*> m_items;

private:
    const std::string m_name;
    MenuPage* m_refParent;
    std::vector<MenuPage*> m_children;

    size_t m_itemSelected;
};

}
