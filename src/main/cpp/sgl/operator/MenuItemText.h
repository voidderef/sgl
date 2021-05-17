#pragma once

#include <utility>

#include "MenuItem.h"

#include "Screen.h"

namespace sgl {

class MenuPageText;

class MenuItemText : public MenuItem
{
public:
    friend class MenuPageText;

    MenuItemText(EngineProxy* refEngineProxy, const std::string& fontName, uint16_t size, std::string name,
        const std::function<ssize_t()>& callback);

    virtual ~MenuItemText() override;

    void Activate(bool activate) override;

    virtual void SetHighlighted(bool highlighted) override;

private:
    EngineProxy* m_refEngineProxy;

    uint16_t m_font;
    uint16_t m_texNormal;
    uint16_t m_texHighlighted;
    uint16_t m_sprite;
};

}
