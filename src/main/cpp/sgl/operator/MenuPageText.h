#pragma once

#include "MenuPage.h"

#include "Screen.h"

namespace sgl {

class MenuPageText : public MenuPage
{
public:
    MenuPageText(EngineProxy* refEngineProxy, const std::string& fontName, uint16_t size, std::string name,
        std::vector<MenuPage*> children, std::vector<MenuItem*> items);

    ~MenuPageText() override;

    void Activate(bool activate) override;

private:
    EngineProxy* m_refEngineProxy;
    const uint16_t m_fontSize;

    uint16_t m_font;
    uint16_t m_texTitle;
    uint16_t m_spriteTitle;

    void __Layout();
};

}
