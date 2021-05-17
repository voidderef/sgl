#include "MenuPageText.h"

#include "MenuItemText.h"

namespace sgl {

MenuPageText::MenuPageText(EngineProxy* refEngineProxy, const std::string& fontName, uint16_t size, std::string name,
        std::vector<MenuPage*> children, std::vector<MenuItem*> items) :
    MenuPage(std::move(name), std::move(children), std::move(items)),
    m_fontSize(size),
    m_refEngineProxy(refEngineProxy)
{
    m_font = m_refEngineProxy->GetFontManager().CreateFontFromTTFFile(
        m_refEngineProxy->GetDataManager().GetRefFsFont()->GetAbsolutePath(fontName), size);
    m_texTitle = m_refEngineProxy->GetTextureManager().CreateTextureFromText(
        GetName(), 0xFF, 0xFF, 0xFF, 0xFF, m_font);
    m_spriteTitle = m_refEngineProxy->GetSpriteRenderer().CreateSprite(m_texTitle);

    __Layout();
}

MenuPageText::~MenuPageText()
{
    m_refEngineProxy->GetSpriteRenderer().DestroySprite(m_spriteTitle);
    m_refEngineProxy->GetTextureManager().DestroyTexture(m_texTitle);
    m_refEngineProxy->GetFontManager().DestroyFont(m_font);
}

void MenuPageText::Activate(bool activate)
{
    MenuPage::Activate(activate);

    m_refEngineProxy->GetSpriteRenderer().SpriteSetActive(m_spriteTitle, activate);

    for (auto& it : m_items) {
        it->Activate(activate);
    }
}

void MenuPageText::__Layout()
{
    uint16_t originX = 20;
    uint16_t originY = 20;
    auto offsetY = static_cast<uint16_t>(m_fontSize + 5);

    m_refEngineProxy->GetSpriteRenderer().SpriteSetPos(m_spriteTitle, originX, originY);

    originX += 20;
    originY += offsetY;

    for (size_t i = 0; i < m_items.size(); i++) {
        m_refEngineProxy->GetSpriteRenderer().SpriteSetPos(dynamic_cast<MenuItemText*>(m_items[i])->m_sprite, originX,
            static_cast<uint32_t>(originY + i * offsetY));
    }
}

}