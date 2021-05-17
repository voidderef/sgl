#include "MenuItemText.h"

namespace sgl {

MenuItemText::MenuItemText(EngineProxy* refEngineProxy, const std::string& fontName, uint16_t size, std::string name,
        const std::function<ssize_t()>& callback) :
    MenuItem(std::move(name), callback),
    m_refEngineProxy(refEngineProxy)
{
    m_font = m_refEngineProxy->GetFontManager().CreateFontFromTTFFile(
        m_refEngineProxy->GetDataManager().GetRefFsFont()->GetAbsolutePath(fontName), size);
    m_texNormal = m_refEngineProxy->GetTextureManager().CreateTextureFromText(
        GetName(), 0xFF, 0xFF, 0xFF, 0xFF, m_font);
    m_texHighlighted = m_refEngineProxy->GetTextureManager().CreateTextureFromText(
        GetName(), 0xFF, 0x00, 0x00, 0xFF, m_font);
    m_sprite = m_refEngineProxy->GetSpriteRenderer().CreateSprite(m_texNormal);
}

MenuItemText::~MenuItemText()
{
    m_refEngineProxy->GetSpriteRenderer().DestroySprite(m_sprite);
    m_refEngineProxy->GetTextureManager().DestroyTexture(m_texHighlighted);
    m_refEngineProxy->GetTextureManager().DestroyTexture(m_texNormal);
    m_refEngineProxy->GetFontManager().DestroyFont(m_font);
}

void MenuItemText::Activate(bool activate)
{
    m_refEngineProxy->GetSpriteRenderer().SpriteSetActive(m_sprite, activate);
}

void MenuItemText::SetHighlighted(bool highlighted)
{
    m_refEngineProxy->GetSpriteRenderer().SpriteSetTexture(m_sprite, highlighted ? m_texHighlighted : m_texNormal);
}

}